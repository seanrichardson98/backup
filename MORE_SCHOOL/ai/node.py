import numpy as np

class oneNode:
    def __init__(self,inp):
        self.inp = inp
        inp.out = self
    def eval(self):
        self.val = self.operation(self.inp.eval())
        return self.val
    def grad(self,prev):
        return self.local_grad(prev)*self.out.grad(self)
    #holders:
    def operation(self,val):
        return None
    def local_grad(self,prev):
        return None

class twoNode:
    def __init__(self,input1,input2):
        self.in1 = input1
        self.in2 = input2
        input1.out = self
        input2.out = self
    def eval(self):
        self.val = self.operation(self.in1.eval(),self.in2.eval())
        return self.val
    def grad(self,prev):
        return self.local_grad(prev)*self.out.grad(self)
    #holders:
    def operation(self,val1,val2):
        return None
    def local_grad(self,prev):
        return None

class Var:
    def __init__(self,val):
        self.val = val
    def eval(self):
        return self.val
    def grad(self):
        return self.out.grad(self)

class Add(twoNode):
    def operation(self,val1,val2):
        return val1+val2
    def local_grad(self,prev):
        return 1

class Mult(twoNode):
    def operation(self,val1,val2):
        return val1*val2
    def local_grad(self,prev):
        return self.in1.val*self.in2.val/prev.val

class Inv(oneNode):
    def operation(self,val):
        return 1/val
    def local_grad(self,prev):
        return -1/(self.inp.val**2)

class Exp(oneNode):
    def operation(self,val):
        return np.exp(val)
    def local_grad(self,prev):
        return np.exp(self.inp.val)

class Scale(oneNode):
    def __init__(self,inp,scale_factor):
        self.inp = inp
        inp.out = self
        self.scale = scale_factor
    def operation(self,val):
        return self.scale*val
    def local_grad(self,prev):
        return self.scale

class AddConst(oneNode):
    def __init__(self,inp,const):
        self.inp = inp
        inp.out = self
        self.const = const
    def operation(self,val):
        return self.const+val
    def local_grad(self,prev):
        return 1

class Out:
    def __init__(self,inp):
        self.inp = inp
        inp.out = self
    def eval(self):
        self.val = self.inp.eval()
        return self.val
    def grad(self,prev):
        return 1
