
def is_legal(arr):
    check = [0]*20
    for a in arr:
        for b in arr:
            if (b > a):
                index = b-a
                check[index-1] = 1
                check[(21-index)-1] = 1
    for c in check:
        if c == 0: return 0
    return 1


for a in range(1,21):
    for b in range(a+1,21):
        for c in range(b+1,21):
            for d in range(c+1,21):
                for e in range(d+1,21):
                    arr = [a,b,c,d,e]
                    if is_legal(arr) == 1:
                        print(arr)
#arr = [1,2,4,10]

