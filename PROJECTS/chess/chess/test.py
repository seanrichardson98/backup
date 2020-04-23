from boardstate import *
from gui import *
from Node import *
from encoder import *


b = BoardState(1)

gui = GUI()
gui.draw_board()
gui.draw_position(b)

root = Node(None,b)
current = root

for i in range(5):

#print(current.boardstate)
    print("current" + str(current))
    print("parent"+ str(current.parent))

    clicks = gui.wait_input()

    new_state = new_boardstate(current.boardstate,clicks[0],clicks[1])
    new_node = Node(current,new_state)
    current = new_node


    gui.draw_board()
    gui.draw_position(current.boardstate)

#print("------------")
#print(base)
#print(base.get_children())
#print(base.get_child(0))
#print(base.get_child(0).get_children())
#print("------------")
#
#current = base
#gui.draw_board()
#gui.draw_position(current.boardstate)
#print(base)
#gui.win.getMouse()
#move1 = base.get_child(0)
#gui.draw_board()
#gui.draw_position(current.boardstate)
#print(move1)
#gui.win.getMouse()
#move2 = move1.get_child(0)
#gui.draw_board()
#gui.draw_position(current.boardstate)
#print(move2)

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

save_nodes(root)
read_nodes()

input("")
