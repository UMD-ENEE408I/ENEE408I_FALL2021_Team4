import Intersection
import matplotlib.pyplot as plt
import numpy as np
import cv2

# root = Intersection.Intersection(0, 1, 0, [0,0], 0)
# A = Intersection.Intersection(1, 0, 1, [0,6], 0)
# B = Intersection.Intersection(0, 1, 1, [-2,6], 0)
# C = Intersection.Intersection(0, 0, 0, [-4,6], 0)
# D = Intersection.Intersection(0, 0, 0, [-2,8], 0)
# E = Intersection.Intersection(1, 1, 0, [3,6], 0)
# F = Intersection.Intersection(0, 1, 1, [3,8], 0)
# G = Intersection.Intersection(0, 0, 0, [3,9], 0)
# H = Intersection.Intersection(0, 0, 0, [4,8], 0)
# I = Intersection.Intersection(0, 1, 1, [6,6], 0)
# J = Intersection.Intersection(0, 0, 0, [8,6], 1)
# K = Intersection.Intersection(1, 1, 0, [6,2], 0)
# L = Intersection.Intersection(0, 0, 0, [6,0], 0)
# M = Intersection.Intersection(0, 0, 0, [8,2], 0)

#MAZE DEMO
# root = Intersection.Intersection(0, 1, 0, [750,600], 0)
# A = Intersection.Intersection(1, 0, 1, [750,480], 0)
# B = Intersection.Intersection(0, 1, 1, [650,480], 0)
# C = Intersection.Intersection(0, 0, 0, [550,480], 0)
# D = Intersection.Intersection(0, 0, 0, [650,440], 0)
# E = Intersection.Intersection(1, 1, 0, [900,480], 0)
# F = Intersection.Intersection(0, 1, 1, [900,440], 0)
# G = Intersection.Intersection(0, 0, 0, [900,420], 0)
# H = Intersection.Intersection(0, 0, 0, [950,440], 0)
# I = Intersection.Intersection(0, 1, 1, [1050,480], 0)
# J = Intersection.Intersection(0, 0, 0, [1150,480], 1)
# K = Intersection.Intersection(1, 1, 0, [1050,560], 0)
# L = Intersection.Intersection(0, 0, 0, [1050,600], 0)
# M = Intersection.Intersection(0, 0, 0, [1150,560], 0)

# root.set_straight(A)
# A.set_left(B)
# B.set_straight(C)
# B.set_right(D)
# A.set_right(E)
# E.set_left(F)
# F.set_straight(G)
# F.set_right(H)
# E.set_straight(I)
# I.set_straight(J)
# I.set_right(K)
# K.set_left(L)
# K.set_straight(M)


#MAZE1
root = Intersection.Intersection(0, 1, 0, [1250,625], 0)
A = Intersection.Intersection(1, 0, 0, [1250,550], 0)
B = Intersection.Intersection(0, 0, 1, [900,550], 0)
C = Intersection.Intersection(1, 1, 1, [900,475], 0)
D = Intersection.Intersection(1, 0, 1, [725,475], 0)
E = Intersection.Intersection(0, 0, 1, [900,400], 0)
F = Intersection.Intersection(0, 0, 1, [1025,400], 0)
G = Intersection.Intersection(0, 0, 0, [1025,475], 0)
H = Intersection.Intersection(0, 0, 0, [725,550], 0)
I = Intersection.Intersection(1, 1, 0, [725,400], 0)
J = Intersection.Intersection(1, 0, 0, [650,400], 0)
K = Intersection.Intersection(1, 1, 0, [725,325], 0)
L = Intersection.Intersection(0, 1, 1, [650,475], 0)
M = Intersection.Intersection(0, 1, 1, [650,550], 0)
N = Intersection.Intersection(0, 1, 1, [575,475], 0)
O = Intersection.Intersection(0, 1, 1, [575,550], 0)
P = Intersection.Intersection(1, 0, 0, [575,325], 0)
Q = Intersection.Intersection(1, 0, 0, [575,400], 0)
R = Intersection.Intersection(1, 1, 0, [1025,325], 0)
S = Intersection.Intersection(1, 0, 0, [1025,250], 0)
T = Intersection.Intersection(1, 1, 0, [1250,325], 0)
U = Intersection.Intersection(1, 0, 0, [800,250], 1)
V = Intersection.Intersection(1, 1, 0, [1250,400], 0)

