#ifndef __ERROR_H__
#define __ERROR_H__

#define uint unsigned int
#define uchar unsigned char 

/***************异常指令表*************
0	无异常/异常清除
1	crc校验异常
2	指令不存在
3	其他通信类异常
4	过热
5	看门狗复位
********************************/
#define	ERROR_CLASS(a)		P2 |= 0X0F;\
							P2 &= ~(0XF0 + a);P2 |= 0XF0

sbit error_1 = P2^0;	
sbit error_2 = P2^1;	
sbit error_3 = P2^2;	
sbit error_4 = P2^3;	

sbit stat_1 = P2^4;		//上电指示
sbit stat_2 = P2^5;		//接收正常
sbit stat_3 = P2^6;		//接收指示
sbit stat_4 = P2^7;		//测试标志

sbit bell = P4^5;		//蜂鸣器
sbit key = P3^3;		//测试键

sbit relay_1 = P0^2;	//继电器1
sbit relay_2 = P0^1;	//继电器2
sbit relay_3 = P0^0;	//继电器3

void Timer3Init(void);		//1毫秒@24.000MHz
void Timer0Init(void);		//2500微秒@24.000MHz
void bibi(uint on, uint off, int num);
void Delay100ms();		//@24.000MHz

#endif
