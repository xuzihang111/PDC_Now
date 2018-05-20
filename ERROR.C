#include <STC15F2K60S2.H>
#include <ERROR.H>
#include <INTRINS.H>

uint count = 0;
uint cycle,high_level;
int close = 0, close_count = 0;
//extern char xdata UART1_Timeout, UART2_Timeout;


//void Delay100ms()		//@24.000MHz
//{
//	unsigned char i, j, k;

//	_nop_();
//	_nop_();
//	i = 10;
//	j = 31;
//	k = 147;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}


void Timer3Init(void)		//1毫秒@24.000MHz
{
	T4T3M |= 0x02;		//定时器时钟1T模式
	T3L = 0x40;		//设置定时初值
	T3H = 0xA2;		//设置定时初值
	T4T3M |= 0x08;		//定时器3开始计时
}

void Timer0Init(void)		//2500微秒@24.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xA0;		//设置定时初值
	TH0 = 0x15;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}


void bibi(uint on, uint off, int num)
{
    IE2 |= 0x20;        //开定时器2中断
	cycle = on + off;
	high_level = on;
	close = num - 1;
}

void t3int() interrupt 19           //中断入口
{
	count++;
	if(count < high_level)
	{
		bell = 0;
	}
	else
	{
		bell = 1;
	}
	if(count > cycle)
	{
		count = 0;
		if(close <= close_count && close != -1)
		{
			IE2 &= ~0x20;
			close_count = 0;
			
		}
		else
		{
			close_count++;
		}
	}
}






