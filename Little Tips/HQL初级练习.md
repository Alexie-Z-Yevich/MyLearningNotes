# HQL初级练习

> using() 使用的子列不能作为 group by 对象—— mysql 可以，hive 不行
>
> mysql 中没有对 string 的操作，所以遇到 date_format() 等操作的时候，需要先使用 str_to_date() 将字符串转换为时间，否则无法被 ‘yyyy-MM-dd’ 识别

## 基础查询训练

###### 1. 查询姓名中带“山”的学生名单

```sql
select * from student_info where stu_name like "%山%";
```



###### 2. 查询姓“王”老师的个数

```sql
select count(*) from teacher_info where tea_name like '王%';
```



###### 3. 检索课程编号为“04”且分数小于60的学生的分数信息，结果按分数降序排列

```sql
select * from score_info where course_id='04' and score < 60 order by score desc ;
```



###### 4. 查询数学成绩不及格的学生信息和其对应的数学学科成绩，按照学号升序排序

```sql
select
    s.stu_id,
    s.stu_name,
    t1.score
from student_info s
join (
    select
        *
    from score_info
    where course_id=(select course_id from course_info where course_name='数学') and score < 60
    ) t1 on s.stu_id = t1.stu_id
order by s.stu_id;
```



###### 5. 查询编号为“02”的课程的总成绩

```sql
select sum(score) from score_info where course_id = '02';
```



###### 6. 查询参加考试的学生个数

```sql
select count(distinct stu_id) from score_info;
```



###### 7. 查询各科成绩最高和最低的分，以如下的形式显示：课程号、最高分、最低分

```sql
select course_id, max(score) as maximum, min(score) as minimum from score_info group by course_id;
```



###### 8. 查询每门课程有多少学生参加了考试（有考试成绩）

```sql
select course_id, count(distinct stu_id) from score_info group by course_id;
```



######  9. 查询男生、女生人数

```sql
select sex, count(sex) from student_info group by sex;
```



######  10. 查询平均成绩大于60分的学生的学号和平均成绩

```sql
select stu_id, avg(score) from score_info group by stu_id having avg(score) > 60;
```



######  11. 查询至少考了四门课程的学生学号

```sql
select stu_id from score_info group by stu_id having count(score);
```



######  12. 查询每门课程的平均成绩，结果按平均成绩升序排序，平均成绩相同时，按课程号降序排列

```sql
select course_id, avg(score) from score_info group by course_id order by avg(score) asc, course_id desc;
```



######  13. 统计参加考试人数大于等于15的学科

```sql
select course_id from score_info group by course_id having count(distinct stu_id) >= 15;
```



######  14. 查询学生的总成绩并按照总成绩降序排序

```sql
select stu_id, sum(score) from score_info group by stu_id order by sum(score) desc;
```



######  15. 查询一共参加三门课程且其中一门为语文课程的学生的id和姓名

```sql
select stu_id, stu_name from student_info left join score_info USING (stu_id)
where stu_id in
(select stu_id from course_info LEFT JOIN score_info using(course_id) where course_name = "语文")
GROUP BY stu_id HAVING count(*)  = 3;
```



######  16. 查询没有学全所有课的学生的学号、姓名

```sql
select stu_id, stu_name from student_info where stu_id in
(select stu_id from score_info group by stu_id having count(*) < (select count(*) from course_info));
```



######  17. 查询出只选修了三门课程的全部学生的学号和姓名

```sql
select stu_id, stu_name from student_info where stu_id in
(select stu_id from score_info group by stu_id having count(*) = 3);
```



######  18. 查询所有学生的学号、姓名、选课数、总成绩

```sql
select stu_id, stu_name, count(score), sum(score) from student_info left join score_info using (stu_id) group by stu_id;
```



######  19. 查询平均成绩大于85的所有学生的学号、姓名和平均成绩

```sql
select stu_id, stu_name, avg(score) from student_info left join score_info using (stu_id) group by stu_id having avg(score) > 85;
```



######  20. 查询学生的选课情况：学号，姓名，课程号，课程名称

