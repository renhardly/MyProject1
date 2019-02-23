import numpy as np
import cv2
from matplotlib import pyplot as plt

def draw_circle(event,x,y,flags,param):

    if event == cv2.EVENT_LBUTTONDBLCLK:
        print(x,y)

        
# Load an color image in grayscale
img = cv2.imread('./Pages/Tuner_main.png',0)
cv2.imshow('image',img)
run = 1
cv2.setMouseCallback('image',draw_circle)
while run == 1:
    k=cv2.waitKey(0)
    print(k)
    if k == 27:         # wait for ESC key to exit
        cv2.destroyAllWindows()
        run = 0
    elif k == ord('n'): # wait for 's' key to save and exit
        img1 = cv2.imread('./Pages/Tuner_Setting.png',0)
        cv2.imshow('image',img1)
    elif k == ord('t'): # wait for 's' key to save and exit
        img2 = cv2.imread('./Pages/Tuner_tuning.png',0)
        cv2.imshow('image',img2)

