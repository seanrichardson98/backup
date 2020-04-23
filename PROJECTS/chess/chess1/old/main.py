from node import *
from encoder import *
from gui import *
from board import *

current = None
root = None

def eval_menu_inp(inp,gui):
    global current,root
    root = load()
    current = root
    gui.state = "board_menu"

def eval_bm_inp(inp,gui):
    global current,root
    if inp[0] == "None": pass
    elif inp[0] == "Edit": 
        root = load()
        current = root
        gui.set_state("edit")
    elif inp[0] == "Board": 
        coord1 = (inp[1],inp[2])
        inp = gui.wait_input()
        if inp[0] == "Board":
            coord2 = (inp[1],inp[2])
            if not (coord1[0] == coord2[0] and coord1[1] == coord2[1]):
                new_state = new_boardstate(current.boardstate,coord1,coord2)
                current = Node(current,new_state)

def eval_edit_input(inp,gui):
    global current,root
    if inp[0] == "None": print("outside")
    elif inp[0] == "Save & Quit":
        save_nodes(root) 
        gui.state = "board_menu"
    elif inp[0] == "Undo":
        print("Yeah, that button doesn't work yet...")
    elif inp[0] == "Back":
        if current.parent != None:
            current = current.parent
    elif inp[0] == "Delete Line":
        print("Yeah, that button doesn't work yet...")
    elif inp[0] == "Board":
        coord1 = (inp[1],inp[2])
        inp = gui.wait_input()
        if inp[0] == "Board":
            coord2 = (inp[1],inp[2])
            if not (coord1[0] == coord2[0] and coord1[1] == coord2[1]):
                new_state = new_boardstate(current.boardstate,coord1,coord2)
                current = Node(current,new_state)
    elif inp[0] == "Child":
        current = inp[1]
        print("inp:" + str(inp[1]))
    else: print("error")

#MAIN LOOP:
    
gui = GUI()
gui.set_state("menu")
while True:
    print(gui.state)
    if current != None: print(len(current.boardstate.board))
    if gui.state != "menu": gui.give_node(current)
    gui.update()
    inp = gui.wait_input()
    if gui.state == "menu":
        eval_menu_inp(inp,gui)
    elif gui.state == "board_menu":
        eval_bm_inp(inp,gui)
    elif gui.state == "edit":
        eval_edit_input(inp,gui)


while True:
    gui.give_node(current)
    gui.update()
    inp = gui.wait_input()
    print(inp[0])
    if gui.state == "menu":
        pass

    if gui.state == "board_menu":
        if inp[0] == "None": pass
        elif inp[0] == "Edit": 
            current = root
            gui.set_state("edit")
        elif inp[0] == "Board": 
            coord1 = (inp[1],inp[2])
            inp = gui.wait_input()
            if inp[0] == "Board":
                coord2 = (inp[1],inp[2])
                if not (coord1[0] == coord2[0] and coord1[1] == coord2[1]):
                    new_state = new_boardstate(current.boardstate,coord1,coord2)
                    current = Node(current,new_state)
    elif gui.state == "edit":
        if inp[0] == "None": print("outside")
        elif inp[0] == "Save & Quit":
            save_nodes(root) 
            gui.state = "board_menu"
        elif inp[0] == "Undo":
            print("Yeah, that button doesn't work yet...")
        elif inp[0] == "Back":
            if current.parent != None:
                current = current.parent
        elif inp[0] == "Delete Line":
            print("Yeah, that button doesn't work yet...")
        elif inp[0] == "Board":
            coord1 = (inp[1],inp[2])
            inp = gui.wait_input()
            if inp[0] == "Board":
                coord2 = (inp[1],inp[2])
                if not (coord1[0] == coord2[0] and coord1[1] == coord2[1]):
                    new_state = new_boardstate(current.boardstate,coord1,coord2)
                    current = Node(current,new_state)
        elif inp[0] == "Child":
            current = inp[1]
            print("inp:" + str(inp[1]))
        else: print("error")
