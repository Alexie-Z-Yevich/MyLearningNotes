> **前言：**
>
> 这是一个快失业的大学牲为了在简历上写熟练使用SQL而做的简单练习，越写感觉自己越菜。。。很多常见的语法根本不会，所以借由New Bing进行学习记录。主要采用记录我用的很少/没用过的关键词为主，同时记录了我遇到的不错的SQL题。因为篇幅不宜过长，所以SQL小练不会超过10个一文。
>
> 改文记录如下：
>
> （1）CASE
>
> （2）UNION
>
> （3）CONCAT && CONCAT_WS
>
> （4）COLLECT && COLLECT_SET 【Hive SQL】
>
> （5）CAST
>
> （6）ROW_NUMBER
>
> （7）ROUND()
>
> （8）LAG() && LEAD()
>
> （9）LATERAL VIEW
>
> （10）UDTF简述

#### 1、SQL语法case的具体操作

SQL语法case的具体操作是一种条件表达式，用于根据条件执行不同的操作。SQL语法case有两种形式：简单的case和搜索的case。

简单的case是将一个表达式与一组简单表达式进行比较以确定结果。其语法如下：

```sql
CASE 表达式
  WHEN 值1 THEN 结果1
  WHEN 值2 THEN 结果2
  ...
  ELSE 结果N
END
```

搜索的case是计算一组布尔表达式以确定结果¹²。其语法如下：

```sql
CASE 
  WHEN 条件1 THEN 结果1
  WHEN 条件2 THEN 结果2
  ...
  ELSE 结果N
END
```

以下是两个例子：

- 在OrderDetails表中，根据Quantity的大小返回不同的文本¹

```sql
SELECT OrderID, Quantity,
CASE 
  WHEN Quantity > 30 THEN 'The quantity is greater than 30'
  WHEN Quantity = 30 THEN 'The quantity is 30'
  ELSE 'The quantity is under 30'
END AS QuantityText 
FROM OrderDetails;
```

- 在Customers表中，按照City排序，如果City为空，则按照Country排序¹

```sql
SELECT CustomerName, City, Country 
FROM Customers 
ORDER BY (CASE 
            WHEN City IS NULL THEN Country 
            ELSE City 
          END);
```

(1) SQL CASE Expression - W3Schools. https://www.w3schools.com/sql/sql_case.asp 

(2) SQL CASE 语句. https://www.w3schools.cn/sql/sql_case.asp 

(3) CASE statement in SQL - SQL Shack. https://www.sqlshack.com/case-statement-in-sql/ 

(4) SQL CASE - 1Keydata SQL 語法教學. https://www.1keydata.com/tw/sql/sql-case.html 

------



#### 2、SQL语法UNION的具体操作

SQL语法Union的具体操作是一种集合运算符，用于合并两个或多个SELECT语句的结果集。SQL语法Union有两种形式：Union和Union All。

Union是将两个或多个SELECT语句的结果集合并起来，并去除重复的记录。其语法如下：

```sql
SELECT 列名 FROM 表名1
UNION
SELECT 列名 FROM 表名2
```

请注意，Union内部的每个SELECT语句必须拥有相同数量的列，列也必须拥有相似的数据类型，同时，每条SELECT语句中的列的顺序必须相同。

Union All是将两个或多个SELECT语句的结果集合并起来，并保留重复的记录¹²。其语法如下：

```sql
SELECT 列名 FROM 表名1
UNION ALL
SELECT 列名 FROM 表名2
```

以下是两个例子：

- 在Websites表和apps表中，选取所有不同的country

```sql
SELECT country FROM Websites 
UNION 
SELECT country FROM apps 
ORDER BY country;
```

- 在Websites表和apps表中，选取所有中国(CN)的数据，并保留重复值

```sql
SELECT country, name FROM Websites 
WHERE country='CN' 
UNION ALL 
SELECT country, app_name FROM apps 
WHERE country='CN' 
ORDER BY country;
```

(1) SQL UNION 操作符 | 菜鸟教程. https://www.runoob.com/sql/sql-union.html

(2) SQL UNION 和 UNION ALL 操作符. https://www.w3school.com.cn/sql/sql_union.asp

(3) SQL UNION - 1Keydata SQL 語法教學. https://www.1keydata.com/tw/sql/sqlunion.html

------