```sql
select stu_id, stu_name, course_id, course_name from student_info left join score_info using (stu_id) left join course_info using (course_id);
```



######  21. 课程编号为"01"且课程分数小于60，按分数降序排列的学生信息

```sql
select stu_id, stu_name from student_info left join score_info using (stu_id) where course_id = '01' and score < 60;
```



######  22. 查询所有课程成绩在70分以上的学生的姓名、课程名称和分数，按分数升序排列

```sql
select stu_name, course_name, score from score_info left join student_info using (stu_id) left join course_info using (course_id) where stu_id in
    (select stu_id from score_info where stu_id not in (select stu_id from score_info where score <= 70));
```



######  23. 查询该学生不同课程的成绩相同的学生编号、课程编号、学生成绩

```sql
select si1.stu_id, si1.course_id, si1.score from score_info si1 join score_info si2 on si1.stu_id = si2.stu_id and si1.score = si2.score and si1.course_id <> si2.course_id;
```



######  24. 查询课程编号为“01”的课程比“02”的课程成绩高的所有学生的学号

```sql
select s1.stu_id from score_info s1 join score_info s2 on s1.stu_id = s2.stu_id and s1.score > s2.score where s1.course_id = '01' and s2.course_id = 02;
```



######  25. 查询学过编号为“01”的课程并且也学过编号为“02”的课程的学生的学号、姓名

```sql
select stu_id, stu_name from student_info where stu_id in
(select s1.stu_id from score_info s1 join score_info s2 on s1.stu_id = s2.stu_id where s1.course_id = '01' and s2.course_id = 02);
```



######  26. 查询学过“李体音”老师所教的所有课的同学的学号、姓名

```sql
SELECT
   stu_id,
   stu_name
FROM
   student_info
WHERE
   stu_id IN (
   SELECT
      stu_id
   FROM
      (
      SELECT
         stu_id,
         count( course_id ) AS ct
      FROM
         score_info
      WHERE
         course_id IN ( SELECT course_id FROM course_info LEFT JOIN teacher_info USING ( tea_id ) WHERE tea_name = "李体音" )
      GROUP BY
         stu_id
      ) t1
   JOIN ( SELECT count( course_id ) AS ct FROM course_info LEFT JOIN teacher_info USING ( tea_id ) WHERE tea_name = "李体音" ) t2 ON t1.ct = t2.ct
   );
```



######  27. 查询学过“李体音”老师所讲授的任意一门课程的学生的学号、姓名

```sql
select stu_id, stu_name from student_info where stu_id in
(select stu_id from score_info left join course_info using (course_id) left join teacher_info using (tea_id) where tea_name = "李体音");
```



######   28. 查询没学过"李体音"老师讲授的任一门课程的学生姓名

```sql
select stu_name from student_info where stu_id not in
(select stu_id from score_info left join course_info using (course_id) left join teacher_info using (tea_id) where tea_name = "李体音");
```



######  29. 查询至少有一门课与学号为“001”的学生所学课程相同的学生的学号和姓名

```sql
select stu_id, stu_name from student_info where stu_id in
(select stu_id from score_info where course_id in (select course_id from score_info where score_info.stu_id = '001'));
```



######  30. 按平均成绩从高到低显示所有学生的所有课程的成绩以及平均成绩

```sql
select score, avg from score_info t1 left join
    (select stu_id, avg(score) as avg from score_info group by stu_id) t2
on t1.stu_id = t2.stu_id order by avg desc;
```



## 初级查询训练

######  1. 从订单明细表（order_detail）中筛选出2021年总销量小于100的商品及其销量，假设今天的日期是2022-01-10，不考虑上架时间小于一个月的商品

```sql
SELECT sku_info.sku_id, sku_info.name, SUM(order_detail.sku_num) AS order_num
FROM order_detail
LEFT JOIN sku_info ON order_detail.sku_id = sku_info.sku_id
WHERE year(order_detail.create_date) = 2021
AND datediff(order_detail.create_date, "2021-12-10") <= 0
GROUP BY sku_info.sku_id, sku_info.name
HAVING order_num < 100;
```



