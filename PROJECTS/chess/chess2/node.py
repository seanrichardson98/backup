class Node:

    def __init__(self,parent,boardstate):
        self.children = []
        self.parent = parent
        self.boardstate = boardstate
        self.num = None
        if parent != None:
            parent.add_child(self)
            self.num = parent.num + 1
        else: self.num = 0
#print("parent:" + str(parent))

    def add_child(self,child):
        self.children.append(child)

    def delete_child(self,child):
        self.children.remove(child)

    def delete(self):
        self.parent.delete_child(self)
        return parent

    def get_boardstate(self):
        return boardstate

    def get_child(self,i):
        return self.children[i]

    def set_num(num):
        self.num = num

    def get_num():
        return self.num

    def get_children(self):
        ret = "["
        for c in self.children:
            ret += str(c) + "\t"
        ret += "]"
        return ret