#### 3、SQL语法concat和concat_ws的区别和具体操作

SQL语法concat和concat_ws的区别和具体操作是：

- concat函数是将多个字符串连接成一个字符串，如果有任何一个参数为null，则返回值为null。其语法如下：

```sql
CONCAT(string1, string2, ..., stringN)
```

- concat_ws函数是一种特殊形式的concat函数，也是将多个字符串连接成一个字符串，但是可以一次性指定分隔符（concat_ws就是concat with separator）。其语法如下：

```sql
CONCAT_WS(separator, string1, string2, ..., stringN)
```

请注意，concat_ws函数的第一个参数是其它参数间的分隔符，不能为null，如果为null，则返回值为null。

以下是两个例子：

- 在employee_tbl表中，选取id和name列，并用空格连接起来，使用concat函数

```sql
SELECT CONCAT(id,' ',name) AS id_name FROM employee_tbl;
```

- 在employee_tbl表中，选取id和name列，并用空格连接起来，使用concat_ws函数

```sql
SELECT CONCAT_WS(' ',id,name) AS id_name FROM employee_tbl;
```

(1) SQL里的concat() 以及concat_ws() 函数的使用_concat_ws函数_一个学习Java的小菜鸟的博客-CSDN博客. https://blog.csdn.net/weixin_48633655/article/details/106838028 

(2) concat和 concat_ws 区别以及踩出来的坑骑白马de图图的博客-CSDN博客. https://blog.csdn.net/liusuoyin/article/details/108110718

(3) MySQL中concat()、concat_ws()、group_concat()函数使用技巧与心得 - 知乎. https://zhuanlan.zhihu.com/p/257399676

------



#### 4、Hive SQL语法collect和collect_set的区别和具体操作

SQL语法collect和collect_set的区别和具体操作是：

- collect函数是一个Hive函数，用于将分组中的某列转为一个数组返回。其语法如下：

```sql
COLLECT(column)
```

- collect_set函数也是一个Hive函数，用于将分组中的某列转为一个数组返回，但是会去除重复的值。其语法如下：

```sql
COLLECT_SET(column)
```

请注意，collect函数和collect_set函数一般搭配group by使用，并且可以和concat_ws函数连用，以指定分隔符。

以下是两个例子：

- 在student表中，按照class分组，将每个班级的name列转为一个数组，并用逗号连接起来，使用collect函数

```sql
SELECT class, CONCAT_WS(',', COLLECT(name)) AS names FROM student GROUP BY class;
```

- 在student表中，按照class分组，将每个班级的name列转为一个数组，并用逗号连接起来，使用collect_set函数

```sql
SELECT class, CONCAT_WS(',', COLLECT_SET(name)) AS names FROM student GROUP BY class;
```

(1) 数仓工具—Hive语法之collect_set 和 collect_list(17)浊酒南街的博客-CSDN博客. https://blog.csdn.net/weixin_43597208/article/details/117450304

(2) SQL小知识点系列-3-collect_list/collect_set（列转行） - 知乎. https://zhuanlan.zhihu.com/p/440574505

(3) Spark SQL里concat_ws和collect_set的作用 - 大葱拌豆腐 - 博客园. https://www.cnblogs.com/itboys/p/11217815.html

------



#### 5、SQL语法cast的具体操作

SQL语法cast的具体操作是：

- cast函数用于将某种数据类型的表达式显式转换为另一种数据类型。其语法如下：

```sql
CAST(expression AS data_type)
```

请注意，expression可以是任何有效的SQL表达式，data_type可以是以下类型之一：BINARY，CHAR，DATE，DATETIME，TIME，DECIMAL，SIGNED，UNSIGNED。

以下是两个例子：

- 将文本字符串'12'转换为整型

```sql
SELECT CAST('12' AS INT);
```

- 将日期时间2021-01-01 12:34:56转换为日期

```sql
SELECT CAST('2021-01-01 12:34:56' AS DATE);
```

(1) SQL之CAST函数用法-百度经验. https://jingyan.baidu.com/article/90808022fab5dbfd91c80f35.html

(2) SQL CAST() - SQL. https://sql.sh/fonctions/cast

(3) SQL Server CAST() Function - W3Schools. https://www.w3schools.com/sql/func_sqlserver_cast.asp