######  2. 从用户登录明细表（user_login_detail）中查询每天的新增用户数。若一个用户在某天登录了，且在这一天之前没登录过，则认为该用户为这一天的新增用户。

```sql
select login_date_first, count(*) as user_count from (
 select user_id, min(date_format(login_ts, 'yyyy-MM-dd')) as login_date_first from user_login_detail group by user_id ) t1
                                               group by login_date_first;
```



######  3. 从用户登录明细表（user_login_detail）和订单信息表（order_info）中查询每个用户的注册日期（首次登录日期）、总登录次数，以及2021年的登录次数、订单数和订单总额。

```sql
select t1.user_id, register_date, total_login_count, login_count_2021, nvl(order_count_2021, 0) as order_count_2021,
       cast (nvl( order_amount_2021, 0.00) as decimal(16, 2)) as order_amount_2021 from (
    select user_id, min(date_format(login_ts, 'yyyy-MM-dd')) as register_date,
           count(login_ts) as total_login_count, count(year(login_ts) = '2021') as login_count_2021
    from user_login_detail group by user_id ) t1
    left join (
        select user_id, count(*) as order_count_2021, sum(total_amount) as order_amount_2021
        from order_info where year(create_date) = '2021' group by user_id
) t2 on t1.user_id = t2.user_id order by user_id;
```



######  4. 请向所有用户推荐其朋友收藏但是自己未收藏的商品，从好友关系表（friendship_info）和收藏表（favor_info）中查询出应向哪位用户推荐哪些商品。

```sql
select user_id, sku_id from (select distinct t1.user1_id as user_id, sku_id
                             from friendship_info t1
                                      left join favor_info t2 on t1.user2_id = t2.user_id where sku_id is not null) t1
where user_id except (
    select distinct t1.user1_id as user_id, sku_id
                             from friendship_info t1
                                      left join favor_info t2 on t1.user1_id = t2.user_id
    ) order by user_id;
```



######  5. 从订单信息表（order_info）和用户信息表（user_info）中，分别统计每天男性和女性用户的订单总金额，如果当天男性或者女性没有购物，则统计结果为0。

```sql
select create_date, cast(sum(male) as decimal(16, 2)) as total_amount_male, cast(sum(female) as decimal(16, 2)) as total_amount_female from (
select create_date, if(gender = '男', total_amount, 0) male, if(gender = '女', total_amount, 0) female from order_info t1 left join user_info t2
on t1.user_id = t2.user_id ) t1 group by create_date;
```



######  6. 从订单明细表（order_detail）中查询出所有购买过商品1和商品2，但是没有购买过商品3的用户。

```sql
select user_id from (select t3.user_id, collect_set(t1.sku_id) as sku
                     from order_detail t1
                              left join order_info t2 on t1.order_id = t2.order_id
                              left join user_info t3 on t2.user_id = t3.user_id
                     group by t3.user_id) t1
where array_contains(sku, '1') = true and array_contains(sku, '2') = true and array_contains(sku, '3') = false;
```



######  7. 从订单明细表（order_detail）中统计每天商品1和商品2销量（件数）的差值（商品1销量-商品2销量）

```sql
select create_date,  sum(a) - sum(b) as diff from (select create_date,
                                                if(sku_id = '1', sku_num, 0) a,
                                                if(sku_id = '2', sku_num, 0) b from order_detail) t1
group by create_date;
```



######  8. 通过订单详情表（order_detail）的数据，根据销售件数对商品进行分类，销售件数0-5000为冷门商品，5001-19999为一般商品，20000以上为热门商品，统计不同类别商品的数量

```sql
select category, count(*) as cn from (
select case when sum(sku_num) >= 0 and sum(sku_num) <= 5000 then '冷门商品'
    when sum(sku_num) >= 5001 and sum(sku_num) <= 19999 then '一般商品'
    when sum(sku_num) >= 20000 then '热门商品' end category from order_detail group by sku_id
      ) t1 group by category;
```



