from sympy import *
#for i in range(1000):
#    prod = 1
#    for j in range(i**2):
#        if isprime(j):
#            prod *= j/(j-1)
#    print(str(i**2) + ": " + str(prod))

for x in range(2,1000):
    if isprime(x):
    #number of primes
        pi_x = 0
        for i in range(0,x+1):
            if isprime(i):
                pi_x += 1
        ret = (float(x)/float(x-1))**pi_x
        print(x/x-1)
        print(str(x) + ": " + str(ret))

