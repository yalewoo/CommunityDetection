#导入需要的模块
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.ticker as ticker

#读取CSV数据为numpy record array记录
r = mlab.csv2rec(r"F:\HICODE_SUB\syn\param\allmod.txt")
n = r.dtype.names
r.sort()


        
#绘图
fig = plt.figure()
ax = fig.add_subplot(111)

ax.set_xlabel('p12')
ax.set_ylabel('modularity & nmi')

for col in n[1:]:
    ax.plot(r['p11'], r[col], 'o-',label=col)



handles, labels = ax.get_legend_handles_labels()
ax.legend(handles, labels, loc=4)

plt.legend(loc='upper center', bbox_to_anchor=(0.6,0.95),ncol=3,fancybox=True,shadow=True)

#图标的标题
#ax.set_title(fn)

#显示图片
plt.grid()

#将图片保存到指定目录
#plt.savefig(opath + fn + ".png")


plt.show()