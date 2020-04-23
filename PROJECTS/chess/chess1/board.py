import chess

#TODO:
#parse fen

class Board:

#Variables:
#   fen
#   board ... 64 array with FEN notation and "."
#   color ... string "w" or "b"
#   castling ... FEN notation
#   
#FEN
#board arr
#color
#castling rights
#en passant square

    def __init__(self,fen):
        self.fen = fen
        self.parse_fen(self.fen)

    def __init__(self):
        self.fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        self.parse_fen(self.fen)

    def parse_fen(self,fen):
        split_fen = fen.split()
        #board:
        board_arr = []
        board_fen = split_fen[0]
        board_fen = board_fen.replace("/","")
        board_temp = list(board_fen)
        for char in board_temp:
            if char.isdigit(): board_arr.extend(["."]*int(char))
            else: board_arr.append(char)
    
        self.board = board_arr
        self.color = split_fen[1]
        self.castling = split_fen[2]
        self.enpassant = split_fen[3]
        self.halfmove = split_fen[4]
        self.full_move = split_fen[5]
        
    def get_coord(self,i,j):
        return self.board[(8-j)*8+i-1]

    def get_fen(self):
        pass

    def get_board(self):
        pass

    def get_legal_fens(self):
        pass

    def __str__(self):
        ret = self.color + " " + self.castling + " " + self.enpassant + "\n"
        for j in range(0,8):
            for i in range(1,9):
                ret += str(self.get_coord(i,8-j)) + " "
            ret += "\n"
        return ret