(4) SQL-cast()函数 - 知乎. https://zhuanlan.zhihu.com/p/343703794

(5) MySQL cast()函数 - MySQL教程. https://www.yiibai.com/mysql/cast.html

(6) sql CAST用法 - 水狼一族 - 博客园. https://www.cnblogs.com/shuilangyizu/p/5952526.html

------



#### 6、SQL语法row_number()的具体操作

row_number()是一个窗口函数，它为查询结果集中的每一行分配一个连续的整数。它的语法如下：

```sql
row_number() over (partition by column order by column)
```

其中，partition by子句用于指定分组依据，order by子句用于指定排序依据。如果没有partition by子句，则对整个结果集进行编号；如果没有order by子句，则按照任意顺序进行编号。

以下是两个例子：

- 例1：从employee表中选择name、salary、department三列，并按照department分组，在每个分组内按照salary降序排序，给每一行添加一个序号rank。

```sql
select name, salary, department, row_number() over (partition by department order by salary desc) as rank
from employee;
```

这样就会得到类似如下的表格：

| name  | salary | department | rank |
| ----- | ------ | ---------- | ---- |
| Alice | 8000   | IT         | 1    |
| Bob   | 7000   | IT         | 2    |
| Cathy | 6000   | IT         | 3    |
| David | 9000   | HR         | 1    |
| Eva   | 8000   | HR         | 2    |
| Frank | 7000   | HR         | 3    |

- 例2：从student表中选择name、score、class三列，并按照class分组，在每个分组内按照score降序排序，给每一行添加一个序号rank。

```sql
select name, score, class, row_number() over (partition by class order by score desc) as rank
from student;
```

这样就会得到类似如下的表格：

| name | score | class | rank |
| ---- | ----- | ----- | ---- |
| Amy  | 95    | A     | 1    |
| Ben  | 90    | A     | 2    |
| Carl   |85   |  A |    	3|
|Dan|	80	|A	|4|
|Eve|	75	|A	|5|
|Fay|	92	|B	|1|
|Glen|	88	|B	|2|
|Hank|	84	|B	|3|
|Iris|	80|	B	|4|
|Jack|	76	|B	|5|

(1) SQL ROW_NUMBER() Function - SQL Tutorial. https://www.sqltutorial.org/sql-window-functions/sql-row_number/

(2) SQL Server Row_number()函数 - SQL Server教程. https://www.yiibai.com/sqlserver/sql-server-row_number-function.html

(3) 【SQL】連番を振るROW_NUMBER関数を解説！一番よく使う順位付け関数をマスターしよう | ポテパンスタイル. https://style.potepan.com/articles/23566.html

(4) ROW_NUMBER (Transact-SQL) - SQL Server | Microsoft Learn. https://learn.microsoft.com/en-us/sql/t-sql/functions/row-number-transact-sql?view=sql-server-ver16

(5) SQL中row_number函数用法_sql的rownumber_sharon@zhang的博客-CSDN博客. https://blog.csdn.net/baidu_41797613/article/details/120489521

------



#### 7、SQL语法round()的具体操作

SQL round()函数用于把数值字段舍入为指定的小数位数。它的语法是：

```sql
round(number, decimals [, operation])
```

其中，number是要舍入的数值，decimals是要保留的小数位数，operation是可选的参数，表示是否进行截断而不是四舍五入。如果operation为0或省略，则进行四舍五入；如果operation为非0，则进行截断。

举两个例子：

- 如果要把3.14159舍入为两位小数，可以使用：

```sql
select round(3.14159, 2);
```

结果是3.14。

- 如果要把1234.5678截断为个位数，可以使用：

```sql
select round(1234.5678, 0, 1);
```

结果是1230。

(1) SQL ROUND() 函数. https://www.w3school.com.cn/sql/sql_func_round.asp 

(2) SQL Server ROUND() Function - W3Schools. https://www.w3schools.com/SQL/func_sqlserver_round.asp 

(3) SQL Server ROUND() Function - TutorialsTeacher. https://www.tutorialsteacher.com/sqlserver/round-function 

(4) How to Use the ROUND() Function in SQL | LearnSQL.com. https://learnsql.com/blog/round-function-sql/ 

(5) SQL ROUND() 函數 / Function - SQL 語法教學 Tutorial. https://www.fooish.com/sql/round-function.html

