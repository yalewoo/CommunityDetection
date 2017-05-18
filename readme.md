
## 本程序仍在开发中

## 常用社团检测算法

本程序是使用常用社团检测算法的打包，目前支持调用以下7种算法（通过命令行调用其提供的可执行文件）：

* [Louvain Modularity](https://sourceforge.net/p/louvain/code/ci/default/tree/)
* [Link Communities](https://github.com/bagrow/linkcomm)
* [OSLOM](http://www.oslom.org/)
* [Infomap](http://www.mapequation.org/code.html)
* [Greedy Clique Expansion（GCE）](https://sites.google.com/site/greedycliqueexpansion/)
* [DEMON](http://www.michelecoscia.com/?page_id=42)
* [CFinder](http://www.cfinder.org/)


### 使用示例
```
#include <cstdio>
#include "Graph.h"

map<string, string> Graph::config;

int main()
{
	//读配置文件，该文件记录社团检测算法的具体路径
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	Graph g;
	//从edge list的文本文件读入图
	g.load("F:/Project/CommunityDetection/graph.txt");


	g.runInfomap().save("result/Infomap.gen");
	g.runLinkComm().save("result/LC.gen");
	g.runOSLOM2().save("result/OSLOM.gen");
	g.runGCE().save("result/GCE.gen");
	g.runDemon().save("result/Demon.gen");
	g.runCFinder().save("result/CFinder.gen");
	g.runMod().save("result/Mod.gen");

	/*
	g.showPic();
	Communities cs = g.runMod();
	g = g.remove(cs);
	g.print();
	g.showPic();*/

	printf("------------\ndone\n");
	return 0;
}
```