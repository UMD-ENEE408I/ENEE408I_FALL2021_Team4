import cv2
import time

img = cv2.imread('plain black.jpg', 1)



img = cv2.circle(img, (750,600), 5, (0, 0, 255), -1)
cv2.imshow('image', img)
cv2.waitKey(800)
img = cv2.line(img, [750, 600], (750, 550), (0,0,255), 1)
cv2.imshow('image', img)
cv2.waitKey(800)
img = cv2.line(img, (750, 600), (750, 550), (255,0,0), 1)


cv2.imshow('image', img)
cv2.waitKey(800)

img = cv2.circle(img, (750,550), 5, (0, 0, 255), -1)
cv2.imshow('image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()