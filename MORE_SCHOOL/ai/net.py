from node import *


w0 = Var(2.0)
x0 = Var(-1.0)
w1 = Var(-3.0)
x1 = Var(-2.0)
w2 = Var(-3.0)

a = Mult(w0,x0)
b = Mult(w1,x1)
c = Add(a,b)
d = Add(c,w2)
e = Scale(d,-1)
f = Exp(e)
g = AddConst(f,1)
h = Inv(g)
out = Out(h)

print(out.eval())
print(w0.grad())
