class Board:

#board arr
#color: 
#castling rights
#en passant square

    def __init__(self):
        self.board =  [ "r","n","b","q","k","b","n","r",
                        "p","p","p","p","p","p","p","p",
                        "0","0","0","0","0","0","0","0",
                        "0","0","0","0","0","0","0","0",
                        "0","0","0","0","0","0","0","0",
                        "0","0","0","0","0","0","0","0",
                        "P","P","P","P","P","P","P","P",
                        "R","N","B","Q","K","B","N","R"]
        self.color = "w"
        self.castling = "KQkq"
        self.enpassant = "-"
        self.fen = "no_fen"

    def copy(self,other):
        return

    def set_with_fen(self,fen):
        ret = []
        board = fen.split(" ")[0]
        board = board.replace("/","")
        arr = list(board)
        for char in arr:
            if char.isdigit(): ret.extend(["0"]*int(char))
            else: ret.append(char)

        self.board = ret
        self.color = fen.split(" ")[1]
        self.castling = fen.split(" ")[2]
        self.enpassant = fen.split(" ")[3]
        
    def make_fen(self):
        ret = ""
        for i in range(8):
            row = self.board[i*8:(i+1)*8]
            count = 0
            for square in row:
                if square == "0": count += 1
                else:
                    if count != 0: 
                        ret += str(count)
                        count = 0
                    ret += str(square)
            if count != 0: ret += str(count)
            if i != 7: ret += "/"
        ret += " " + self.color
        ret += " " + self.castling
        ret += " " + self.enpassant

        self.fen = ret
        return ret

    def get_fen(self):
        if self.fen == "no_fen":
            self.make_fen()
        return self.fen

    def copy_state(self,other):
        for i in range(64):
            self.board[i] = other.board[i]
    
    def get_coord(self,i,j):
        return self.board[(8-j)*8+i-1]

    def set_coord(self,i,j,val):
        self.board[(8-j)*8+i-1] = val

    def move_piece(self,coord1,coord2):
        piece = self.get_coord(coord1[0],coord1[1])
        if (piece == 0): return
        self.set_coord(coord1[0],coord1[1],"0")
        self.set_coord(coord2[0],coord2[1],piece)

    def __str__(self):
        ret = self.color + " " + self.castling + " " + self.enpassant + "\n"
        for j in range(0,8):
            for i in range(1,9):
                ret += str(self.get_coord(i,8-j)) + "\t"
            ret += "\n"
        return ret
