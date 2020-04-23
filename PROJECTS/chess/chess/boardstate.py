class BoardState:

#boardstate
#


#get


    def __init__(self, state):
        if state == 0:
            self.board = ["0"]*64

    def start_position(self):    
        self.board =  [ "r","n","b","q","k","b","n","r",
                        "p","p","p","p","p","p","p","p",
                        "0","0","0","0","0","0","0","0",
                        "0","0","0","0","0","0","0","0",
                        "0","0","0","0","0","0","0","0",
                        "0","0","0","0","0","0","0","0",
                        "P","P","P","P","P","P","P","P",
                        "R","N","B","Q","K","B","N","R"]

    def init_fen(fen):
        return

    def get_fen(fen):
        if self.fen != None:
            return fen
        str1 = ''.join(self.board)


    def copy_state(self,other):
        for i in range(64):
            self.board[i] = other.board[i]
    
    def get_coord(self,i,j):
        return self.board[(8-j)*8+i-1]

    def set_coord(self,i,j,val):
        self.board[(8-j)*8+i-1] = val


#    def get_coord(self,i,j):
#        return self.board[8*(i-1)+(j-1)]
#
#    def set_coord(self,i,j,val):
#        self.board[8*(i-1)+(j-1)] = val

    def move_piece(self,coord1,coord2):
        piece = self.get_coord(coord1[0],coord1[1])
        if (piece == 0): return
        self.set_coord(coord1[0],coord1[1],0)
        self.set_coord(coord2[0],coord2[1],piece)

    def __str__(self):
        ret = ""
        for j in range(0,8):
            for i in range(1,9):
                ret += str(self.get_coord(i,8-j)) + "\t"
            ret += "\n"
        return ret
