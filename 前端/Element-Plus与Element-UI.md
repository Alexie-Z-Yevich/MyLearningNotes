# Element-Plus&&Element-UI

> **写在前面：**
>
> 作为个人的一个学习文档，最近开了一个新坑，跟着20年的视频做了一个前端的demo，但是人家的前端配置是v-cli+vue2+element-ui+axios，而我决定使用当下比较新的技术栈：vite+vue3+element-plus+axios，所以不可避免地在过程中遇到了许多因为跨版本而留下的坑。这篇文档就以element-ui和element-plus为起点，记录简单的开发流程以供参考，以及一些已经发生变更的代码。方便未来借鉴与参考。

在此基础上，在所有流程中仅针对使用vite+vue3技术栈的开发人员。（node.js环境）

## 安装&问题详解（待补充）

###### **1、使用包管理器 (NPM) 安装 Element Plus**

```
$ npm install element-plus --save
$ cnpm install element-plus --save  // 通过中国镜像下载，但最好所有安装方式保持一致
```

------

###### 2、问题详解

1、Vue3中使用Element-Plus与Element-UI不兼容问题：Element-UI是饿了么开发的前端组件库，主要针对的是Vue框架，但是Vue3相比Vue2有了很多改动，一些函数、方法都被弃用了，因此在使用中遇到了不少需要解决的问题。

（1）icon的使用方式：在Element-UI中icon是作为class样式引用的固定字段，但是Element-Plus中为了方便开发者自定义icon的样式，将这些字段变成了标签。因此如果需要动态传输icon的内容（比如从后端向前传）就不能使用{{}}来进行引用了，而是要把要用到的icon先从Element-Plus中提取出来，放入component中实例化后才能使用；

（2）插槽的更新：slot具名插槽和slot-scope作用域插槽在Vue3中已经被弃用，在Vue3中使用v-slot取代了原有的插槽；

（3）弹出框的更新：Element-UI的各种弹出框使用:visible.sync方法绑定属性实现弹出框的显示、隐藏，但是实际上这句话的内核还是调用的Vue里的方法，所以在Element-Plus中舍弃了这个方法，直接使用更为简单直接的v-model实现对于属性的双向绑定；

（4）关于文档的阅读：Element-Plus用的是TypeScript语法体系，所以有些地方反而使用不了，ElementUI虽然适配的是Vue2，但是使用的是JavaScript语法体系，在export default中参考ElementUI会方便很多。但是实际上TS体系比JS体系要安全方便一些，但是因为本人能力有限，在JS都不是很熟悉的情况下不敢贸然转到TS，但是值得一提的是：未来的趋势一定是TS。

 

2、Vue3中require方法的弃用：require是node.js环境下对于静态资源的一种调用方式，但是因为它属于node.js环境，所以在浏览器上实际上是不生效的。同样，如果用require请求的是图片资源的话，在后续前后端合并时会出现找不到的情况，所以取消require使用import来引用资源，同时个人建议图片等资源放到远程仓库（Github）中通过URL来使用，可以避免很多不必要的麻烦。

 

3、mock.js的使用：在还没有开发后端的情况下，对前端进行数据渲染时通常需要获取后端的数据，目前网上的教程都是教大家使用开源的mock.js去Mock数据，但是近几年有不少国内外的接口软件崭露头角，比如说postman、Apifox等。这里我使用的是Apifox向前后端发送Mock数据。