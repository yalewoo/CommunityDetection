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
run.exe F:/HICODE_SUB/syn/runtest/ mod infomap
对图 F:/HICODE_SUB/syn/3000/graph 运行算法mod和infomap，运行结果会放到F:/HICODE_SUB/syn/3000/baseline/目录下