#导入需要的模块
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.ticker as ticker

#读取CSV数据为numpy record array记录
r = mlab.csv2rec(r"F:\Local\VGG\conv1_2_modularity.txt")
r.sort()


        
#绘图
fig = plt.figure()
ax = fig.add_subplot(111)

ax.set_xlabel('thres')
ax.set_ylabel('Modularity')

l1 = ax.plot(r['thres'], r['truth1'], 'o-',label='truth1')

l2 = ax.plot(r['thres'], r['truth2'], 'o-',label='truth2')

l3 = ax.plot(r['thres'], r['r1'], 'o-',label='truth1 on Reduced Graph')

l4 = ax.plot(r['thres'], r['r2'], 'o-',label='truth2 on Reduced Graph')

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