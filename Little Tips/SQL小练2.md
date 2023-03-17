> **前言：**
>
> 这是一个快失业的大学牲为了在简历上写熟练使用SQL而做的简单练习，越写感觉自己越菜。。。很多常见的语法根本不会，所以借由New Bing进行学习记录。主要采用记录我用的很少/没用过的关键词为主，同时记录了我遇到的不错的SQL题。因为篇幅不宜过长，所以SQL小练不会超过10个一文。
>
> 改文记录如下：
>
> （1）EXPLOD && POSEXPLOD
>
> （2）
>
> （3）
>
> （4）
>
> （5）
>
> （6）
>
> （7）
>
> （8）
>
> （9）
>
> （10）



#### 1、SQL语法explod和posexplod的具体操作

SQL语法中的explode和posexplode都是表值生成器函数，它们可以将一个数组或映射类型的列拆分成多行。explode函数只返回数组的元素或映射的键和值，而posexplode函数还返回数组或映射中元素的位置。explode和posexplode函数通常和lateral view子句一起使用，以便在查询中引用拆分后的列。

举两个例子：

1. 使用explode函数将一个包含多个单词的字符串拆分成多行。

```sql
select word
from sentence
lateral view explode(split(sentence, ' ')) t as word;
```

2. 使用posexplode函数将一个包含多个数字的数组拆分成多行，并显示每个数字在数组中的位置。

```sql
select pos, num
from numbers
lateral view posexplode(array) t as pos, num;
```

(1) posexplode 表值生成器函数 - Azure Databricks - Databricks SQL. https://learn.microsoft.com/zh-cn/azure/databricks/sql/language-manual/functions/posexplode

(2) Hive--sql中的explode()函数和posexplode()函数_dzysunshine的博客-CSDN博客. https://blog.csdn.net/dzysunshine/article/details/101110467

(3) Hive中的explode使用全解 - 知乎. https://zhuanlan.zhihu.com/p/115918587

------



#### 课后小练

###### （1）产生连续数

```sql
输出结果如下所示：
1
2
3
4
5
...
100
参考答案：
不 借 助 其 他 任 何 外 表 ， 实 现 产 生 连 续 数 值 ！

此处给出两种解法，其一：
select id_start+pos as id
from(
     select 1 as id_start,
            1000000 as id_end
) m lateral view posexplode(split(space(id_end-id_start), ' ')) t as pos, val

SQL解释：
1. 在子查询中，创建一个临时表m，包含两个常量列id_start和id_end，分别赋值为1和1000000。
2. 在主查询中，使用lateral view和posexplode函数，将m表中的id_end-id_start（即999999）转换为一个包含999999个空格的字符串，并将其拆分成多行，每行包含一个空格字符和它在字符串中的位置（从0开始）。
3. 在主查询中，使用select语句，将m表中的id_start（即1）加上t表中的pos（即空格字符的位置），得到一个从1到1000000的连续整数序列，并将其命名为id。

其二：
select row_number() over() as id
from(
    select split(space(99), ' ') as x) t
lateral view explode(x) ex;

SQL解释：
1. 在子查询中，创建一个临时表t，包含一个数组列x，该列的值为一个包含99个空格的字符串，使用split函数将其按照空格分隔成一个数组。
2. 在主查询中，使用lateral view和explode函数，将t表中的x列拆分成多行，每行包含一个空格字符，并命名为ex.col。
3. 在主查询中，使用select语句和row_number函数，为每一行生成一个自增的序号，并将其命名为id。由于每一行都是一个空格字符，所以序号就相当于从1到100的连续整数序列。
```

###### （2）数据展开

```sql
问题描述：如何将字符串"1-5,16,11-13,9"扩展成
"1,2,3,4,5,16,11,12,13,9"？注意顺序不变。


参考答案：
select concat_ws(',',collect_list(cast(rn as string)))
from(
    select a.rn, b.num, b.pos
    from(
        select row_number() over() as rn
        from (select split(space(20), ' ') as x) t -- space(20)可灵活调整
        lateral view explode(x) pe
) a lateral view outer
posexplode(split('1-5,16,11-13,9', ',')) b as pos, num
where a.rn between cast(split(num, '-')[0] as int) and cast(split(num, '-')[1] a
s int) or a.rn = num
order by pos, rn
) t;

SQL解释：
1. 在子查询中，创建一个临时表a，包含一个列rn，该列的值为从1到21（由space(20)决定）的连续整数序列，使用row_number函数和lateral view explode函数实现。
2. 在子查询中，创建一个临时表b，包含两个列pos和num，分别表示给定字符串（'1-5,16,11-13,9'）中每个子串（以逗号分隔）在原字符串中的位置（从0开始）和值。
3. 在子查询中，使用lateral view outer posexplode函数将b表中的num列拆分成多行，并保留原来的pos列。如果num是一个单个数字（如16或9），则拆分后只有一行；如果num是一个范围（如1-5或11-13），则拆分后有多行，并且每行都是范围内的一个整数。
4. 在子查询中，使用join语句将a表和b表连接起来，并使用where条件过滤出符合范围要求的rn值。例如，当num为1-5时，只保留rn为1到5之间的行；当num为16时，只保留rn为16的行。同时使用order by语句按照pos和rn排序结果集。
5. 在主查询中，使用select语句和concat_ws函数将结果集中的rn列转换为字符串，并用逗号连接起来。同时使用collect_list函数将所有行合并成一行。最终得到一个包含给定字符串指定范围内的整数的逗号分隔的字符串。
```

