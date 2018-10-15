# 算法用途

计算行向量的两两相似度。

# 算法原理

[算法出处](https://blog.twitter.com/engineering/en_us/a/2014/all-pairs-similarity-via-dimsum.html)

由Twitter提出的DIMSUMv2算法，就是这个思想，这个算法已经在Spark中实现了，就是RowMatrix中的columnSimilarity。下面这个算法详细描述一下。注意该算法原始论文中是计算矩阵的列向量两两相似度，这里计算的是行向量的两两相似度。

假设输入为：

1. 用户物品矩阵为m * n，即m个用户，n个物品。
2. 设定输出相似用户的阈值为s，即相似度低于s的相似用户不必再输出。
3. $\gamma = \frac{4*log(m)}{s}$  是一个计算过程使用的常数。
4. $a_{ij}$ 表示一个非0元素。

以Map-Reduce编程模型计算，其中Map阶段为：

For $a_{ij}$ in $c_j$ do:

---- with probability = min(1, $\frac{\sqrt{\gamma}}{||r_i||}$ ):

---- For $a_{kj}$ in $c_j$ do:

--------  with probability = min(1, $\frac{\sqrt{\gamma}}{||r_k||}$):

--------  emit((i, k), $\frac{a_{ij}* a_{kj}}{min(\sqrt{\gamma}, ||r_i||)min(\sqrt{\gamma}, ||r_k||)}$)

解释一下这个过程：

> 每一次读入的是一个物品的用户列表（即列向量），对用户列表遍历两次，以一定的概率累加当前物品用户之间的共同物品数，既然出现在同一个物品的用户列表中，说明这些用户至少共同喜欢当前这一个物品。
> 但是并不是对所有用户都计算，而是以一定的概率，这个概率与三个值有关：列向量的维度（总用户数），相似度阈值，行向量的模（当前用户喜欢的物品数，越多则当前记录被抛弃的可能性越大）。

其原理很符合直觉：对于每一个非零元素$a_{ij}$ 都是有助于行向量i和j的相似度的，但是行向量i和j如果自身非零元素很多，那么以一定的概率舍弃掉一些，并不会太损失最终相似度计算的精度。

DIMSUM算法的Reduce阶段就是求和，得到行向量i和行向量k之间的相似度了。

我们借助openmp来模拟一个map reduce计算向量相似度的过程，实际在生产系统中，Spark中的RowMatrix已经实现了这个算法，如果数据量较大(TB级别及以上)，可以考虑使用Spark这个库计算，否则采用Spark计算不太划算。如果单机可计算，建议采用openmp单机计算，充分发挥单机多核的计算力。

# 编译运行

## 编译

1. 仅支持Linux平台,如果支持其他平台，请自行编译boost库
2. 依赖boost， openmp
3. 依赖c++11(g++ 在4.8以上)

```
git clone --recursive https://github.com/xingwudao/dimsum.git

cmake -H. -Bbuild
cmake --build build -- -j3
cp build/bin/compute-row-similarity bin/

```

## 数据格式：

* 第一行是矩阵形状：行 列 非零元素
* 从第二行开始是稀疏矩阵：行 列 元素值

示例：

```
2 3 5
0 0 1
0 1 1
0 2 3
1 0 1
1 1 1
```

## 运行

```
./bin/compute-row-similarity -d data/matrix.dat   -o similarity.txt  -t 0.5
```

# 性能

测试矩阵

* 行42177
* 列4809
* 非零元素 100483


[Spark.Rowmatrix.ColumnSimilairty](https://spark.apache.org/docs/latest/api/java/org/apache/spark/mllib/linalg/distributed/RowMatrix.html#columnSimilarities) ：30min

本程序：30s

所以，我再一次鼓吹：

> 不要为了分布式而分布式，你没有那么大的数据。
