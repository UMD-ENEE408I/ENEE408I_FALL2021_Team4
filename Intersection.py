

class Intersection(object):
    turns =[0]*3
    distance = 0
    left = None
    straight = None
    right = None
    prev = None
    dead = False
    finished = False
    location = [0]*2

    def __init__(self, left, straight, right, location, finished):
        self.turns = [left, straight, right]
        self.location = location;
        self.left = None
        self.right = None
        self.straight = None
        self.prev = None
        if finished == True:
            self.finished=True
        #if [left, straight, right] == [0, 0, 0]:
            #self.dead = True


    
    def set_left(self, intersect):
        self.left = intersect
        intersect.prev = self
    def set_straight(self, intersect):
        self.straight = intersect
        intersect.prev = self
    def set_right(self, intersect):
        self.right = intersect
        intersect.prev = self
    def set_node_dead(self):
        self.finished = True

class Mouse(object):
    mouse_number
    curr_node
    prev_node = None

    def __init__(self, mouse, root):
        self.mouse_number = mouse
        self.curr_node = root
    
    def update_node(self, node):
        self.prev_node = self.curr_node
        self.curr_node = node
    
    def turnLeft(self):
        if self.curr_node.prev == self.prev_node:
            #turn Left
            return

    def turnStraight(self):
        if self.curr_node.prev == self.prev_node:
            #goStraight
            return
        elif self.prev_node == self.curr_node.left:
            #turnLeft
            return
    
    def turnRight(self):
        if self.curr_node.prev == self.prev_node:
            #turn right
            return
        elif self.prev_node == self.curr_node.left:
            #go straight
            return
        elif self.prev_node == self.curr_node.straight:
            #turn left
            return




def make_intersection(left, straight, right, location, finished):
    intersect = Intersection(left, straight, right, location, finished)
    return intersect

def set_node_dead(curr):
    curr.dead = True

def create_node_from_mouse(mouse_number):
    #Link to emily's code 
    #int=make_intersection(l, s, r, d, f)
    return #int


#def add_next(curr, dir, new_intersection, distance):
#        curr.left = make_intersection(new_intersection(1), new_intersection(2), new_intersection(3), distance)
 #   elif dir == 2:
 #       curr.straight = make_intersection(new_intersection(1), new_intersection(2), new_intersection(3), distance)
  #  else:
 #       curr.right = make_intersection(new_intersection(1), new_intersection(2), new_intersection(3), distance)

def return_to_alive(node, mouse):
    if node.dead == False:
        return
    else:
        if node == node.prev.left:
            #turn right and drive to next node
            mouse.update_node(node.prev)
            return_to_alive(node.prev, mouse)
        elif node == node.prev.straight:
            #drive straight to next node
            mouse.update_node(node.prev)
            return_to_alive(node.prev, mouse)
        else:
            #turn left and drive to next node
            mouse.update_node(node.prev)
            return_to_alive(node.prev, mouse)

def check_children(curr):
    if curr.turns(1)==1 & curr.left.dead == False:
        #turn_left(mouse_number)
        #drive forward to next node(mouse_num)
        #wait for mouse to idle
        #acquire data from mouse and create new intersection
        #set left (curr, intersect)
        check_children(curr.left)
        if curr.finished == True:
            curr.prev.finished = True
            return 
    if curr.turns(2)==1 & curr.straight.dead == False:
        #drive straight to next node
        check_children(curr.straight)
        if curr.finished == True:
            curr.prev.finished = True
            return
    if curr.turns(3)==1 & curr.right.dead == False:
        #turn_right
        #drive forward to next node
        check_children(curr.right)
        if curr.finished == True:
            curr.prev.finished = True
            return
    set_node_dead(curr)
    #U-Turn mouse and return to previous node

#Create First Intersection from mouse
root  = getIntersection
mouse1 = Mouse(1, root)




