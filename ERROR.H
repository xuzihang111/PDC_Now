#ifndef __ERROR_H__
#define __ERROR_H__

#define uint unsigned int
#define uchar unsigned char 

/***************�쳣ָ���*************
0	���쳣/�쳣���
1	crcУ���쳣
2	ָ�����
3	����ͨ�����쳣
4	����
5	���Ź���λ
********************************/
#define	ERROR_CLASS(a)		P2 |= 0X0F;\
							P2 &= ~(0XF0 + a);P2 |= 0XF0

sbit error_1 = P2^0;	
sbit error_2 = P2^1;	
sbit error_3 = P2^2;	
sbit error_4 = P2^3;	

sbit stat_1 = P2^4;		//�ϵ�ָʾ
sbit stat_2 = P2^5;		//��������
sbit stat_3 = P2^6;		//����ָʾ
sbit stat_4 = P2^7;		//���Ա�־

sbit bell = P4^5;		//������
sbit key = P3^3;		//���Լ�

sbit relay_1 = P0^2;	//�̵���1
sbit relay_2 = P0^1;	//�̵���2
sbit relay_3 = P0^0;	//�̵���3

void Timer3Init(void);		//1����@24.000MHz
void Timer0Init(void);		//2500΢��@24.000MHz
void bibi(uint on, uint off, int num);
void Delay100ms();		//@24.000MHz

#endif