######  9. 从用户登录明细表（user_login_detail）中统计有新增用户的日期的新增用户数（若某日未新增用户，则不出现在统计结果中），并统计这些新增用户的一日留存率。用户首次登录为当天新增，次日也登录则为一日留存。一日留存用户占新增用户数的比率为一日留存率。

```sql
select regist_date, count(*) regist_count, cast(sum(rate) / count(*) as decimal(16, 2)) retention_1_rate from
(select min(date_format(login_ts, 'yyyy-MM-dd')) as regist_date,
           if(array_contains(
              collect_set(date_format(login_ts, 'yyyy-MM-dd')),
              cast(date_add(min(date_format(login_ts, 'yyyy-MM-dd')), 1) as string)),
                   1, 0) as rate
        from user_login_detail
group by user_id) t1 group by regist_date;
```



######  10. 分别从登录明细表（user_login_detail）和配送信息表（delivery_info）中的用户登录时间和下单时间字段，统计登陆次数和交易次数

```sql
select t1.user_id, t1.login_date, nvl(login_count, 0) as login_count, nvl(order_count, 0) as order_count from (
select user_id, date_format(login_ts, 'yyyy-MM-dd') login_date, count(*) login_count from user_login_detail
group by user_id, date_format(login_ts, 'yyyy-MM-dd')) t1
left join (
    select user_id, date_format(order_date, 'yyyy-MM-dd') login_date, count(*) order_count from delivery_info
group by user_id, date_format(order_date, 'yyyy-MM-dd')
) t2 on t1.user_id = t2.user_id and t1.login_date = t2.login_date;
```



######  11. 从订单明细表（order_detail）中统计每个商品各年度的销售总额

```sql
select sku_id, year(create_date) year_date, sum(sku_num) * price sku_sum from
order_detail group by sku_id, year_date order by sku_id;
```



######  12. 从订单详情表（order_detail）中查询2021年9月27号-2021年10月3号这一周所有商品每天销售件数

```sql
select sku_id, sum(a) as monday, sum(b) as tuesday, sum(c) as wednesday,
        sum(d) as thusday, sum(e) as friday, sum(f) as saturday, sum(g) as sunday from (
select sku_id,
       if (dayofweek(create_date) = 2, sku_num, 0) as a,
       if (dayofweek(create_date) = 3, sku_num, 0) as b,
       if (dayofweek(create_date) = 4, sku_num, 0) as c,
       if (dayofweek(create_date) = 5, sku_num, 0) as d,
       if (dayofweek(create_date) = 6, sku_num, 0) as e,
       if (dayofweek(create_date) = 7, sku_num, 0) as f,
       if (dayofweek(create_date) = 1, sku_num, 0) as g from
    order_detail where datediff(date_format(create_date, 'yyyy-MM-dd'), '2021-09-27') >= 0
    and datediff(date_format(create_date, 'yyyy-MM-dd'), '2021-10-03') <= 0) t
group by sku_id;
```



######  13. 从订单明细表（order_detail）中，统计同一个商品在2021年和2022年中同一个月的销量对比

```sql
select sku_id,
       month(create_date) month,
       sum(if(year(create_date) = 2020, sku_num, 0)) 2020_skusum,
       sum(if(year(create_date) = 2021, sku_num, 0)) 2021_skusum
from order_detail
where year(create_date) = 2021
   or year(create_date) = 2020
group by sku_id, month(create_date);
```



######  14. 从订单明细表（order_detail）和收藏信息表（favor_info）中统计2021年国庆节期间（10月1日-10月7日），每个商品的购买总数量和总收藏次数

```sql
select t1.sku_id, nvl(sku_sum, 0) as sku_sum, nvl(favor_cn, 0) as favor_cn from
    (select sku_id, sum(sku_num) as sku_sum from order_detail
                    where datediff(date_format(create_date, 'yyyy-MM-dd'), '2021-10-01') >= 0
                    and datediff(date_format(create_date, 'yyyy-MM-dd'), '2021-10-07') <= 0
                    group by sku_id) t1 full outer join (
              select sku_id, count(*) as favor_cn from favor_info
                    where datediff(date_format(create_date, 'yyyy-MM-dd'), '2021-10-01') >= 0
                    and datediff(date_format(create_date, 'yyyy-MM-dd'), '2021-10-07') <= 0
                    group by sku_id
    ) t2 on t1.sku_id = t2.sku_id;
```



