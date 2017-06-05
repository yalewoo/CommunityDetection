#导入需要的模块
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.ticker as ticker

#读取CSV数据为numpy record array记录
#n = ["x","layer1","layer2"]
r = mlab.csv2rec(r"F:\Project\CommunityDetection\vs2015\vs2015\hicode\nmi_last.txt")
n = r.dtype.names
r.sort()



        
#绘图
fig = plt.figure()
ax = fig.add_subplot(111)

ax.set_xlabel('thres')
ax.set_ylabel('Modularity')

for col in n[1:]:
    ax.plot(r['x'], r[col], 'o-',label=col)





handles, labels = ax.get_legend_handles_labels()
ax.legend(handles, labels, loc=2)


#图标的标题
ax.set_title(u"Modularity on different thres")


#在折线图上标记数据，-+0.1是为了错开一点显示数据
#datadotxy=tuple(zip(r['thres'],r['truth1']))
#for dotxy in datadotxy:
#    ax.annotate(str(float(dotxy[1])),xy=dotxy)


#显示图片
plt.grid()
plt.show()

#将图片保存到指定目录
#plt.savefig("mod.png")