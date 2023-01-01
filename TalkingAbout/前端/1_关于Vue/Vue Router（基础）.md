# Vue Router

## **常规html中使用路由**

```html
<!-- 通过设置router-link标签实现链接的跳转 -->
<router-link to="/">Go to Home</router-link>
    
<!-- 启动router-link跳转 -->
<router-view></router-view>
```



## **常规项目中router文件夹下.js文件的书写方式**

```js
import {createRouter, createWebHashHistory} from 'vue-router'
import Home from '../views/Home.vue'
import Login from '../views/Login.vue'
import User from '../views/Index.vue'  // 引入视图文件

const routes = [
  {
    path: '/',
    redirect: '/home'  // 重定向
  },
  {
    path: '/home',  // 路由地址
    name: 'Home',  // 路由名称
    component: Home,  // 对应组件
    children: [  // 子路由（嵌套路由）
      {
        path: '/index',
        name: 'Index',
        component: Index
      }
    ]
  },

  {
    path: '/login',
    name: 'Login',
    component: Login
  }
]

const router = createRouter({
  history: createWebHashHistory(),
  routes
})

export default router
```

#### 关于嵌套路由
<router-view>是一个顶层的 <router-view>。它渲染顶层路由匹配的组件。同样地，一个被渲染的组件也可以包含自己嵌套的 <router-view>



* 如果只是应付项目作业，那么到这里基本已经够用了，以下是对于官方文档的一点点不成熟见解与引用。

------



## 编程式导航

**注意：在 Vue 实例中，你可以通过 $router 访问路由实例。因此你可以调用 this.$router.push。**

想要导航到不同的 URL，可以使用 router.push 方法。这个方法会向 history 栈添加一个新的记录，所以，当用户点击浏览器后退按钮时，会回到之前的 URL。

当你点击 <router-link> 时，内部会调用这个方法，所以点击 <router-link :to="..."> 相当于调用 router.push(...)

![编程式导航-1](https://raw.githubusercontent.com/Alexie-Z-Yevich/MyLearningNotes/master/img/image-20220506105927372.png)

```js
// 字符串路径
router.push('/users/eduardo')

// 带有路径的对象
router.push({ path: '/users/eduardo' })

// 命名的路由，并加上参数，让路由建立 url
router.push({ name: 'user', params: { username: 'eduardo' } })

// 带查询参数，结果是 /register?plan=private
router.push({ path: '/register', query: { plan: 'private' } })

// 带 hash，结果是 /about#team
router.push({ path: '/about', hash: '#team' })
```

**注意：**如果提供了 path，params 会被忽略，上述例子中的 query 并不属于这种情况。取而代之的是下面例子的做法，你需要提供路由的 name 或手写完整的带有参数的 path:

```js
const username = 'eduardo'
// 我们可以手动建立 url，但我们必须自己处理编码
router.push(`/user/${username}`) // -> /user/eduardo
// 同样
router.push({ path: `/user/${username}` }) // -> /user/eduardo
// 如果可能的话，使用 `name` 和 `params` 从自动 URL 编码中获益
router.push({ name: 'user', params: { username } }) // -> /user/eduardo
// `params` 不能与 `path` 一起使用
router.push({ path: '/user', params: { username } }) // -> /user
```

可以看出，路径使用的是动态的$占位符，能很大程度降低代码的耦合。



## 基础知识点

###  1、name 的优势

（1）没有硬编码的 URL。
（2）params 的自动编码/解码。
（3）防止你在 url 中出现打字错误。
（4）绕过路径排序（如显示一个）。

### 2、redict重定向

```js
// 直接重定向到地址
redirect: '/'

// 重定向到名称，通过名称定位地址（有效降低耦合）
redirect: { name: 'homepage' }

// 重定向也可以是方法，动态返回重定向目标
redirect: to => {
      return { path: '/search', query: { q: to.params.searchText } }
}
```

**注意：**导航守卫并没有应用在跳转路由上，而仅仅应用在其目标上。

### 3、别名

```js
alias: '/home'
```

通过别名，你可以自由地将 UI 结构映射到一个任意的 URL，而不受配置的嵌套结构的限制。

### 4、props

```js
const routes = [{ path: '/user/:id', component: User }]
// 替换成如下
const routes = [{ path: '/user/:id', component: User, props: true }]
```

通过加入**props: true**，你可以在多个地方使用该路由，使得该组件更容易重用和测试。

当 props 设置为 true 时，route.params 将被设置为组件的 props。

### *5、历史模式

history 配置允许我们在不同的历史模式中进行选择。

#### （1）Hash 模式

```js
import { createRouter, createWebHashHistory } from 'vue-router'

const router = createRouter({
  history: createWebHashHistory(),
  routes: [
    //...
  ],
})
```

由于这部分 URL 从未被发送到服务器，所以它不需要在服务器层面上进行任何特殊处理。不过，它在 **SEO 中确实有不好的影响**。

#### （2）HTML5 模式（推荐）

```js
import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(),
  routes: [
    //...
  ],
})
```

使用这种历史配置，URL会看起来干净漂亮。但是需要适当的服务器配置，如果直接访问单页地址，就会出现404错误。

* 详细的服务器配置请参考

  [官方文档]: https://router.vuejs.org/zh/guide/essentials/history-mode.html#caddy-v2	""在这""
