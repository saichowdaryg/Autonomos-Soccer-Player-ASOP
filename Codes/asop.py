"""
ASOP project
Course: Embedded Systems EE404, Semester-I 2015
IIT Gandhinagar
Instructor:	Prof. Joycee M. Mekie
TA:		Sneha Ved
Team:		Sai Chowdary (sai_gullapally@iitgn.ac.in)
		Veerabadra Lokesh (veerabadralokesh_paturu@iitgn.ac.in)
		Sidhartha Reddy (sidhartha_reddy@iitgn.ac.in)
"""

# Python code for making a bot to move a green ball into a blue goal post, based on the input from camera, processing the images using opencv libraries on Intel Atom processor
import socket,sys
from time import sleep as sleep
import numpy as np
# without appending the path of the location of opencv libraries, python will not be able to import cv2 libraries
sys.path.append("/usr/local/lib/python2.7/site-packages/")
import cv2

# creating the socket
s = socket.socket()
host='127.0.0.1'
port=9999

# distance of the columns from center of image on both sides which are processed for locating the ball
p = 50
q = 320
# No. of columns in the image to be used for locating the ball
lfb = 3

# Trying to connect to the Motor controlling program
try:
	s.connect((host,port))
except:
	s.close()
	exit()

# Function for sending a command to the motorrunning program to act accordingly based on the input character
def sd(command):
	try:
		s.send(command)
	except:
		exit()


def openhands():
	for i in range(3):
		sd('o')
		sleep(0.02)
		sd('q')
                sleep(0.5)
def closehands():
	for i in range(4):
		sd('c')
		sleep(0.02)
		sd('q')
                sleep(0.5)

# Rotate the bot to left propotionally to the input integer i
def rotateleft(i):
	sd('a')
	sleep(0.04*i)
	sd('q')

# Rotate the bot to right propotionally to the input integer i
def rotateright(i):
	sd('d')
	sleep(0.04*i)
	sd('q')

# Make the bot go straight proportional to the input integer i
def gostraight(i):
	sd('w')
	sleep(0.5*i)
	sd('q')

# Make the bot go in reverse direction proportional to the input integer i
def goback(i):
	sd('s')
	sleep(0.5*i)
	sd('q')

# This funciton make the bot to push the ball into the goal, this is very specific for the current hands being used
def makegoal():
	openhands()
	#gostraight(4)
	goback(10)
	closehands()

def startgame():
        # This function runs first and it waits until the value of certain pixels go below a threshold value
        # It stops once all the pixels of image goes below threshold value and the bot starts searching for the ball
        
        # For starting the video caputre by camera
        cap=cv2.VideoCapture(0)
        
        # Rows and colums of pixels to be checked for threshold
        r = [80,160,240]
        c = [80,160,240]
        
        #Threshold for checking with the pixels
        l=30
        k = min(len(r),len(c))
        while(1):
                n = 0
                # Reading a frame from the camera
                _,frame=cap.read()
                for i in range(k):
                        if frame[c[i],r[i],0]<l:
                                if frame[c[i],r[i],1]<l:
                                        if frame[c[i],r[i],2]<l:
                                                n=n+1
                
                if n==k:
                        cap.release()	# Stop the camera
                        break

