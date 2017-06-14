#导入需要的模块
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.ticker as ticker

l = ["mod","nmi_truth","nmi_truth1","nmi_truth2","nmi_truth1_1","nmi_truth1_2","nmi_truth2_1","nmi_truth2_2","nmi_last"]

for fn in l:
    opath = "F:/Project/CommunityDetection/vs2015/vs2015/hicode_mod/";
    #opath = "F:/HICODE_SUB/syn/3000_21/hicode/";
    #opath = "F:/Project/CommunityDetection/vs2015/vs2015/hicode/";
    #opath = "F:/HICODE_SUB/facebook/Smith60/hicode/"
    path = opath + fn + ".txt"
    r = mlab.csv2rec(path)
    n = r.dtype.names
    r.sort()
    
    
    #绘图
    plt.xlabel('iter')
    plt.ylabel(fn)

    for col in n[1:]:
        plt.plot(r['x'], r[col], 'o-',label=col)
    
    plt.ylim(-0.1,1.1)
    

    plt.legend(loc='upper center', bbox_to_anchor=(0.6,0.95),ncol=3,fancybox=True,shadow=True)

    
    
    #图标的标题
    plt.title(fn)
    
    #显示图片
    plt.grid()
    
    #将图片保存到指定目录
    plt.savefig(opath + fn + ".png")
    
    
    plt.show()
    
    
