import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

L = 1
lo = -L/2
hi = L/2

V0 = 0

hbar = 6.62607004*pow(10,-34)/(2*np.pi)

mass = 1
hbar = 1


def A(k):
    return 8*np.sqrt(15)/pow(np.pi,3)*1/pow(k,3)

def E(k):
    return pow(k*np.pi*hbar/L,2)/(2*mass) + V0

#revival time
T = np.pi*hbar/E(1)*(4/3)/8

def psi(k,x):
    return np.sin(k*np.pi*(x+L/2)/L)

def cos_term(n,m,t):
    return np.cos((E(n)-E(m))*t/hbar)

def psi_sq(x,t):
    ret = 0
    for n in range(1,10,2):
        for m in range(1,10,2):
            ret += A(n)*A(m)*psi(n,x)*psi(m,x)*cos_term(n,m,t)
    return ret

print(A(1))
print(psi_sq(-L/2,0))


def graph():
    x = np.linspace(lo, hi, 1000)
    y = psi_sq(x,0)
    plt.plot(x,y)
    x = np.linspace(lo, hi, 1000)
    y = psi_sq(x,T/4)
    plt.plot(x,y)
    x = np.linspace(lo, hi, 1000)
    y = psi_sq(x,T/2)
    plt.plot(x,y)
    x = np.linspace(lo, hi, 1000)
    y = psi_sq(x,3*T/4)
    plt.plot(x,y)
    x = np.linspace(lo, hi, 1000)
    y = psi_sq(x,T)
    plt.plot(x,y)
    plt.show()

graph()

#fig = plt.figure()
#ax = plt.axes(xlim=(lo, hi), ylim=(0, 1.2))
#line, = ax.plot([], [], lw=2)
#
#def init():
#    line.set_data([], [])
#    return line,
#
#def animate(t):
#    x = np.linspace(lo, hi, 1000)
#    y = psi_sq(x,0.001*t*np.pi/4)
#    line.set_data(x, y)
#    return line,
#
#anim = animation.FuncAnimation(fig, animate, init_func=init, frames=200, interval=20, blit=True)
#
#anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264', 'imagemagick'])
#
#plt.show()
