#TODO: 
#   - Click and drag instead of double click
#   - Stockfish implementation
#   - Multiple node structures

from boardstate import *
from gui import *
from Node import *

node_num = 0

#DECODER
def decode(state_string):
    state = BoardState(0)
    state_list = state_string.replace("/","")
    i = 0
    for char in state_list:
        state.board[i] = char
        i+=1
    return state

def read_nodes():
    tree_file = open("tree_file.txt","r")
    tree_data = tree_file.readlines()
    state_file = open("state_file.txt","r")
    state_data = state_file.readlines()
    return rn_helper(1,tree_data,state_data,None,None)

def rn_helper(num,tree_data,state_data,parent_node,root):
    tree_line = tree_data[num-1]
    state_line = state_data[num-1]
    child_list = tree_line.split(" ")[1:]
    state_string = state_line.split(" ")[1].rstrip()
    boardstate = decode(state_string)
    node = Node(parent_node,boardstate)
    if parent_node == None:
        root = node
    for child in child_list:
        child = child.rstrip()
        child = int(child)
        rn_helper(child,tree_data,state_data,node,root)
    return root

#str.rstrip() takes away trailing newline characters

#ENCODER
def piece_notation(num):
    arr = ["0","p","n","b","r","q","k",
               "P","N","B","R","Q","K"]
    return arr[num]

def encode(node):

    ret = ""
    ret += str(node.num) + " "
    for j in range(8):
        for i in range(8):
            piece_num = node.boardstate.get_coord(i+1,8-j)
            ret += str(piece_num)
        ret += "/"
    return ret
        

def save_nodes(root):
    global node_num
    node_num = 0
    sn_helper1(root)
    tree_file = open("tree_file.txt","w") 
    sn_helper2(root,tree_file)
    tree_file.close()
    state_file = open("state_file.txt","w")
    sn_helper3(root,state_file)
    state_file.close()

def sn_helper1(node):
#number the node
    global node_num
    node_num += 1
    node.num = node_num
    for child in node.children:
        sn_helper1(child)

def sn_helper2(node,file):
#encode tree structure
    file.write(str(node.num))
    for child in node.children:
        file.write(" " + str(child.num))
    file.write("\n")
    for child in node.children:
        sn_helper2(child,file)

def sn_helper3(node,file):
#encode 
    file.write(encode(node))
    file.write("\n")
    for child in node.children:
        sn_helper3(child,file)

def clear_data():
    open('tree_file.txt', 'w').close()
    open('state_file.txt','w').close()

def new_boardstate(old_state,coord1,coord2):
    new_state = BoardState(0)
    new_state.copy_state(old_state)
    new_state.move_piece(coord1,coord2)
    return new_state

def replay(root):
        
    
    gui = GUI()

    current = root

    for i in range(5):
        print(current.boardstate)
        print(current)
        gui.draw_board()
        gui.draw_position(current.boardstate)
        gui.win.getMouse()
        print(len(current.children))
        new = current.get_child(0)
        current = new
