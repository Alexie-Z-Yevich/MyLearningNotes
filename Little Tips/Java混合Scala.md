最近这几天把Scala学完了，被他的数据处理能力深深的折服。

尤其是WordCount功能，之前接过的一个小外包就有这种要求的分词功能，当时是使用的开源依赖Kumo词库，但是别人的东西怎么实现的说实话真不知道。

后来和同学探讨的过程中了解到Elastic Search是可以实现分词功能的，效果极佳但是资源占用大，对于一个小项目来说可能资源大部分回响ES倾斜，所以虽然有所耳闻但是并没有实际实践。

再后来学了Hadoop框架，了解到WordCount在上面的实现是拆分成Map和Reduce两个阶段，但是其过程主要面向框架中处理，虽然都是Java但是其实很多过程并不清晰（其实是没能力手撸一个）

然后学了Scala，刚好Scala最后也是介绍了WordCount功能，本质上Scala对WordCount的处理也是一个Map、Reduce的过程，但是代码简洁（可读性不强），流程也更清晰。于是想着Java和Scala都是基于JVM，那干脆Java中调用Scala，看能否实现。

###### Scala代码如下：

```scala
import java.util
import scala.collection.JavaConverters._  // 格式转换依赖包

class wc {
  def WordCount(stringList: util.List[String]): util.List[(String, Int)] = {
    val s = stringList.asScala.toList  // 格式转换Java.List --> Scala.List
    // 1) 将句子切分成单词
    val wordList: List[String] = s.flatMap(str => str.split(" "))
    // 2) 将相同的单词放置在一起
    val wordToWordsMap: Map[String, List[String]] = wordList.groupBy(word => word)
    // 3) 对相同的单词进行计数
    // (word, list) => (word, count)
    val wordToCountMap: Map[String, Int] = wordToWordsMap.map(tuple => (tuple._1, tuple._2.size))
    // 4) 对计数完成后的结果进行排序（降序）
    val sortList: List[(String, Int)] = wordToCountMap.toList.sortWith {
      (left, right) => {
        left._2 > right._2
      }
    }
    sortList.asJava  // 以Java.List的格式返回
  }
}
```



###### Java端代码如下：

```Java
import scala.Tuple2;
// 这里引用了Scala元组
import java.util.*;

public class WCImp {
    public static void main(String[] args) {
        List<String> sl = new ArrayList<String>();
        sl.add("Hello Scala Hbase kafka");
        sl.add("Hello Scala Hbase");
        sl.add(" Hello Scala");
        sl.add("Hello");
        System.out.println(sl);

        wc word = new wc();
        List<Tuple2<String, Object>> s = word.WordCount(sl);
        System.out.println(s);
    }
}


// 输出结果：
// [(Hello,4), (Scala,3), (Hbase,2), (,1), (kafka,1)]
```

关于这个Tuple元组类型，Java中没有原生支持元组，所以还是导入了一个Scala下的依赖包实现Java端数据的传输，这里是为了方便将传回的数据再在Java中进行处理，如果直接作为结果输出可以不需要在Scala那边转Java格式。

如果还想全部变成Java的话可以尝试在Scala那边将tuple转成map或者其他之后再传，但个人认为这个WordCount的例子中并没有必要。同时，在Java中引入Tuple2后也能实现Scala的元组操作，我觉得挺好玩的（但真不一定很方便）

关于Scala的数据类型，Java中一切类继承自Object类，所以String自然也是Object的子类；但是Scala中略有不同（参照博客[scala中的数据类型](https://www.cnblogs.com/tt-day/p/16499623.html)），Scala的基本数据类型继承自AnyVal，但是String，我们通过源码：

```java
package java.lang;

import java.io.ObjectStreamField;
import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.util.*;

public final class String
    implements java.io.Serializable, Comparable<String>, CharSequence
```

索引自String.java，所以说Scala中的String和Java中的String基本上是一样的（甚至跳转到的源码也是一样的）。作为参照，可以参考Scala其他数据类型的源码（以Int为例）：

```scala
package scala

// Int.scala
final abstract class Int private extends AnyVal {
  def toByte: Byte
  def toShort: Short
  def toChar: Char
  def toInt: Int
  def toLong: Long
  def toFloat: Float
  def toDouble: Double
}

// AnyVal.scala
abstract class AnyVal extends Any {
  def getClass(): Class[_ <: AnyVal] = null
}
```

这样就能很好的解释为什么在Scala和Java进行数据类型转换的时候String只能转String，而其他的都是AnyVal转Object了。

回到Java的Tuple2中，应该对于Tuple包有相应的包处理，但是我只是一时兴起，并没有过多深入，所以只是浅浅的在Java中对Tuple进行一个输出观察，代码如下：

```java
for (Tuple2 i: s) {
	System.out.println("i =" + i);
    System.out.println("i.toString =" + i.toString());
    System.out.println("Normal: " + i._1 + " " +  i._2);
    String s1 = (String) i._1;
    Integer s2 = (Integer) i._2;
    System.out.println("Change: " + s1 + " " + s2);
}

// 输出结果：
// i =(Hello,4)
// i.toString =(Hello,4)
// Normal: Hello 4
// Change: Hello 4
```

可以看出，输出结果在Java能够正常使用，用getClass函数查看：

```java
System.out.println("Normal: " + i._1.getClass() + " " +  i._2.getClass());
System.out.println("Normal: " + i._1.getClass() + " " +  i._2.getClass());

// 输出结果：
// Normal: class java.lang.String class java.lang.Integer
// Change: class java.lang.String class java.lang.Integer
```

但是注意！虽然其类型都是Java.lang.*，但是tuple中的仍然属于Object类型，需要转换。

本来还想做个Scala、Java转Json和读取Json的测试，但是需要引入其他的依赖，暂时先进行搁置。



------

参考资料：

[为什么String与Int，Boolean，Byte ...在scala中不同？](http://cn.voidcc.com/question/p-vrjwohbl-bad.html)

[将Java集合转换为Scala集合](https://qa.1r1g.com/sf/ask/47229941/)

[将Java集合转换为Scala集合](https://www.dovov.com/javascala-3.html)