def catchball():
        # Function for catching the ball
        #print "catching ball"
        
        cl=1		# Hands are closed
        op=0		# Hands are open
        # Starting the Video Capture
        cap=cv2.VideoCapture(0)
        pl = [0]*lfb
        newframe = np.zeros((480,lfb,3), np.uint8)
        for i in range(0,lfb):
        	# Array of columns that are being processed for calculating the location of ball
		pl[i] = (i-(lfb-1)/2)*p+q
	while(1):
		# Read frame from camera
		_,frame=cap.read()

		for i in range(0,lfb):
			# copying the pixels in required colums from the captured frame to the new frame
			newframe[:,i] = frame[:,pl[i]]

		# Masking the image with the hsv values in a range for detecting green color
		hsv=cv2.cvtColor(newframe,cv2.COLOR_BGR2HSV)
		lower=np.array([45,100,100])
		upper=np.array([75,255,255])
		mask=cv2.inRange(hsv,lower,upper)
	
		n=0
		x=0
		y=0
		c = [0]*lfb
		j=0
		
		# Checking for green in the selected columns
		for l in range(lfb):
			for i in range(0,480):
				if mask[i,l]>0:
					x = x + i
					y = y + pl[l]
					c[j] = 1
					n = n + 1
					break
			j = j + 1
		
		if n==1:# if green is found only in one column of the above selcted colums
		        if x<100:# if ball is far away
		                gostraight(0.1)
		        else:# else rotate towards the direction of the ball location
			        if c[0]==1:
				        rotateleft(1)
			        if c[2]==1:
				        rotateright(1)
		if n==0:# if green is not found int the selected rows
			rotateleft(1)

		if n>=1:# if green is found in more than one row then take the average of the pixel locations
			x = x/n
			y = y/n
			if x<400:# if ball is found and far away open hands and move towards the ball
				if op == 0:
					openhands()
					op = 1
					cl = 0
				sd('w')
			if x>400:# if the ball is close enough catch the ball and stop moving and search for ball function
				sd('q')
				if cl==0:
					closehands()
					cl=1
					op=0
				break
		
		"""
		print x,y
		#Display the input frame
		cv2.imshow('originals',frame)
		# without waitkey cv2.imshow() does'nt work
		k = cv2.waitKey(5) & 0xFF
		if k==27:
			#sd('p')
			break"""
        cap.release()
        # Destroy any cv2 windows if opened; not required here as no image is being displayed using cv2.imshow, below line must be uncommented if cv2.imshow is used
	#cv2.destroyAllWindows()

def bgoal():
        # Function for moving ball to the goal post
        
	cap=cv2.VideoCapture(0)
	newframe = np.zeros((6,640,3), np.uint8)
	while(1):
		_,frame=cap.read()
		
		for i in range(0,6):
			# copying the required rows from the captured frame to the new empty frame for processing
			newframe[i] = frame[i*80]
		
		# Masking the image with the hsv values in a range for detecting blue color
		hsv=cv2.cvtColor(newframe,cv2.COLOR_BGR2HSV)
		lower=np.array([100,100,100])
		upper=np.array([140,255,255])
		mask=cv2.inRange(hsv,lower,upper)
	
		rowl=[]
		coll=[]
		rowr=[]
		colr=[]
		n=0
		
		# Checking from the right side of image in the selected rows for the goal post
		for i in range(0,6):
		        for j in range(0,640):
				if mask[i,639-j]==255:
			        	rowr.append(639-j)
			        	colr.append(i*80)
			        	n=n+1
					break
		if n!=0:
			# Calcuating the average of pixels where blue color was found
			xr=sum(rowr)/n
		m = 0
		# Checking from the left side of image in the selected rows for the goal post
		for i in range(0,6):
		        for j in range(0,640):
				if mask[i,j]==255:
			        	rowl.append(j)
			        	coll.append(i*80)
			        	m=m+1
					break
		if m!=0:
			# Calcuating the average of pixels where blue color was found
			xl=sum(rowl)/m
	
		if n==0:
			# If no blue color was found then rotate left
			rotateleft(1)
			xr=-1
			xl=-1
		
		else:
			# mid point of the left side average and right side average
		        x = (xl+xr)/2
			if(xr-xl<600):#if the goal post is not close
				if(x>360):# if goal post was to the right rotate right
					rotateright(1)
					#sd('d')
				elif(x<280):# if goal post was to the left rotate left
					rotateleft(1)
					#sd('a')
				else:# else go straight
					gostraight(1)
					#sd('w')
			else:# if goalpost is close move the ball into the goal
				makegoal()
				break
		        #print xr-xl,x
		
		"""
		cv2.imshow('goal',frame)
		#cv2.imshow('mask',mask2)
		k = cv2.waitKey(5) & 0xFF
		if k==27:
		        #sd('p')
			break"""
        cap.release()
	#cv2.destroyAllWindows()

while True:
	startgame()
	catchball()
        bgoal()

sd('p')