(6) ROUND (Transact-SQL) - SQL Server | Microsoft Learn. https://learn.microsoft.com/en-us/sql/t-sql/functions/round-transact-sql?view=sql-server-ver16

------



#### 8、SQL语法lag()和lead()的具体操作

SQL语法中的lag()和lead()函数是两种窗口分析函数，可以在同一次查询中取出同一字段的前N行的数据(Lag)和后N行的数据(Lead)作为独立的列。这两个函数有三个参数，第一个参数是列名，第二个参数是偏移的行数，第三个参数是超出记录窗口时的默认值。

举两个例子：

1. 查询每个员工的姓名、部门、工资以及比他工资高的下一个员工的姓名和工资。

```sql
select name, dept, salary,
lead(name) over (partition by dept order by salary desc) as next_name,
lead(salary) over (partition by dept order by salary desc) as next_salary
from employee;
```

2. 查询每个商品的名称、价格以及与上一个商品价格的差异。

```sql
select product, price,
price - lag(price) over (order by price asc) as diff
from goods;
```

(1) 关于SQL的LAG()和LEAD()函数_lag sql_一碗烫烫的汤圆的博客-CSDN博客. https://blog.csdn.net/a5276957/article/details/103808997

(2) Sql窗口分析函数【lead、lag详解】 - 知乎. https://zhuanlan.zhihu.com/p/466752301

(3) lag lead分析函数 - 知乎. https://zhuanlan.zhihu.com/p/347444265

(4) SQL中lag()和lead()函数使用_lag函数_Schafferyy的博客-CSDN博客. https://blog.csdn.net/Schaffer_W/article/details/109029630

------



#### 9、SQL语法lateral view的具体操作

SQL语法中的lateral view子句是用于和split、explode等UDTF（用户自定义表生成函数）一起使用的，它能够将一行数据拆成多行数据，在此基础上可以对拆分后的数据进行聚合。lateral view首先为原始表的每行调用UDTF，UTDF会把一行拆分成一或者多行，lateral view再把结果组合，产生一个支持别名表的虚拟表。

举两个例子：

1. 查询每个学生的姓名、年龄以及选修的课程名称。

```sql
select name, age, course
from student
lateral view explode(split(courses, ',')) t as course;
```

2. 查询每个订单的编号、金额以及包含的商品名称和数量。

```sql
select order_id, amount, product, quantity
from order
lateral view explode(items) t as item
lateral view explode(map_keys(item)) k as product
lateral view explode(map_values(item)) v as quantity;
```

(1) SQL 之 lateral view explode()_hankl1990的博客-CSDN博客. https://blog.csdn.net/weixin_36630761/article/details/76146842

(2) Lateral View语法 - 简书. https://www.jianshu.com/p/4804db6a3677

(3) LATERAL VIEW 子句 - Azure Databricks - Databricks SQL. https://learn.microsoft.com/zh-cn/azure/databricks/sql/language-manual/sql-ref-syntax-qry-select-lateral-view

(4) SQL: lateral view & explode & split (分割&侧视图) - 知乎. https://zhuanlan.zhihu.com/p/538641683

(5) LATERAL VIEW clause - Azure Databricks - Databricks SQL. https://learn.microsoft.com/en-us/azure/databricks/sql/language-manual/sql-ref-syntax-qry-select-lateral-view

------



#### 10、SQL UDTF 常见操作

SQL UDTF（用户自定义表值函数）是一种可以返回多行数据的函数，它可以接受0个、1个或多个标量值作为输入参数。UDTF可以在查询的FROM子句中使用，也可以和lateral view子句结合使用。

举两个例子：

1. 创建一个UDTF，用于将一个字符串按照指定的分隔符拆分成多行。

```sql
create function split_string(str string, sep string)
returns table (word string)
as
return select value as word from unnest(string_to_array(str, sep));
```

2. 创建一个UDTF，用于将一个日期范围转换成每一天的日期。

```sql
create function date_range(start_date date, end_date date)
returns table (day date)
as
return select generate_series(start_date, end_date, interval '1 day')::date as day;
```

(1) Flink自定义表值函数UDTF开发、注册和使用流程_实时计算 Flink版-阿里云帮助中心. https://help.aliyun.com/document_detail/188055.html

