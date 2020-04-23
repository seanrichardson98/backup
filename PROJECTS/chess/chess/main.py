from boardstate import *
from gui import *
from Node import *
from encoder import *

def load():
    return read_nodes()

def save(root):
    save_nodes(root)

def add_branch(node,state):
    child = Node(node,state)
    return child

def display_children(node):
    return

def to_child(node,child_num):
    return node.children[child_num]

def to_parent(node):
    return node.parent

try:
    root = load()
except:
    b = BoardState(1)
    root = Node(None,b)

current = root
gui = GUI()

while(True):

    gui.draw_board()
    gui.draw_position(current.boardstate)

    action = input("next: ")

    if action == "0":
        #make move
        coord1 = gui.wait_valid_input()
        coord2 = gui.wait_valid_input()
        new_state = new_boardstate(current.boardstate,coord1,coord2)
        current = add_branch(current,new_state)

    elif action == "1":
        current = to_parent(current)

    elif action == "2":
        child = input("child: ")
        current = to_child(current,int(child))
    
    elif action == "3":
        save(root)
        quit()

    elif action == "4":
        clear_data()
        b = BoardState(1)
        root = Node(None,b)
        current = root
    
    else: print("error")
