import os,sys
from datetime import datetime



sizes = range(512, 2048+512, 256)
blockSizes = [16, 64]
cnt=0
start = datetime.now()
for b in blockSizes:
    for s in sizes:
        for mod in [-2,-1,0,1,2]:
            elapsed = datetime.now() - start
            cmd = "./bin/matmul " + str(s+mod) + " " + str(b) + " 3 " + sys.argv[1] 
            print(cmd)
            cnt+=1
            print()
            print("Elapsed "+str(elapsed)+" sec." + str(cnt) + "/80")
            os.system(cmd)
