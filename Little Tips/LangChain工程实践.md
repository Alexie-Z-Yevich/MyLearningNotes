# LangChain 实践

> 前言：
>
> 该文档为《LangChain 入门与避坑指北》的项目实践，实际上就是将
>
> [ChatGLM+Langchain构建本地知识库，只需6G显存，支持实时上传文档](https://www.bilibili.com/video/BV1t8411y7fp?vd_source=75fc8fb6d21bda88e7f19b3fe3d00dbb)
>
> [九天玩转Langchain！](https://www.bilibili.com/video/BV1E94y187YX?vd_source=75fc8fb6d21bda88e7f19b3fe3d00dbb)
>
> 中的项目进行复现与注释~



## 1、本地知识库问答（文本）

```python
import os

from langchain.chains import RetrievalQA  # 导入 RetrievalQA 类用于检索问答
from langchain.embeddings.huggingface import HuggingFaceEmbeddings
from langchain.text_splitter import CharacterTextSplitter
from langchain_community.document_loaders import DirectoryLoader
from langchain_community.vectorstores import Chroma

from langchain_chatglm3 import ChatGLM3  # 自定义 ChatGLM3 类用于加载模型

# 加载embeddings
embeddings_model_dict = {
    "ernie-tiny": "nghuyong/ernie-3.0-nano-zh",
    "ernie-base": "nghuyong/ernie-3.0-base-zh",
    "text2vec": "GranymeNil/text2vec-large-chinese",
    "text2vec2": "uer/sbert-base-chinese-nil",
    "text2vec3": "shibing624/text2vec-base-chinese",
}


# 这里默认是同级目录下的 data 文件夹，将txt文件放入 data 文件夹即可
def load_document(directory="data"):
    loader = DirectoryLoader(directory)  # 加载文档
    documents = loader.load()
    text_splitter = CharacterTextSplitter(chunk_size=256, chunk_overlap=0)
    split_docs = text_splitter.split_documents(documents)  # 切分文档
    print(split_docs[:2])
    return split_docs


def load_embedding_mode(model_name="ernie-tiny"):
    encode_kwargs = {"normalize_embeddings": False}
    model_kwargs = {"device": "cuda:0"}
    return HuggingFaceEmbeddings(
        model_name=embeddings_model_dict[model_name],
        encode_kwargs=encode_kwargs,
        model_kwargs=model_kwargs
    )


def store_chroma(docs, embeddings, persist_directory='VectorStore'):
    # 存储 Chroma 向量化的文档
    db = Chroma.from_documents(docs, embeddings, persist_directory=persist_directory)
    db.persist()
    return db


embeddings = load_embedding_mode('text2vec3')  # 加载嵌入模型
if not os.path.exists("Vectorstore"):
    documents = load_document()  # 加载文档
    db = store_chroma(documents, embeddings)  # 存储向量化的文档
else:
    # 加载已存在的向量化文档
    db = Chroma(persist_directory='VectorStore', embedding_function=embeddings)

# 创建 ChatGLM3 实例
llm = ChatGLM3(endpoint_url="http://localhost:8000/", max_token=5000)

retriever = db.as_retriever()  # 创建文档检索器
qa = RetrievalQA.from_chain_type(
    llm=llm,
    chain_type='stuff',
    retriever=retriever
)  # 装填至 chain

response = qa.invoke('你的数据库中有几本书？')  # 使用 QA 模型对问题进行推理
print(response)
```

**注意：**这个不能 copy 即用，使用的是 GLM3 模型，可以换成其他本地模型如 GLM，从控制台查看引入包即可。



## 2、本地知识库问答（PDF）

- langchain_helper.py （实现方法的工具类）

```python
import os

from langchain_community.document_loaders import PyPDFLoader  
from langchain.embeddings.openai import OpenAIEmbeddings
from langchain.indexes.vectorstore import VectorstoreIndexCreator
from langchain.vectorstores.chroma import Chroma  
from langchain.indexes.vectorstore import VectorStoreIndexWrapper  #

os.environ["OPENAI_API_BASE"] = 'https://~'  # 设置模型接口位置
os.environ["OPENAI_API_KEY"] = 'sk-~'        # 设置模型 KEY
file_path = "*.pdf"  # PDF 文件路径
local_persist_path = "./vector_store"  # 向量存储的本地持久化路径


# 获取索引路径
def get_index_path(index_name):
    return os.path.join(local_persist_path, index_name)


# 加载PDF文档并保存索引
def load_pdf_and_save_index(file_path, index_name):
    loader = PyPDFLoader(file_path)  # 使用 PyPDFLoader 加载PDF文档
    index = VectorstoreIndexCreator(vectorstore_kwargs={"persist_directory": get_index_path(index_name)}). \
        from_loaders([loader])  # 创建向量存储索引
    index.vectorstore.persist()  # 持久化索引


# 加载索引
def load_index(index_name):
    index_path = get_index_path(index_name)  # 获取索引路径
    embedding = OpenAIEmbeddings()  # 使用 OpenAIEmbeddings 加载嵌入模型
    vectordb = Chroma(
        persist_directory=index_path,
        embedding_function=embedding
    )  # 创建 Chroma 向量存储
    return VectorStoreIndexWrapper(vectorstore=vectordb)  # 返回向量存储索引的包装类


def query_index_lc(index, query):
    # 使用 map_reduce 方法查询索引
    ans = index.query_with_sources(query, chain_type="map_reduce") 
    return ans['answer']

```



- main.py （使用 fastapi 制作的简单接口交互）

```python
from fastapi import FastAPI  # 导入 FastAPI 框架
from typing import Annotated  # 导入 Annotated 用于在类型提示中注释
from fastapi import File, Form, UploadFile
from langchain_helper import *  # 导入 langchain_helper 中的函数
from pydantic import BaseModel  # 导入 BaseModel 用于定义数据模型
import os

file_upload_path = "./uploads"  # 文件上传路径
app = FastAPI()  # 创建 FastAPI 应用


# 定义根路径的 GET 请求处理函数，返回一个简单的 JSON 响应
@app.get("/")
def read_root():
    return {"Hello": "World"}  


@app.post("/upload")
def upload_file(file: Annotated[UploadFile, File()], 
                index_name: Annotated[str, Form(...)]):
    # 指定文件上传目标路径
    file_upload_target_path = os.path.join(file_upload_path, file.filename)  

    with open(file_upload_target_path, "wb") as f:
        contents = file.file.read()
        f.write(contents)  # 将接收到的文件内容写入目标文件路径

    # 调用 load_pdf_and_save_index 函数对上传的文件进行处理
    load_pdf_and_save_index(file_upload_target_path, index_name)  
    # 返回上传文件的文件名和索引名
    return {"filename": file.filename, "index_name": index_name}  


class Query(BaseModel):
    index_name: str
    query: str


@app.post("/query")
def query_index(request: Query):
    index_name = request.index_name
    query = request.query
    index = load_index(index_name)  # 调用 load_index 函数加载索引
    ans = query_index_lc(index, query)  # 使用索引进行查询
    # 返回查询结果、索引名和查询内容
    return {"answer": ans, "index_name": index_name, "query": query}  
```



## 3、网页新闻转相声

- html2news.py （网页新闻转相声工具类）

```python
import os  # 导入os模块

from langchain_community.document_loaders import UnstructuredURLLoader 
from langchain.text_splitter import RecursiveCharacterTextSplitter  
from langchain.chains.summarize import load_summarize_chain  
from langchain_openai import OpenAI, PromptTemplate, ChatOpenAI  
from langchain.schema import HumanMessage  
from langchain.output_parsers import PydanticOutputParser  
from pydantic import BaseModel, Field  

# 设置环境变量 OPENAI_API_BASE 和 OPENAI_API_KEY
os.environ["OPENAI_API_BASE"] = 'https://~'
os.environ["OPENAI_API_KEY"] = 'sk-~'

# 定义新闻的URL（这里是当天随便找的新浪新闻，实测知乎的文章不太好用）
url = "https://news.sina.com.cn/zx/gj/2024-01-27/doc-inaexruh3759336.shtml"


# 将URL转换为新闻文本
def url2News(url):
    # 初始化 RecursiveCharacterTextSplitter 实例
    # 从 “盛世 开始（包含），到 “看，这幅图 结束（不包含）
    # 块大小为 1000，块重叠为 20，默认 len 方法计算块长度
    text_splitter = RecursiveCharacterTextSplitter(separators=["“盛世", "“看，这幅图"], chunk_size=1000, chunk_overlap=20, length_function=len)
    # 初始化 UnstructuredURLLoader 实例
    loader = UnstructuredURLLoader([url])
    # 加载并分割文本
    data = loader.load_and_split(text_splitter)
    return data[1:2]


# 将新闻文本转换为相声剧本
def news2script(news):
    # 初始化 OpenAI 实例
    llm = OpenAI(max_tokens=1500)

    # 定义提示模板
    prompt_template = """总结这段新闻的内容：
    "{text}"
    总结:"""

    # 初始化 PromptTemplate 实例
    chinese_prompt = PromptTemplate(template=prompt_template, input_variables=["text"])

    # 加载相声剧本生成模型
    chain = load_summarize_chain(llm, prompt=chinese_prompt)

    # 运行模型生成相声剧本
    summary = chain.invoke(news)
    print(summary['output_text'])

    # 初始化 ChatOpenAI 实例
    openaichat = ChatOpenAI(model_name="gpt-3.5-turbo")

    # 定义相声剧本模板
    template = """\
        我将给你一段新闻的概括，请按照要求把这段新闻改写成郭德纲和于谦的对口相声剧本。
        新闻: "{新闻}"
        要求: "{要求}"
        {output_instructions}
    """
    # 初始化 PydanticOutputParser 实例，设置相声的数据类型
    parser = PydanticOutputParser(pydantic_object=XiangSheng)
    # 初始化 PromptTemplate 实例
    prompt = PromptTemplate(
        template=template,
        input_variables=["新闻", "要求"],
        # 装载输出类型
        partial_variables={"output_instructions": parser.get_format_instructions()}
    )

    # 创建人类消息列表
    msg = [HumanMessage(
        content=prompt.format(新闻=summary['output_text'],
                要求="风趣幽默，十分讽刺，剧本对话角色为郭德纲和于谦，以他们的自我介绍为开头"))]

    # 运行对话生成相声剧本
    res = openaichat.invoke(msg)
    print(res.content)
    # 解析相声剧本
    xiangsheng = parser.parse(res.content)
    return xiangsheng


# 定义台词行数据模型
class Line(BaseModel):
    character: str = Field(description="说这句台词的角色名字")
    content: str = Field(description="台词的具体内容，其中不再包括角色的名字")


# 定义相声剧本数据模型
class XiangSheng(BaseModel):
    script: List[Line] = Field(description="一段相声的台词剧本")


# 将URL转换为相声剧本
def url2xiangsheng(url):
    doc = url2News(url)
    xiangsheng = news2script(doc)
    return xiangsheng
```



- interface.py （gradio 绘制的简易前端）

```python
import gradio as gr  # 导入 gradio 库
from html2news import *  # 导入 html2news 模块，其中包含之前的函数和类定义

with gr.Blocks() as demo:  # 创建 Gradio Blocks 应用

    url = gr.Textbox()  # 创建一个文本框组件，用于输入新闻的URL
    chatbot = gr.Chatbot()  # 创建一个聊天机器人组件
    submit_btn = gr.Button("生成相声")  # 创建一个按钮组件用于提交生成相声的请求

    # 定义生成对话的函数
    def generate_conversation(url):
        # 调用之前定义的函数，将URL转换为相声剧本
        xiangsheng: XiangSheng = url2xiangsheng(url)  
        chat_history = []  # 创建一个列表，用于存储对话内容

        # 定义解析台词的函数
        def parse_line(line: Line):
            if line is None:
                return ""
            return f'{line.character}: {line.content}'

        # 将生成的相声剧本转换为对话历史
        for i in range(0, len(xiangsheng.script), 2):
            line1 = xiangsheng.script[i]
            line2 = xiangsheng.script[i + 1] if (i + 1) < len(xiangsheng.script) 
                                             else None
            chat_history.append((parse_line(line1), parse_line(line2)))

        return chat_history  # 返回生成的对话历史

    # 点击按钮时触发生成对话函数，将URL作为输入，将聊天机器人组件作为输出
    submit_btn.click(fn=generate_conversation, inputs=url, outputs=chatbot)

if __name__ == '__main__':
    demo.launch()  # 运行 Gradio Blocks 应用
```



## 总结

希望上面三个简单的 Demo 能够帮助你更好的理解 LangChain 中各个模块的关系，以及一些基础的工程实践流程。复杂的项目往往只需要拆解成简单的需求，对着官方文档一顿操作即可~