(2) Tabular SQL UDFs (UDTFs) | Snowflake Documentation. https://docs.snowflake.com/en/developer-guide/udf/sql/udf-sql-tabular-functions

(3) mySql的UDF是什么 - 洪福必成 - 博客园. https://www.cnblogs.com/ghc666/p/8609067.html

(4) Udf in SQL Server: Create UDF (User Defined Function) in SQL Database. https://www.webtrainingroom.com/sql/udf

(5) User-Defined Functions - SQL Server | Microsoft Learn. https://learn.microsoft.com/en-us/sql/relational-databases/user-defined-functions/user-defined-functions?view=sql-server-ver16

------



#### 课后小练

###### （1）常见SQL——行列转换

```sql
描述：表中记录了各年份各部门的平均绩效考核成绩。 
表名：t1 
表结构： a -- 年份 
        b -- 部门 
        c -- 绩效得分 
表内容： a  b  c 
      2014 B  9 
      2015 A  8 
      2014 A  10 
      2015 B  7 
问题一：多行转多列 
问题描述：将上述表内容转为如下输出结果所示： 
     a col_A col_B 
   2014   10   9 
   2015   8    7 
   
   
参考答案： 
select a, 
       max(case when b="A" then c end) col_A, 
       max(case when b="B" then c end) col_B
from t1 
group by a;
```



###### （2）多行转多列（背景和第一题相同）

```sql
问题描述：2014 年公司组织架构调整，导致部门出现多个绩效，业务及人员不同，
无法合并算绩效，源表内容如下：
    2014 B 9
    2015 A 8
    2014 A 10
    2015 B 7
    2014 B 6
输出结果如下所示：
    a col_A col_B
   2014 10  6,9
   2015  8   7
   
   
参考答案:
select a,
       max(case when b="A" then c end) col_A,
       max(case when b="B" then c end) col_B
from (
       select a, b,
              concat_ws(",",collect_set(cast(c as string))) as c       # 将相同部门数字转换为字符串后进行拼接
       from t1
       group by a,b
)tmp
group by a;
```



###### （3）按a分组取b字段排最小时对应的c字段

```sql
表名：t2
表字段及内容：
    a   b  c
  2014  A  3
  2014  B  1
  2014  C  2
  2015  A  4
  2015  D  3
问题一：按 a 分组取 b 字段最小时对应的 c 字段
输出结果如下所示：
   a   min_c
  2014   3
  2015   4
  
  
参考答案:
select a, c as min_c
from
(
       select a, b, c,
              row_number() over(partition by a order by b) as rn
       from t2
) a
where rn = 1;

SQL解释：
从t2表中选择a、b、c三列，按照a列进行分组，然后在每个分组内按照b列进行排序，给每一行添加一个序号rn。然后从这个结果中选择a、c两列，并将c列重命名为min_c。最后只保留rn等于1的行，也就是每个分组内b列最小的那一行。
```



###### （4）按a分组按b字段排序，对b取累计排名比例

```sql
表名：t3
表字段及内容：
    a   b   c
  2014  A   3
  2014  B   1
  2014  C   2
  2015  A   4
  2015  D   7
输出结果如下所示：
    a  b  ratio_c
  2014 A   0.33
  2014 B   0.67
  2014 C   1.00
  2015 A   0.50
  2015 D   1.00


参考答案：
select a, b,
       round(row_number() over(partition by a order by b) 
             / (count(c) over(partition by a)),2) as ratio_c
from t3
order by a,b

SQL解释：
从t3表中选择a、b两列，并计算一个新的列ratio_c，它表示每个a分组内的行号与该分组内c列的总数的比值，保留两位小数。然后按照a、b两列进行排序。
```



###### （5）按a分组按b字段排序，对c取平均值

```sql
表名：t4
表字段及内容：
    a   b   c
   2014 A   3
   2014 B   1
   2014 C   2
   2015 A   4
   2015 D   3
问题描述：前一行与当前行的均值！
输出结果如下所示：
    a   b avg_c
   2014 A   3
   2014 B   2
   2014 C  1.5
   2015 A   4
   2015 D  3.5
   
   
参考答案：
select a, b,
       case when lag_c is null then c
       else (c+lag_c)/2 end as avg_c
from
(
       select a, b, c,
              lag(c,1) over(partition by a order by b) as lag_c
       from t4
)temp;
```

