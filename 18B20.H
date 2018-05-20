#ifndef __18B20_H__
#define __18B20_H__

#define uchar unsigned char
#define uint  unsigned int

sbit DS=P0^4;

#define Read_ROM    		0X33
#define Conform_ROM 		0X55
#define Search_ROM  		0Xf0
#define Skip_ROM    		0Xcc
#define The_alarm   		0Xec
#define Read_temperature 	0X44
#define Read_memory 		0Xbe
#define write_memory 		0X55

#define WRITE_MODE 1
#define READ_MODE  0


void ReadTemperature();
float Calculate_temperature();

#endif

