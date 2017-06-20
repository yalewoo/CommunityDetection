#导入需要的模块
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.ticker as ticker
import sys
import os



path = sys.argv[1];
outpath = sys.argv[2];

r = mlab.csv2rec(path)
n = r.dtype.names
r.sort()

fn = os.path.basename(path)
#绘图

plt.xlabel('iter')
plt.ylabel(fn)

for col in n[1:]:
	plt.plot(r['x'], r[col], 'o-',label=col)

plt.ylim(0,1)


plt.legend(loc='upper center', bbox_to_anchor=(0.6,0.95),ncol=3,fancybox=True,shadow=True)



#图标的标题
plt.title(fn)

#显示图片
plt.grid()

#将图片保存到指定目录
plt.savefig(outpath)

    
    
