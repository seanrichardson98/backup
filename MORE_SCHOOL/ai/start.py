import numpy as np

def loss(data,a,b):
    X = data[:,0]
    Y = data[:,1]
    vec = Y-(a*X+b)
    return np.linalg.norm(vec)**2

def grad_loss(data,a,b):
    X = data[:,0]
    Y = data[:,1]
    vec = Y-(a*X+b)
    da = np.dot(-2*vec,X)
    db = np.dot(-2*vec,np.ones(4))
    return np.array([da,db])

def get_a(data):
    X = data[:,0]
    Y = data[:,1]
    Xmean = np.full(4,np.mean(X))
    Ymean = np.full(4,np.mean(Y))
    return (np.dot(X-Xmean,Y-Ymean)/np.dot(X-Xmean,X-Xmean))

def get_b(data,a):
    X = data[:,0]
    Y = data[:,1]
    return (np.mean(Y)-a*np.mean(X))

a = 10
b = 100
vec = np.array([a,b])
step = 0.01

data = np.array([[1,2],[3,4],[2,2],[1,1]])

print(loss(data,a,b))
print(grad_loss(data,a,b)*step)

for i in range(0,1000):
    print(loss(data,vec[0],vec[1]))
    vec = vec - step*grad_loss(data,vec[0],vec[1])

print(vec)
a=get_a(data)
b=get_b(data,a)
print(str(a)+ "  " + str(b))


