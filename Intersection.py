import bluetooth as comm

STRAIGHT =  1
LEFT = 2
RIGHT = 3
UTURN = 4

NORTH = 1
EAST = 2
WEST = 4
SOUTH = 3


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
    mouse_number = 0
    orientation = NORTH
    curr_node = None
    prev_node = None

    def __init__(self, mouse, root):
        self.mouse_number = mouse
        self.curr_node = root
        self.orientation = NORTH
    
    def update_node(self, node):
        self.prev_node = self.curr_node
        self.curr_node = node
    
    def turnLeft(self):
        if self.curr_node.prev == self.prev_node:
            data = comm.send_instr(LEFT)
            return data

    def turnStraight(self):
        if self.curr_node.prev == self.prev_node:
            data = comm.send_instr(STRAIGHT)
            return data
        elif self.prev_node == self.curr_node.left:
            data = comm.send_instr(LEFT)
            return data
    
    def turnRight(self):
        if self.curr_node.prev == self.prev_node:
            data = comm.send_instr(RIGHT)
            return data
        elif self.prev_node == self.curr_node.left:
            data = comm.send_instr(STRAIGHT)
            return data
            
        elif self.prev_node == self.curr_node.straight:
            data = comm.send_instr(LEFT)
            return data

def updateLocation(location, mouse, dist, turnDir):
    if turnDir == LEFT:
        mouse.orientation = (mouse.orientation - 1)%4
    elif turnDir == RIGHT:
        mouse.orientation = (mouse.orientation + 1)%4
    if mouse.orientation == NORTH:
        location[0] += dist
    if mouse.orientation == EAST:
        location[1] += dist
    if mouse.orientation == WEST:
        location[1] -= dist
    if mouse.orientation == SOUTH:
        location[0] -= dist
    return location





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
            comm.send_instr(RIGHT)
            mouse.update_node(node.prev)
            return_to_alive(node.prev, mouse)
        elif node == node.prev.straight:
            #drive straight to next node
            comm.send_instr(STRAIGHT)
            mouse.update_node(node.prev)
            return_to_alive(node.prev, mouse)
        else:
            #turn left and drive to next node
            comm.send(LEFT)
            mouse.update_node(node.prev)
            return_to_alive(node.prev, mouse)

def check_children(curr, mouse):
    
    if curr.turns(1)==1 & curr.left.dead == False:
        
        #turn_left and drive to next node(mouse_number)
        [dir, dist, k] = mouse.turnLeft()

        finished = dir & 0b1000
        left = dir & 0b0100
        straight = dir & 0b0010
        right = dir & 0b0001

        location = updateLocation(curr.location, mouse, dist, LEFT)
        #acquire data from mouse and create new intersection
        intersect = Intersection(left, straight, right, location, finished)
        curr.setLeft(intersect)
        
        

        mouse.updateNode(curr.left)
        check_children(curr.left)
         
        if curr.finished == True:
            curr.prev.finished = True
            return 

    if curr.turns(2)==1 & curr.straight.dead == False:
        #turn_left and drive to next node(mouse_number)
        [dir, dist, k] = mouse.turnStraight()

        finished = dir & 0b1000
        left = dir & 0b0100
        straight = dir & 0b0010
        right = dir & 0b0001

        location = updateLocation(curr.location, mouse, dist, STRAIGHT)
        #acquire data from mouse and create new intersection
        intersect = Intersection(left, straight, right, location, finished)
        curr.setStraight(intersect)
        
        mouse.updateNode(curr.straight)
        check_children(curr.straight)

        if curr.finished == True:
            curr.prev.finished = True
            return
        
    if curr.turns(3)==1 & curr.right.dead == False:
        #turn_left and drive to next node(mouse_number)
        [dir, dist, k] = mouse.turnRight()

        finished = dir & 0b1000
        left = dir & 0b0100
        straight = dir & 0b0010
        right = dir & 0b0001

        location = updateLocation(curr.location, mouse, dist, RIGHT)
        #acquire data from mouse and create new intersection
        intersect = Intersection(left, straight, right, location, finished)
        curr.setRight(intersect)
        
        mouse.updateNode(curr.right)
        check_children(curr.right)
        
        if curr.finished == True:
            curr.prev.finished = True
            return

    set_node_dead(curr)
    comm.send_instr(UTURN)
    return_to_alive(curr, mouse)

#Create First Intersection from mouse
# [dir, dist, k] = comm.send_instr(STRAIGHT)

# finished = dir & 0b1000
# left = dir & 0b0100
# straight = dir & 0b0010
# right = dir & 0b0001
# location  = [0, 0]

root = Intersection(0, 1, 0, [0,0], 0)
mouse = Mouse(1, root)
check_children(root, mouse)




