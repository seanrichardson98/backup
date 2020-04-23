from graphics import *

WIDTH, HEIGHT = 500, 500
POSITION = Point(250, 250)
RADIUS = 20
STEPS = 7

def MouseTracker(window, shape):
    while True:
        position = window.getMouse()

        if position != None:  # in case we want to use checkMouse() later
            center = shape.getCenter()
            xincr = (position.getX() - center.getX()) / STEPS
            yincr = (position.getY() - center.getY()) / STEPS
            for _ in range(STEPS):
                shape.move(xincr, yincr)

win = GraphWin("MyWindow", WIDTH, HEIGHT)
win.setBackground("blue")
cir = Circle(POSITION, RADIUS)
cir.setFill("red")
cir.draw(win)
MouseTracker(win, cir)
