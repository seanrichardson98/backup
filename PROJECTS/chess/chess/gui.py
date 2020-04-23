
from graphics import *

class GUI:

    WIN_SIZE = 800
    LEFT_ = 100
    RIGHT_ = 100
    UP_ = 100
    DOWN_ = 100
    lo = LEFT_
    hi = WIN_SIZE - RIGHT_
    square_size = (hi-lo)/8.0

    piece_ref = {"0":None,
"P":"\u2659","N":"\u2658","B":"\u2657","R":"\u2656","Q":"\u2655","K":"\u2654",
"p":"\u265F","n":"\u265E","b":"\u265D","r":"\u265C","q":"\u265B","k":"\u265A"}

    def __init__(self):
        self.win = GraphWin("Test", self.WIN_SIZE, self.WIN_SIZE)

    def draw_piece(self,i,j,piece_num):
        sq = self.square_size
        piece = Text(Point(i*sq+self.LEFT_+sq/2.0,
                j*sq+self.UP_+sq/2.0), self.piece_ref[piece_num])
        piece.setSize(36)
        piece.draw(self.win)

    def draw_board(self):
        for i in range(0,8):
            for j in range(0,8):
                p1 = Point(i*self.square_size+self.LEFT_,
                     j*self.square_size+self.UP_)
                p2 = Point((i+1)*self.square_size+self.LEFT_,
                     (j+1)*self.square_size+self.UP_)
                r = Rectangle(p1,p2)
                if (i+j)%2 == 0: r.setFill("white")
                else: r.setFill("grey")
                r.draw(self.win)

    def draw_position(self,position):
        for i in range(0,8):
            for j in range(0,8):
                piece_num = position.get_coord(i+1,8-j)
                if (piece_num != 0):
                    self.draw_piece(i,j,piece_num)
                   
    def click_coord(self,x,y):
        I = int((x-self.LEFT_)/self.square_size+1)
        J = 8-int((y-self.UP_  )/self.square_size)
        return (I,J)
       
    def wait_valid_input(self):
        p = self.win.getMouse()
        return self.click_coord(p.getX(),p.getY())

    def wait_input(self):
        p = self.win.getMouse()
        click1 = self.click_coord(p.getX(),p.getY())
        p = self.win.getMouse()
        click2 = self.click_coord(p.getX(),p.getY())
        return (click1,click2)




