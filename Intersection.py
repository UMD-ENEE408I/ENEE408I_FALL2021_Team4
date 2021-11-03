class Intersection(object):
    turns =[0]*3
    distance = 0
    left = None
    straight = None
    right = None
    prev = None
    dead = False
    finished = False

    def __init__(self, left, straight, right, distance, finished):
        self.turns = [left, straight, right]
        self.distance = distance;
        self.left = None
        self.right = None
        self.straight = None
        self.prev = None
        if finished == True:
            self.finished=True
        if [left, straight, right] == [0, 0, 0]:
            self.dead = True


    
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

    

def make_intersection(left, straight, right, distance, finished):
    intersect = Intersection(left, straight, right, distance, finished)
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





