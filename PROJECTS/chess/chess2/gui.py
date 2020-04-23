from graphics import *

class GUI:

    BOARD_SIZE = 600
    LEFT_ = 50
    RIGHT_ = 300
    UP_ = 200
    DOWN_ = 200
    square_size = BOARD_SIZE/8.0
    WIN_HEIGHT = BOARD_SIZE + UP_ + DOWN_
    WIN_WIDTH = BOARD_SIZE + RIGHT_ + LEFT_

    BOARD = [Point(LEFT_,UP_),Point(LEFT_+BOARD_SIZE,UP_+BOARD_SIZE),"Board"]

    #BOARD_MENU:
    BOARD_MENU = [[Point(50,50),Point(250,150),"Study"],
     [Point(250,50),Point(450,150),"Test"],
     [Point(450,50),Point(650,150),"Edit"],
     [Point(700,50),Point(900,150),"Menu"]]
 
    #EDIT:
    EDIT_MENU = [[Point(50,50),Point(250,150),"Undo"],
     [Point(250,50),Point(450,150),"Back"],
     [Point(450,50),Point(650,150),"Delete Line"],
     [Point(700,50),Point(900,150),"Save & Quit"]]

    MOVES_X = 50
    MOVES_Y = 850
    MOVES_DIFF = 50

    #MOVES:
    NOTATION = [Point(700,200),Point(900,800),"Notation"]

    piece_ref = {"0":None,
"P":"\u2659","N":"\u2658","B":"\u2657","R":"\u2656","Q":"\u2655","K":"\u2654",
"p":"\u265F","n":"\u265E","b":"\u265D","r":"\u265C","q":"\u265B","k":"\u265A"}

    def __init__(self):
        self.win = GraphWin("Test", self.WIN_WIDTH, self.WIN_HEIGHT,autoflush=False)
        self.state = "blank"

    #UTIL:
    def click_coord(self,x,y):
        I = int((x-self.LEFT_)/self.square_size+1)
        J = 8-int((y-self.UP_  )/self.square_size)
        return (I,J)

    def mid_point(self,p1,p2):
        return Point((p1.getX()+p2.getX())/2,
                    (p1.getY()+p2.getY())/2)


    def in_bb(self,point,p1,p2):
        return (point.getX() > p1.getX() and point.getY() > p1.getY()
        and point.getX() < p2.getX() and point.getY() < p2.getY())

    #DRAWING STATES:
    def clear(self):
        for item in self.win.items[:]:
            item.undraw()
        self.win.update()

    def draw_piece(self,i,j,piece_num):
        sq = self.square_size
        piece = Text(Point(i*sq+self.LEFT_+sq/2.0,
                j*sq+self.UP_+sq/2.0), self.piece_ref[piece_num])
        piece.setSize(36)
        piece.draw(self.win)

    def draw_position(self,position):
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

        for i in range(0,8):
            for j in range(0,8):
                piece_num = position.get_coord(i+1,8-j)
                if (piece_num != 0):
                    self.draw_piece(i,j,piece_num)

    def draw_home(self):
        return

    def draw_board_menu(self):
        for item in self.BOARD_MENU:
            r = Rectangle(item[0],item[1])
            r.draw(self.win)
            t = Text(Point((item[0].getX()+item[1].getX())/2,
                    (item[0].getY()+item[1].getY())/2),item[2])
            t.draw(self.win)
        r = Rectangle(self.NOTATION[0],self.NOTATION[1])
        r.draw(self.win)
       
    def draw_edit(self):
        for item in self.EDIT_MENU:
            r = Rectangle(item[0],item[1])
            r.draw(self.win)
            t = Text(Point((item[0].getX()+item[1].getX())/2,
                    (item[0].getY()+item[1].getY())/2),item[2])
            t.draw(self.win)
        r = Rectangle(self.NOTATION[0],self.NOTATION[1])
        r.draw(self.win)
        i = 0
        for child in self.node.children:
            p1 = Point(self.MOVES_X+(i*self.MOVES_DIFF),self.MOVES_Y)
            p2 = Point(self.MOVES_X+(i+1)*self.MOVES_DIFF,self.MOVES_Y+self.MOVES_DIFF)
            r = Rectangle(p1,p2)
            t = Text(self.mid_point(p1,p2),str(i))
            r.draw(self.win)
            t.draw(self.win)
            i += 1

    #INPUT IN STATES:
    def board_menu_input(self,point):
        item = self.BOARD
#if (point.getX() > item[0].getX() and point.getY() > item[0].getY()
#       and point.getX() < item[1].getX() and point.getX() < item[1].getX()):
        if self.in_bb(point,item[0],item[1]):
            coord = self.click_coord(point.getX(),point.getY())
            return (item[2],coord[0],coord[1])
        for item in self.BOARD_MENU:    
            if self.in_bb(point,item[0],item[1]):
                return (item[2],None,None)
        return ("None",None,None)

    def edit_input(self,point):
        item = self.BOARD
        if self.in_bb(point,item[0],item[1]):
            coord = self.click_coord(point.getX(),point.getY())
            return (item[2],coord[0],coord[1])
        for item in self.EDIT_MENU:    
            if self.in_bb(point,item[0],item[1]):
        
                return (item[2],None,None)
        if self.in_bb(point,Point(self.MOVES_X,self.MOVES_Y),Point(self.MOVES_X+self.MOVES_DIFF*len(self.node.children),self.MOVES_Y+50)):
            i = int((point.getX()-50)/self.MOVES_DIFF)
            print(i)
            print("test" + str(self.node.children[i]))
            return ("Child",self.node.children[i],None)
        return ("None",None,None)
       
    #CENTRAL:
    def wait_input(self):
        p = self.win.getMouse()
        if self.state == "blank": return ("None",None,None)
        elif self.state == "menu": pass
        elif self.state == "board_menu": return self.board_menu_input(p)
        elif self.state == "edit": return self.edit_input(p)
        else: print("error - invalid board state")

    def update(self):
        self.clear()
        if self.state == "menu": pass
        if self.state == "board_menu":
            self.draw_position(self.board)
            self.draw_board_menu()
        elif self.state == "edit":
            self.draw_position(self.board)
            self.draw_edit()
        self.win.update()

    #SETTERS:
    def give_node(self,node):
        self.node = node
        self.board = node.boardstate

    def set_state(self,state):
        self.state = state

