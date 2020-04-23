import sympy

def is_perfect(value, exponent):
    root = value**(1.0/exponent)
    root = float(round(root))
    return (root**exponent == value)

def a():
    for a in range(0,100):
        print("ALPHA = {}".format(a))
        p = 2
        while (p**2 <= a + 2):
            b = 1
            if (sympy.isprime(p)):
                while (p**b <= a+b):
                    if ((a+b)/(p**b)).is_integer():
                        print("({},{})".format(p,p**b))
                    b+=1
            p+=1

def b():
    for a in range(0,100):
        print("ALPHA = {}".format(a))
        p = 2
        for p in range(0,1000):
            if (sympy.isprime(p)):
                n = 2
                while ((p**a)*n >= 2**n):
                    if is_perfect(((p**a)*n),n):
                        print("({},{})".format(p,n))
                    n+=1
            p+=1
    
b()
