# JavaWeb

> 写在前面：这篇文档旨在对于[LyFive的文档](https://github.com/Lyfive/HnusterComputer/blob/main/JavaWeb/JavaWeb%E5%A4%8D%E4%B9%A0%E6%96%87%E6%A1%A3.md)进行一个补充（也就是仅自用的意思），我的更注重于详细的通俗易懂文字描述以及课后习题的解答，而他的主要是给大家画了流程图，方便大家了解体系结构。建议先看他那版后再来参阅我的OWO。

#### 第一章 Web应用开发简介

###### 1、什么是C/S结构？什么是B/S结构？他们各有什么优缺点？

C/S即客户端/服务器结构，服务器常采用高性能的PC或工作站，并采用大型数据库；B/S即浏览器/服务器结构，客户端不需要开发任何用户界面，而统一采用浏览器，通过Web浏览器向Web服务器发送请求。

优缺点：

（1）**C/S结构开发成本高于B/S结构**，在C/S结构中开发者需要对所有机型机型进行适配，而在B/S结构中只需要兼容浏览器服务即可；

（2）C/S结构的客户端负责与用于交互、收集用户信息、完成通过网络向服务器请求对数据库信息进行处理工作；B/S结构的客户端把事务处理逻辑部分交给服务器处理，客户端只需要进行显示。对于**复杂的功能**来说，**C/S结构劣于B/S结构**，因为客户端程序庞大；但对于**服务器负载较重**，出现“崩溃”现象时，**C/S结构优于B/S结构**，因为数据能够得以保全。

（3）**C/S结构适用于专人使用**的系统，**安全性比较高**；**B/S结构适用于多人使用**，**相对安全性低**一些。

------

###### 2、常见的C/S和B/S结构的例子

（1）下载的游戏-----网页游戏；

（2）QQ-----QQ邮箱；

（3）企业微信-----教务网。

（简单理解：一切网站访问的都属于B/S结构，一切APP、客户端访问的都属于C/S结构）

------

###### 3、Web客户端技术有哪些？Web服务器端技术有哪些？

客户端技术：

（1）HTML；（2）CSS；（3）Flash；（4）客户端脚本技术（JavaScript、TypeScript等）

服务器端技术：

（1）CGI；（2）ASP；（3）PHP；（4）ASP.NET；（5）JSP

------

#### 第二章 网页前端开发基础

###### 1、HTML由哪几部分组成？

HTML文档主要由4个主要标记组成。

- \<html>标记：是html文件的开头，html页面的所有标记都要放在<html></html>标记
  中，此标记没有实质性的功能。但是HTML文件不可缺少的部分。
- \<head>标记：是HTML文件的头标记，作用是放置HTML文件的信息。如：标题、css样式代码等。

- \<title>标记：网页标题，放置在head内。

- \<body>标记：是HTML页面的主体标记。页面的所有内容都定义在body标记内。

------

###### 2、HTML有哪些常用标记？都有什么作用？

- 这个在LyFive的文档中有详细的描述，这里就不过多赘述。

------

###### 3、\<input>标记有几种输入类型？

- text文本框
- password密码域
- file文件域
- radio单选按钮
- checkbox复选按钮
- submit提交按钮
- reset重置按钮
- button普通按钮
- hidden隐藏域
- image图像域

------

###### 4、什么是CSS？CSS有哪些效果？

CSS（层叠样式表）是W3C协会为弥补HTML在显示属性设定上的不足而制定的一套扩展样式标准。通过引入CSS可以对HTML文件进行美化，不必要引入过多资源，加快页面访问速度。

------

###### 5、如何为一个HTML页面添加CSS效果？

- 内样式：直接写在HTML标签内，通过style=“”引入CSS样式。
- 内嵌式：在页面中使用\<style>标签，将CSS代码包含在其中。
- 链接式：将CSS代码写在*.css文件里，在HTML中使用\<link href='\*.css'>链接外部文件。（href：CSS文档的绝对或相对路径；rel：定义外部文档与调用文档的关系）
- 导入式：@import 是 CSS 2.1 特有的。

```html
<!--导入式引入外部样式-->
<style>
    @import url("css/style.css");
</style>
```

**链接式与导入式的区别：**首先 link 和 import 语法结构不同，前者 <link> 是 html 标签，只能放入 html 源代码中使用，后者 import 可看作 css 样式，作用是引入 css 样式功能。import 在 html 使用时需要 <style type="text/css"> 标签，同时可以直接“@import url(CSS文件路径地址);” 放入 css 文件或 css 代码里引入其他 css 文件。

------

#### 第三章 JavaScript脚本语言

###### 1、什么是JavaScript？JavaScript和Java是什么关系？

JavaScript是一种基于对象和事件驱动并具有安全和性能的解释性脚本语言，在Web应用中得到了非常广泛的应用。他不需要进行编译，而是直接嵌入在http页面中。js常用来进行数据验证、控制浏览器以及生成时钟、日历和时间戳文档等。

JavaScript与Java除了语法有一些相似之外，两者毫不相关。

JS具有**解释性**、**基于对象**、**事件驱动**、**安全性**、**跨平台**的特点。

------

###### 2、JavaScript脚本如何调用？JavaScript有哪些常用的属性和方法？

+ 直接嵌入：在HTML代码中直接使用\<script>标签引入JS代码。
+ 链接：在\<script>标签中使用src属性引入外部的*.js文件。

常用属性和方法直接参考[JS的官方文档](https://developer.mozilla.org/en-US/docs/Web/JavaScript)。选择写几个即可。

------

###### 3、什么是Ajax？如何用Ajax实时更新前台页面数据？

Ajax的意思是异步的JavaScript和xml，它是js、xml、css、dom等多种已有技术的组合，可以实现客户端的异步请求操作，进而不需要刷新页面的情况下与服务器进行通信，减少用户的等待时间，减轻服务器和带宽的负担，提供更好的服务响应。

Ajax接收前端发来的信息，通过XMLHttpRequest对象向后端发送请求，同时接收response对象，将它部署到前端页面。

------



#### 第五章 走进JSP

###### 1、什么是JSP？

JSP是在HTML代码中嵌入Java代码片段和JSP标签，构成了JSP网页。主要由指令标签、HTML语句、注释、嵌入Java代码、JSP动作标签等5个元素组成。

------

###### 2、JSP有哪些指令标签？

- page指令：定义整个JSP页面的相关属性，这些属性会在解析成Servlet时转换为对应的Java程序代
  码。

  - 格式： <%@ page attr1 = "value1" attr2 = "value2" %>

  - 属性： import 导包，pageEncoding 文件编码，contentType 设置页面文件类型
    （如"text/html")，include 导入其他文件(和CPP的include基本一致)
- 嵌入Java代码
  - 格式 <% JavaCode %>
- 声明

  - 格式 <%! JavaCode %>

  - 声明全局变量： <%! long startTime = System.nanoTime(); %>

  - 声明全局方法： <%! int Max(int a,int b){return a > b ? a : b ;} %>
- JSP表达式
    - 格式 <%= 表达式 %>
    - 例如： <%= 2*Math.PI %>会把2Pi的结果直接输出在页面上，类型为字符串
- 注释
     - HTML注释： <!-- 注释内容 -->
     - JSP注释： <%-- 注释内容 --%>
     - 动态注释： 把JSP代码嵌入到HTL注释内，如 <!-- <%= new Date() %> -->
     - JSP内部注释和Java注释一致。 // 和 /* */

------

###### 3、如何在JSP中运行Java程序？

见上体的标签。

------

###### 4、什么是request对象？什么是response对象？什么是session对象？什么是application对象？这些对象有哪些共同点和不同点？

- request对象：是javax.servlet.http.HTTPServletRequest类型的对象。代表了客户端的请求信息，主要用于接收HTTP传送到服务器端的数据（包括头信息Header、系统信息、请求方式Methods以及请求参数Params等）。作用域为一次请求。
- response对象：代表的是对客户端的响应，主要是将JSP容器处理过的对象传回客户端。作用域为只在JSP页面内有效。
- session对象：由服务器自动创建的与用户请求相关的对象。一个用户对应一个session对象，用于保存用户的信息，追踪用户的操作状态。（内部使用Map类来保存数据）
- application对象：可将信息保存在服务器中直到服务器关闭（类似于缓存区？），相比于session对象，application对象的生命周期更长，类似于系统的“全局变量”。

共同点：

都是asp中的作用域。（作用域详细的说就是 “信息共享的范围”，也就是一个信息能够在多大的范围内有效）

不同点：

作用域大小各不相同，作用对象也不同。request主要作用于请求；response作用于JSP页面；session和application作用于用户，但是session与用户是一对一关系，而application与用户是一对多关系。

request、response以协议为基础，为此他们面向的是参数Parameter、头信息Header；sesession、application以Attribute属性为基础，为此属性的增删改查基本都一致。

**扩展：**cookie是小段的文本信息，通过cookie可以标识用户身份、记录用户名及密码、追踪重复用户。cookie是由服务端生成的。

cookie和session的区别：

1、cookie存储在客户端，SESSION存储在服务器；

2、cookie机制保持连接，通信时压力在客户端；session机制保持连接，通信时压力在服务器；

3、session机制更安全，因为cookie存放在客户端，容易被窃取。但是session机制依赖于sessionID,而sessionID保存在cookie中，一旦客户端禁用了cookie，session也将失效；

4、cookie是以文本的形式存储在客户端，存储量有限(<=4KB)；session可以无限量地往里面添加内容；

5、cookie支持跨域名访问，session不支持跨域名访问；

6、cookie可以设置为长期有效，而session会随会话窗口关闭而失效。

------



#### 第六章 Servlet技术

###### 1、web.xml文件是做什么用的？

web.xml用来配置Servlet，使Servlet对象能够正常运行。（告知web容器哪一个请求调用哪一个Servlet对象处理，对Servlet起到一个注册的作用）

------

###### 2、Servlet都有哪些接口？这些接口都有什么作用？

- Servlet接口：Servlet运行需要Servlet容器的支持，servlet容器通过调用Servlet对象提供了标准的API接口，对请求进行处理。简单来说就是Servet的生命周期，包含init初始化、service处理请求、getServlet config获取配置信息、getServletInfo返回配置信息、destory销毁对象五个过程。
- ServletConfig接口：封装Servlet的配置信息，每个Servlet都有且只有一个ServletConfig对象。
- HttpServletRequest接口：继承于javax.servlet.ServletRequest接口，用于接收前端传来的的基本信息（路径、参数、类型、方法等）。
- HttpservletResponse接口：继承于javax.servlet.ServletResponse接口，用于向前端发送后端的处理信息（cookie、状态码等）。

------

###### 3、如何指定项目默认页面？

在web.xml中对servlet进行配置，通过配置\<servlet-mapping>指定项目默认页面。

```java
<servlet>
		<servlet-name>Servlet</servlet-name>  // 设置Servlet名字
   		<servlet-class>com.*.Servlet</servlet-class>  // 导入class类文件
</servlet>
<servlet-mapping>
        <servlet-name>Servlet</servlet-name>  // 找到映射对象
        <url-parttern>/</url-parttern>  // 设置映射地址
</servlet-mapping>
```

------

###### 4、如何使用过滤器？过滤器中有哪些方法？它们运行的顺序是什么？

- 在web.xml中配置\<filter>标签以及\<filter-mapping>标签配置过滤器。
- 过滤器中的方法：init初始化、doFilter执行过滤操作、destory销毁（主要方法）。
- 运行顺序：初始化->执行->销毁，但是注意：过滤器之间同样有优先顺序，在web.xml上面的配置先于下面的配置执行，因此需要注意过滤器把其他过滤器给过滤掉了。

------



#### 第七章 数据库技术

###### 1、简述JDBC连接数据库的基本步骤。

- 导入JDBC驱动；
- 输入数据库信息连接数据库；
- 预载SQL语句；
- 更新数据库。（这个是老师上课讲的，没有照着书本来）

```java
Class.forName("com.mysql.jdbc.Driver");  // 导入驱动（需要try/catch语句包裹）
Connection connection = DriverManager.getConnection("jdbc:mysql://127.0.0.1:3306/MysqlDatabase", "username", "password");
PreparedStatement preparedStatement = connection.preparedStatement("select * from MysqlTable");  // 动态加载将预载和更新融为一步。
```

------

###### 2、执行动态SQL语句的接口是什么？& 3、JDBC提供的两种实现数据查询的方法是什么？

- Statement接口执行静态SQL语句；
- PreparedStatement接口执行动态SQL语句。

------

###### 4、Statement类中的两个方法：executeQuery()和executeUpdate()，两者的区别是什么？

- executeUpdate()：返回int类型数值，代表影响数据库记录的条数；（对于增删改操作）
- executeQuery()：返回ResultSet类型的结果集，包含了返回的所有信息。（对应查操作）

------



#### 第九章 Spring MVC框架

###### 1、MVC模式由哪几部分组成？

MVC（Model-View-Controller，模型-视图-控制器）是一个存在服务器表达层的模型。

------

###### 2、WEB-INF目录下的资源有什么特点？需要如何访问？

WEB-INF目录下存放安全性较高的资源文件，不能直接通过浏览器直接访问的，需要通过服务器来访问。在外部通过配置控制层映射访问文件。

------

###### 3、Spring MVC的拦截器有哪些方法？这些方法各有什么特点？

- HanderInterceptor接口：
  - preHandle：对请求进行判断，决定程序是否继续执行；
  - postHandle：同意处理返回的视图；
  - agterCompletion：进行统一的异常或者日志处理操作。

- WebRequestInterceptor接口：

  - preHandle：方法的前期准备；

  - postHandle：改变ModelMap中的属性来改变Controller最终返回的Model模型；
  - agterCompletion：将WebRequest参数中不需要的准备资源释放掉。

------



#### 第十一章 Spring框架

###### 1、什么是IoC注入？如何使用Spring框架进行注入？

IoC（控制反转）和DI密不可分，IoC是在设计的角度看Spring，DI是在Sping的角度看代码，角度不同但是含义一样：依赖注入--将原本实例化的功能从开发者给到了Spring。

注入方法：

- 接口注入：实现容器所规定的接口，使程序代码和容器的API绑定在一起；
- Setter注入：基于JavaBean的Setter方法为属性赋值；
- 构造器注入：基于构造方法为属性赋值，在实例化对象的同时完成了属性的初始化。

------

###### 2、什么是AOP？

AOP是一种思想，即面向切面编程。基于Java的代理机制，使方法与类分离。

------

###### 3、Spring框架有哪些项目开发优势？

1.低侵入式设计，重复代码量少；

2.独立于各种应用服务器，基于Spring框架的应用，可以真正实现一个代码，多平台复用；

3.Spring的DI机制降低了业务对象替换的复杂性，提高了组件之间的解耦；

4.Spring的AOP支持允许将一些通用任务如安全、事务、日志等进行集中式管理，从而提供了更好的复用；

5.Spring的ORM和DAO提供了与第三方持久层框架的良好整合，并简化了底层的数据库访问；

6.Spring并不强制应用完全依赖于Spring，开发者可自由选用Spring框架的部分或全部。

------



#### 第十二章 SSM框架整合应用

###### 什么是SSM？

SSM（Spring+SpringMVC+MyBatis）框架集由Spring、MyBatis两个开源框架整合而成（SpringMVC是Spring中的部分内容），常作为数据源较简单的web项目的框架。

**Spring**
Spring就像是整个项目中装配bean的大工厂，在配置文件中可以指定使用特定的参数去调用实体类的构造方法来实例化对象。也可以称之为项目中的粘合剂。

Spring的核心思想是IoC（控制反转），即不再需要程序员去显式地 new 一个对象，而是让Spring框架帮你来完成这一切。

**SpringMVC**
SpringMVC在项目中拦截用户请求，它的核心Servlet即DispatcherServlet承担中介或是前台这样的职责，将用户请求通过HandlerMapping去匹配Controller，Controller就是具体对应请求所执行的操作。SpringMVC相当于SSH框架中struts。

**Mybatis**
mybatis是对jdbc的封装，它让数据库底层操作变的透明。mybatis的操作都是围绕一个sqlSessionFactory实例展开的。mybatis通过配置文件关联到各实体类的Mapper文件，Mapper文件中配置了每个类对数据库所需进行的sql语句映射。在每次与数据库交互时，通过sqlSessionFactory拿到一个sqlSession，再执行sql命令。