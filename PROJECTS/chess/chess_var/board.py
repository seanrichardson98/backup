class Board:
    #arr board

    def __init__(self):
        self.board = ["r","n","b","q","k","b","n","r",
                      "p","p","p","p","p","p","p","p",
                      ".",".",".",".",".",".",".",".",
                      ".",".",".",".",".",".",".",".",
                      ".",".",".",".",".",".",".",".",
                      ".",".",".",".",".",".",".",".",
                      "P","P","P","P","P","P","P","P",
                      "R","N","B","Q","K","B","N","R"]

#    def __init__(self,other):
#        #TODO - copy method
#        pass
#
#    def __init__(self,fen):
#        #TODO - fen init
#        pass

    def make_copy(self):
        copy = Board()
        for i in range(63):
            copy.board[i] = self.board[i]
        return copy

    def white_piece(self,piece):
        if piece == 

    def get_piece(self,i,j):
        if (i < 1 or i > 8 or j < 1 or j > 8):
            return 'x'
        return self.board[(8-j)*8+(i-1)]

#    def get_piece(self,coord):
#        #TODO - coord is g4 etc.
#        pass

    def set_piece(self,i,j,piece):
        self.board[(8-j)*8+(i-1)] = piece

    def make_move(move):
        copy = self.make_copy()
        move.make_move(copy)
        return copy
    
    def white_pawn_moves(self,i,j):
        #TODO (enpassant, double)
        if (get_piece(i,j+1))
        pass

    def black_pawn_moves(self,i,j):
        #TODO (enpassant, double)
        pass

    def knight_moves(self,i,j):
        #TODO
        pass

    def bishop_moves(self,i,j):
        #TODO
        pass

    def rook_moves(self,i,j):
        #TODO
        pass

    def queen_moves(self,i,j):
        #TODO
        pass

    def white_king_moves(self,i,j):
        #TODO (castling)
        pass

    def black_king_moves(self,i,j):
        #TODO (castling)

    def in_check(self):
        #TODO
        pass

    def legal_moves(self):
        legal_moves = []
        for i in range(1,9):
            for j in range(1,9):
                p = get_piece(i,j)
                if (p == '.'):
                    pass
                elif (p == 'P'):
                    moves = white_pawn_moves(i,j)
                elif (p == 'p'):
                    moves = black_pawn_moves(i,j)
                elif (p == 'N' or p == 'n'):
                    moves = knight_moves(i,j)
                elif (p == 'B' or p == 'b'):
                    moves = bishop_moves(i,j)
                elif (p == 'R' or p == 'r'):
                    moves = rook_moves(i,j)
                elif (p == 'Q' or p == 'q'):
                    moves = queen_moves(i,j)
                elif (p == 'K'):
                    moves = white_king_moves(i,j)
                elif (p == 'k'):
                    moves = black_king_moves(i,j)
                legal_moves.append(moves)
                else:
                    print("error")

    def get_fen(self):
        #TODO
        pass

    def __str__(self):
        ret = ""
        for i in range(8):
            for j in range(8):
                ret += (self.board[i*8+j] + " ")
            ret += "\n"
        return ret

class Move:
    def __init__(self):
        self.remove = []
        self.add = []

    def plus_add(self,add):
        self.add.append(add)
    
    def plus_remove(self,rem):
        self.add.remove(rem)

    def make_move(self,board):
        for rem in self.remove:
            board.set_piece(rem[0],rem[1],rem[2])
        for add in self.add:
            board.set_piece(rem[0],rem[1],".")












