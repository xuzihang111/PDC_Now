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


void Timer3Init(void)		//1����@24.000MHz
{
	T4T3M |= 0x02;		//��ʱ��ʱ��1Tģʽ
	T3L = 0x40;		//���ö�ʱ��ֵ
	T3H = 0xA2;		//���ö�ʱ��ֵ
	T4T3M |= 0x08;		//��ʱ��3��ʼ��ʱ
}

void Timer0Init(void)		//2500΢��@24.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xA0;		//���ö�ʱ��ֵ
	TH0 = 0x15;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
}


void bibi(uint on, uint off, int num)
{
    IE2 |= 0x20;        //����ʱ��2�ж�
	cycle = on + off;
	high_level = on;
	close = num - 1;
}

void t3int() interrupt 19           //�ж����
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





