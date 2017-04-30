ASOP
Autonomous Soccer Player
Embedded Systems (EE404) Course Project
Instructor      : Prof Joycee Mekie
TA 		: Sneha Ved
Team Members    : Sai Chowdary(12110082)      - sai_gullapally@iitgn.ac.in
	          Sidhartha Reddy(12110096)   - sidhartha_reddy@iitgn.ac.in
	          Veerabadra Lokesh(12110111) - veerabadralokesh_paturu@iitgn.ac.in
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Motor Connections(Wheels and arms):

-->Intel Atom has a total of 10 GPIO pins including a VCC(5 Volts) and GND.Refer "GPIO.pdf" section 3.2 for the layout of GPIO pins on Intel Atom.
-->We have used two L293D's motor drivers.One for controlling wheels(L293D-1) and the other for arms(L293D-2)

     GPIO Layout				Connections
--> 1 		2			Pin 1 		- Enable 1 and Enable 2 of L293D-1 
    3 		4			Pins 2,3 	- I1 and I2 of L293D-1
    5		6  			Pins 4,5 	- I3 and I4 of L293D-1
    7 		8			Pin 6 		- Enable 1 of L293D-2
    9(GND)      10(VCC)			Pins 7,8 	- I1 and I2 of L293D-2
					Pin 9,10 	- '-' and '+' of both L293D-1,L293D-2
					O1 and O2(L293D-1) - Two motors on left connected in parallel
					O3 and O4(L293D-1) - Two motors on right connected in parallel
					O1 and O2(L293D-2) - Two motors on top connected in parallel(for arms)
					VS and GND(L293D-1 & L293D-2) - 12 Volt battery

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

How to start ASOP(automatic):

-->Plug in the 12 Volt power supply to the intel atom and connect the keyboard.Switch on the power supply and press F1 to start Intel Atom and wait for 1 minute to get the OS loaded.
-->We have already programmed the intel atom to run the necessary codes automatically(asop.py and motor_run.c).
-->Now the keyboard can be disconnected and the bot can be placed in the field. 
-->To activate the bot, completely cover the camera with finger and keep it closed until the bot starts moving.If nothing happens try uncovering the camera and covering it again.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

How to start ASOP(manual) :

-->Plug in the 12 Volt power supply to the intel atom and connect the keyboard,monitor.Switch on the power supply and press F1 to start Intel Atom and wait for 1 minute to get the   	 OS loaded.The login password is 'inforce123'.
-->Open the terminal window by pressing 'CNTRL+ALT+T' and navigate to the folder containing all the codes by "cd" command
   ex:- "cd /home/inforce/asop" (actual folder where we have saved all the codes)
-->Login as root user by typing "su" and then entering the above mentioned password('inforce123').
-->First the motor running code must be executed.To compile the code type "gcc motor_run.c superio.c -o motor_run".To execute the code type "./motor_run".
-->Now open a new terminal window and navigate to the folder containing the codes as stated above.To execute the image processing code type "python asop.py".
-->Now the keyboard and monitor can be disconnected and the bot can be placed in the field. 
-->To activate the bot, completely cover the camera with finger and keep it closed until the bot starts moving.If nothing happens try uncovering the camera and covering it again.