root.set_straight(A)
A.set_left(B)
B.set_right(C)
C.set_left(D)
D.set_left(H)
D.set_right(I)
I.set_left(J)
J.set_left(L)
L.set_straight(M)
L.set_right(N)
N.set_left(O)
I.set_straight(K)
K.set_left(P)
P.set_left(Q)
K.set_right(R)
R.set_left(S)
S.set_left(U)
R.set_straight(T)
T.set_right(V)
C.set_straight(E)
E.set_right(F)
C.set_right(G)

#MAZE 2
# root = Intersection.Intersection(0, 1, 0, [200,625], 0)
# A = Intersection.Intersection(0, 0, 1, [200,550], 0)
# B = Intersection.Intersection(1, 0, 0, [400,550], 0)
# C = Intersection.Intersection(1, 0, 0, [400,500], 0)
# D =  Intersection.Intersection(0, 0, 1, [333,500], 0)
# E = Intersection.Intersection(1, 0, 1, [333,375], 0)
# F = Intersection.Intersection(0, 1, 1, [266,375], 0)
# G = Intersection.Intersection(1, 0, 1, [200,375], 0)
# H = Intersection.Intersection(1, 1, 0, [200,450], 0)
# I = Intersection.Intersection(0, 0, 0, [266,450], 0)
# J = Intersection.Intersection(0, 0, 0, [200,500], 0)
# K = Intersection.Intersection(0, 0, 0, [200,350], 0)
# L = Intersection.Intersection(0, 0, 1, [266,275], 0)
# M = Intersection.Intersection(1, 0, 0, [600,275], 0)
# N = Intersection.Intersection(0, 0, 0, [600,250], 1)


# root.set_straight(A)
# A.set_right(B)
# B.set_left(C)
# C.set_left(D)
# D.set_right(E)
# E.set_left(F)
# F.set_straight(G)
# G.set_left(H)
# H.set_left(I)
# H.set_straight(J)
# G.set_right(K)
# F.set_right(L)
# L.set_right(M)
# M.set_left(N)





# plot_xvalues = np.array([0,0,-2,-4,-2,3,3,3,4,6,8,6,6,8])
# plot_yvalues = np.array([0,6,6,6,8,6,8,9,8,6,6,2,0,2])
# plt.scatter(plot_xvalues, plot_yvalues)

# plt.show()
# plot_xvals = np.array([])
# plot_yvals = np.array([])
# plot_xvals = np.append(plot_xvals, [A.location[0]])
# plot_yvals = np.append(plot_yvals, [A.location[1]])

def create_map_from_tree(root):
    img = cv2.imread('plain black.jpg', 1)
    img = add_node_to_plot(root, img)
    cv2.imshow('image', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    #fig = plt.figure()
    #plt.scatter(plot_xvals, plot_yvals)
    #plt.show()
    

def add_node_to_plot(node, img):
    # x = np.array([node.location[0]])
    # y = np.array([node.location[1]])
    if node.finished:
        #plt.scatter(x, y, color = 'red')
        img = cv2.circle(img, node.location, 5, (0, 0, 255), -1)
        if node.prev:
            img = cv2.line(img, node.location, node.prev.location, (0,0,255), 1)
        cv2.imshow('image', img)
        cv2.waitKey(800)
    else:    
        #plt.scatter(x, y, color = 'blue') 
        img = cv2.circle(img, node.location, 5, (255, 0, 0), -1) 
        if node.prev:
            img = cv2.line(img, node.location, node.prev.location, (255,0,0), 1)
        cv2.imshow('image', img)
        cv2.waitKey(800) 
    if node.left:
        add_node_to_plot(node.left, img)
    if node.straight:
        add_node_to_plot(node.straight, img)
    if node.right:
        add_node_to_plot(node.right, img)
    
    if node.finished:
        if node.prev:
            img = cv2.line(img, node.location, node.prev.location, (0,0,255), 1)
            cv2.imshow('image', img)
            cv2.waitKey(800)
        node.prev.finished = True
    
    return img

    
    


create_map_from_tree(root)






