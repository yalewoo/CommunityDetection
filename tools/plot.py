#导入需要的模块
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.ticker as ticker

l = ["mod","nmi_truth","nmi_truth1","nmi_truth2","nmi_truth1_1","nmi_truth1_2","nmi_truth2_1","nmi_truth2_2","nmi_last"]

for fn in l:
    path = "F:/Project/CommunityDetection/vs2015/vs2015/hicode/";
    path = path + fn + ".txt"
    r = mlab.csv2rec(path)
    n = r.dtype.names
    r.sort()
    
    
    #绘图
    fig = plt.figure()
    ax = fig.add_subplot(111)
    
    ax.set_xlabel('iter')
    ax.set_ylabel(fn)
    
    for col in n[1:]:
        ax.plot(r['x'], r[col], 'o-',label=col)
    
    
    
    handles, labels = ax.get_legend_handles_labels()
    ax.legend(handles, labels, loc=4)
    
    
    #图标的标题
    ax.set_title(fn)
    
    #显示图片
    plt.grid()
    plt.show()
    
    #将图片保存到指定目录
    #plt.savefig("mod.png")
    
    a = 3



