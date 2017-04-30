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
#ifndef _SUPERIO_H
#define _SUPERIO_H

int write_gpio(unsigned int gpiopin_no, unsigned int direction, unsigned int level);
int read_gpio(unsigned int gpiopin_no);

#endif

