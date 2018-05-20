#include <STC15F2K60S2.H>
#include <ProtocolParsing.H>
#include <MODBUS.H>
#include <UART.H>
#include <intrins.h>
#include <ERROR.h>
#include <18B20.H>
#include <EEPROM.H>
#include <stdio.H>
#include <RESPONSE.H>

extern int xdata Set_temperature;


void SendTemperature()
{
	char xdata T_buf[5];
	ReadTemperature();
	sprintf(T_buf,"%0.2f",	Calculate_temperature());
	
	Send1Data(0X00);
	Send1Data(0X01);
	Send1Data(0X17);
	Send1String(AgreementPackaging(ADDRESS,NUMBER,0xff70, 15 , 
									T_buf),15);
}

void SendSetTemperature()
{
	char xdata T_buf[5];
	ReadTemperature();
	sprintf(T_buf,"%d",	Set_temperature);
	
	Send1Data(0X00);
	Send1Data(0X01);
	Send1Data(0X17);
	Send1String(AgreementPackaging(ADDRESS,NUMBER,0xff71, 12 , 
									T_buf),12);
}



