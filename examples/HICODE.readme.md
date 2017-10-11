##图文件格式
将图文件放到目录中，命名必须为graph 无扩展名
graph文件格式为：
第一行可选结点个数 或省略
下面每行表示一条边，可带权或无权
例如：
```
3000
1 2 1
1 11 1
1 14 1
1 18 1
1 24 1
```
```
1 2 1
1 11 1
1 14 1
1 18 1
1 24 1
```
```
3000
1 2
1 11
1 14
1 18
1 24
```
```
1 2
1 11
1 14
1 18
1 24
```
##使用方法：
run.exe F:/HICODE_SUB/syn/runtest/
对图 F:/HICODE_SUB/syn/runtest/graph 运行HICODE
默认配置文件为 和代码文件同目录的 hicode_default.config
自定义配置文件为 F:/HICODE_SUB/syn/runtest/hicode.config 会覆盖前者
配置文件示例：
```
###########################
#####    hicode     #######
###########################

#truth文件
#truth=truth1.gen;truth2.gen
truth=;

#消边方法 可选:ReduceWeight;Remove
reduce_method=ReduceWeight

#基算法 推荐：mod;infomap 可选：oslom;demon;lc;gce;cfinder
base_alg=mod;infomap

#层数
number_of_layers=2

#迭代次数
number_of_iteration=20



#是否输出log信息

#计算每次迭代的modularity值
log_modularity=true

#输出每次迭代结果变化
log_nmi_last=true

#每次迭代保存当时所用的削边后的图
log_save_reduce_graph=false


###########################
#####      sub      #######
###########################

#使用递归抓子图找社团
use_r_sub=true

#递归次数
sub_times=1

#节点多余下面的数字才递归抓子图
sub_nodes_number_thres=9

#maxOri和maxLayer
sub_maxori=true
sub_maxlayer=false
```

##使用方法
hicode.exe F:/HICODE_SUB/syn/runtest/