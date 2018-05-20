#include <STC15F2K60S2.H>
#include <INTRINS.H>
#include <STRING.H>		//strlen()	取字符串长度，直接返回数字值
#include <math.h>		
#include <18B20.H>

void Delay15us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 87;
	while (--i);
}


void Delay30us()		//@24.000MHz
{
	unsigned char i, j;

	i = 1;
	j = 176;
	do
	{
		while (--j);
	} while (--i);
}

void Delay60us()		//@24.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 99;
	do
	{
		while (--j);
	} while (--i);
}

void Delay470us()		//@24.000MHz
{
	unsigned char i, j;

	i = 11;
	j = 246;
	do
	{
		while (--j);
	} while (--i);
}

void Delay750us()		//@24.000MHz
{
	unsigned char i, j;

	i = 18;
	j = 127;
	do
	{
		while (--j);
	} while (--i);
}


void Star18b20()
{
	DS=1;
	Delay15us();
	DS=0;
	Delay750us();
	DS=1;
	Delay470us();
}

void write_data(uchar dat)
{
	uchar i;
	bit flag;
	for(i=0;i<8;i++)
	{
		DS=0;
		_nop_();
		flag=dat&0x01;
		DS=flag;
		Delay60us();
		DS=1;
		dat=dat>>1;
	}
}

uchar read_data()
{
	uchar i,dat=0;
	for(i=0;i<8;i++)
	{
		DS=0;
		_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();
		dat>>=1;
		DS=1;
		_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();
		if(DS) 	dat|=0x80;
		Delay30us();
	}
	return (dat);
}
void ReadTemperature()
{
	Star18b20();
	Delay30us();
	write_data(Skip_ROM);
	write_data(Read_temperature);											  
}

float Calculate_temperature()
{
	int temp;
	float f_temp;
	uchar H,L;
	Star18b20();
	write_data(Skip_ROM); 
	write_data(Read_memory);
	H=read_data();
	L=read_data();
	temp = L;
	temp <<= 8;
	temp = temp|H;
	f_temp = temp*0.0625;
	return f_temp;
}



















