import os

for i in range(0,878):
    os.system("convert pic" + str(i).zfill(4) + ".xwd pic" + str(i).zfill(4) + ".png")
    print(str(i).zfill(4))
