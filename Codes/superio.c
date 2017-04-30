/*
 * =====================================================================================
 *
 *       Filename:  controls.c
 *
 *    Description:  provides support for writing to & reading from gpio pins
 *
 *        Version:  1.0
 *        Created:  12/08/12 12:07:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  kln (nitheesh), nitheesh.k.l@intel.com
 *   Organization:  Intel 
 *
 * =====================================================================================
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include <sys/io.h>

unsigned char	IO_PORT_BASE=0x2E;  // DATA_PORT = IO_PORT_BASE + 1;


int search(int);

//This function allows only 1 - 8 gpio pin numbers
int search(int a)
{
  if( a==1 || a==2 || a==3 || a==4 || a==5 || a==6 || a==7 || a==8 ) 
    return 1;
  else
    return 0;
}

// This function sets W83627HF into configuration mode
void enter_config()
{
  //permission to access the port
  if(ioperm(IO_PORT_BASE, 3, 1)) 
  { 
     perror("ioperm"); 
     exit(1);
  }

 // Enter W83627HF Config
  outb(0x87,IO_PORT_BASE);
  outb(0x87,IO_PORT_BASE);

  // Set Multi-function Pins to GPIO 
  outb(0x2C,IO_PORT_BASE);
  outb((inb(IO_PORT_BASE+1) & 0x1F),IO_PORT_BASE+1);

  // Select GPIO Port device
  outb(0x07,IO_PORT_BASE);
  outb(0x09,IO_PORT_BASE+1);

  // Set GPIO Port Active GPIO3
  outb(0x30,IO_PORT_BASE);
  outb(0x02,IO_PORT_BASE+1);

}

//This function reads the values from the given gpio pin number
int read_gpio(unsigned int igpiopin_no)
{
  unsigned int data = 0;
  unsigned int direction = 0;
  unsigned int ilevel = 0;
  unsigned int nrotate  = 0;

  //Set the configuration
  enter_config();

  // Read from CF0 register
  outb(0xF0,IO_PORT_BASE);
  data=inb(IO_PORT_BASE+1);

  direction = (int)((data >> (igpiopin_no-1)) & 0x1);

  if ( direction == 0)
    printf("Read gpio_pin : %d direction: %d Output\n",igpiopin_no,direction);
  else
    printf("Read gpio_pin : %d direction: %d Input\n",igpiopin_no,direction);


  // Read from CF1 register
  outb(0xF1,IO_PORT_BASE);
  data=inb(IO_PORT_BASE+1);
  printf("Read input register  %X\n",data);
  switch (igpiopin_no)
  {
   case 1 :
     nrotate  = 0;
     break;
   case 3 :
     nrotate  = 1;
     break;
   case 5 :
     nrotate  = 2;
     break;
   case 7 :
     nrotate  = 3;
     break;
   case 2 :
     nrotate  = 4;
     break;
   case 4 :
     nrotate  = 5;
     break;
   case 6 :
     nrotate  = 6;
     break;
   case 8 :
     nrotate  = 7;
     break;
  }
  
  ilevel = (int)((data >> nrotate) & 0x1);
  
  //printf("Read gpio_pin : %d signal_level: %d Low\n",igpiopin_no,ilevel);
  if ( ilevel == 0)
    printf("Read gpio_pin : %d signal_level: %d Low\n",igpiopin_no,ilevel);
  else
    printf("Read gpio_pin : %d signal_level: %d High\n",igpiopin_no,ilevel);

  // Exit W83627HF Config
  outb(0xAA,IO_PORT_BASE);
  return ilevel;      // modification done here
}

//This function sets the direction and level of the given gpio pin number 
int write_gpio(unsigned int igpiopin_no,unsigned int idir,unsigned int ilevel)
{
  unsigned int data = 0;
  unsigned int nrotate  = 0;
  
  //Set the configuration
  enter_config();

  //Set W83627HF GPIO30~37 direction to Output or Input
  outb(0xF0,IO_PORT_BASE);
  data=inb(IO_PORT_BASE+1);
 
  if(idir)
  {  
     data |= (1 << (igpiopin_no-1));
  }
  else
  { 
    data &= ~(1 << (igpiopin_no-1));
  }
  // Write the data
  outb(data,IO_PORT_BASE+1);

  // Set W83627HF GPIO30~37 level to Low or high
  outb(0xF1,IO_PORT_BASE);
  data=inb(IO_PORT_BASE+1);
  switch (igpiopin_no)
  {
   case 1 :
     nrotate  = 0;
     break;
   case 3 :
     nrotate  = 1;
     break;
   case 5 :
     nrotate  = 2;
     break;
   case 7 :
     nrotate  = 3;
     break;
   case 2 :
     nrotate  = 4;
     break;
   case 4 :
     nrotate  = 5;
     break;
   case 6 :
     nrotate  = 6;
     break;
   case 8 :
     nrotate  = 7;
     break;
  }
  
  
  //printf("Read gpio_pin : %d signal_level: %d Low\n",igpiopin_no,ilevel);
  if(ilevel)
  {
     data |= (1 << nrotate);
  }
  else
  {
    data &= ~(1 << nrotate);
  }
  printf("Read output register  %X\n",data);
  // Write the data
  outb(data,IO_PORT_BASE+1);  

 // Exit W83627HF Config
  outb(0xAA,IO_PORT_BASE);
}

