from board import *
from gui import *
from node import *
from encoder import *

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
   print("attempt")
   root = load()
except:
   print("new data")
   b = Board()
   root = Node(None,b)

current = root
gui = GUI()

while(True):

    gui.set_state("board_menu")
    gui.give_board(current.boardstate)
    gui.update()

    action = input("next: ")

    if action == "0":
        #make move
        coord1 = gui.wait_input()
        coord2 = gui.wait_input()
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
        b = Board()
        root = Node(None,b)
        current = root
    
    else: print("error")