######  15. 动销率的定义为某品类的商品中一段时间内有销量的商品种类数占当前已上架总商品种类数的比例（有销量的商品种类数/已上架总商品种类数）。滞销率的定义为某分类商品中一段时间内没有销量的商品种类数占当前已上架总商品种类数的比例（没有销量的商品种类数/已上架总商品种类数）。只要当天任一店铺有任何商品的销量就输出该天的统计结果。从订单明细表（order_detail）和商品信息表（sku_info）表中统计国庆节期间（10月1日-10月7日）每天每个分类的商品的动销率和滞销率

```sql
select category_id,
       cast(first_sale_rate as decimal(16, 2))         first_sale_rate,
       cast((1 - first_sale_rate) as decimal(16, 2))   first_unsale_rate,
       cast(second_sale_rate as decimal(16, 2))        second_sale_rate,
       cast((1 - second_sale_rate) as decimal(16, 2))  second_unsale_rate,
       cast(third_sale_rate as decimal(16, 2))         third_sale_rate,
       cast((1 - third_sale_rate) as decimal(16, 2))   third_unsale_rate,
       cast(fourth_sale_rate as decimal(16, 2))        fourth_sale_rate,
       cast((1 - fourth_sale_rate) as decimal(16, 2))  fourth_unsale_rate,
       cast(fifth_sale_rate as decimal(16, 2))         fifth_sale_rate,
       cast((1 - fifth_sale_rate) as decimal(16, 2))   fifth_unsale_rate,
       cast(sixth_sale_rate as decimal(16, 2))         sixth_sale_rate,
       cast((1 - sixth_sale_rate) as decimal(16, 2))   sixth_unsale_rate,
       cast(seventh_sale_rate as decimal(16, 2))       seventh_sale_rate,
       cast((1 - seventh_sale_rate) as decimal(16, 2)) seventh_unsale_rate
from (select category_id,
             cast(count(distinct if(t1.create_date = '2021-10-01', t1.sku_id, null)) as decimal(16, 2)) /
             count(distinct if(sku_info.from_date <= '2021-10-01', sku_info.sku_id, null)) first_sale_rate,
             cast(count(distinct if(t1.create_date = '2021-10-02', t1.sku_id, null)) as decimal(16, 2)) /
             count(distinct if(sku_info.from_date <= '2021-10-02', sku_info.sku_id, null)) second_sale_rate,
             cast(count(distinct if(t1.create_date = '2021-10-03', t1.sku_id, null)) as decimal(16, 2)) /
             count(distinct if(sku_info.from_date <= '2021-10-03', sku_info.sku_id, null)) third_sale_rate,
             cast(count(distinct if(t1.create_date = '2021-10-04', t1.sku_id, null)) as decimal(16, 2)) /
             count(distinct if(sku_info.from_date <= '2021-10-04', sku_info.sku_id, null)) fourth_sale_rate,
             cast(count(distinct if(t1.create_date = '2021-10-05', t1.sku_id, null)) as decimal(16, 2)) /
             count(distinct if(sku_info.from_date <= '2021-10-05', sku_info.sku_id, null)) fifth_sale_rate,
             cast(count(distinct if(t1.create_date = '2021-10-06', t1.sku_id, null)) as decimal(16, 2)) /
             count(distinct if(sku_info.from_date <= '2021-10-06', sku_info.sku_id, null)) sixth_sale_rate,
             cast(count(distinct if(t1.create_date = '2021-10-07', t1.sku_id, null)) as decimal(16, 2)) /
             count(distinct if(sku_info.from_date <= '2021-10-07', sku_info.sku_id, null)) seventh_sale_rate
      from (select sku_id,
                   create_date
            from order_detail
            where create_date >= '2021-10-01'
              and create_date <= '2021-10-07') t1
               right join sku_info
                          on t1.sku_id = sku_info.sku_id
      group by category_id) t2;
```









