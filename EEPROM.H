#ifndef	__EEPROM_H
#define	__EEPROM_H

#include <STC15F2K60S2.H>
#include <intrins.H>

#define CMD_IDLE    0               //空闲模式
#define CMD_READ    1               //IAP字节读命令
#define CMD_PROGRAM 2               //IAP字节编程命令
#define CMD_ERASE   3               //IAP扇区擦除命令

//#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
//#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz

#define IAP_ADDRESS 0x4000

#define TEMPERATURE_ADDRESS 0X4000
#define OTHER_ADDRESS 0X4200


typedef unsigned char BYTE;
typedef unsigned int WORD;

#define uchar unsigned char
#define uint  unsigned int
#define ulong  unsigned long


void WriteSection(uint Addr,uchar *Buf);
void ReadSection(uint Addr,uchar *Buf);







#endif

