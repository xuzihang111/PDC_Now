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

#define TASK 2
#define TEMP_TASK 0


TYPE_PACK dat_buf;		//������Ϣ�Ľṹ��
TYPE_MODBUS xdata modbus_struct_buf;

char xdata UART1_recv_buf[300];	//���ջ���
char xdata modbus_buf[255];
extern char xdata EEPROM_buf[512];//EEPROM
char xdata UART1_Timeout;
int xdata ModBus_resend_count;
int xdata Set_temperature = 65;
char Temp_out = 0;
char Start_Test_Conest = 0;
	
uint UATR1_buf_count;				//���ջ������
bit UATR1_Allows_Rec = 1;			//֡ͷ���������־
bit UATR1_rece_flag;				//���ձ�־
bit UATR2_rece_flag;				//���ձ�־
unsigned char xdata modbus_count = 0;

int xdata TimeBase_Buff[TASK];

void Delay1000ms();
void DataDarsing(void);
void TheQuery(char addr,int reg,char len);
void ASK_ok(int cmd);
void Init();
void TemperatureDetection();
void Test();


void main()
{
	Init();

	while(1)
	{
		WDT_CONTR |= 0X10;			//ι��
		
		stat_1 = 0;					//����ָʾ��
		
		DataDarsing();				//���ݰ�����
		
		if(TimeBase_Buff[TEMP_TASK] >= 400)
		{
			TimeBase_Buff[TEMP_TASK] = 0;
			TemperatureDetection();		//�¶ȼ��
		}
		if(!key)					//���� & �������
		{	
			Test();
		}
	}
}








void Uart1() interrupt 4						//����1�����ж�
{
	static uint UART1_rece_len;					//������̬���������ڴ洢Ӧ���յ����ֽ���
	if (RI)
	{
		stat_3 = 0;
		RI = 0;         						//���S2RIλ		
		if(SBUF == 0X7E && UATR1_Allows_Rec)	//�жϵ�֡ͷ�����ݴ������UATR1_Allows_RecҪ��1
		{ 
			UATR1_buf_count = 0;				//���������־
			UATR1_Allows_Rec = 0;				//���������־��ձ�־
		}
		
		UART1_recv_buf[UATR1_buf_count] = SBUF;	//�����յ����ַ��洢

		if(UATR1_buf_count == 7)				//���յ��˰���������
		{
			UART1_rece_len = UART1_recv_buf[6] << 8 | UART1_recv_buf[7];	//�ϳɰ���������
			if(UART1_rece_len >= 299)
			{
				UART1_rece_len = 0;
				UATR1_Allows_Rec = 1;
				UATR1_rece_flag = 0;
			}
		}
		if(UATR1_buf_count == UART1_rece_len - 1)//�������ݽ������
		{
			UART1_Timeout = 0;
			UATR1_rece_flag = 1;				//��UART1_recv_buf������ɺ�Ҫ��������
			stat_3 = 1;		
		}
		UATR1_buf_count++;		
	}
}	

void Uart2() interrupt 8
{
	static uchar UART2_recv_len;				//������̬���������ڴ洢Ӧ���յ����ֽ���
	
	if (S2CON & 0x01)
	{
		S2CON &= ~0x01;         				//���S2RIλ
		ModBus_resend_count = 0;
		
		
		modbus_buf[modbus_count] = S2BUF; 
		
		if(modbus_count == 2)
		{
			UART2_recv_len = modbus_buf[2];
		}
		
		if(modbus_count == UART2_recv_len + 4)
		{
//			UART2_Timeout = 0;
			UATR2_rece_flag = 1;
		}
		
		modbus_count++;
	}
}

void t0int() interrupt 1	//2.5ms ʱ����ʱ
{
	char i;
	UART1_Timeout++;
	ModBus_resend_count++;
	if(UART1_Timeout >= 100)
	{
		UATR1_Allows_Rec = 1;
	}
	

	for(i = 0; i < TASK; i++)
	{
		TimeBase_Buff[i]++;
	}
	
}


void Delay1000ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 92;
	j = 50;
	k = 238;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void DataDarsing(void)
{
	if(UATR1_rece_flag)
	{
		dat_buf = UnpackAgreement(UART1_recv_buf);	//�������ݰ�������������浽dat_buf��
		/*
		*����Ϊ��������ʱ�Ĵ���
		*/
		if(dat_buf.error == -1)
		{
			stat_2 = !stat_2;
			switch (dat_buf.com)
			{
				case 0xff00: TheQuery(1,0x0017,6);break;
				case 0xff01: TheQuery(1,0x0023,6);break;
				case 0xff02: TheQuery(1,0x0029,6);break;
				case 0xff03: TheQuery(1,0x0031,6);break;
				case 0xff04: TheQuery(1,0x0039,6);break;
				case 0xff05: TheQuery(1,0x0041,6);break;
				case 0xff06: TheQuery(1,0x0049,6);break;
				case 0xff07: TheQuery(1,0x004f,2);break;
				
				case 0xff10: relay_1 = 0;					//Զ�̶ϵ�
							  ASK_ok(0xff10);break;
				
				case 0xff70: SendTemperature();break;		//��ȡ��ǰ�¶�
				case 0xff71: SendSetTemperature();break;	//��ȡ�����¶�

				case 0xff72: EEPROM_buf[0] = dat_buf.dat[0];WriteSection(TEMPERATURE_ADDRESS, EEPROM_buf);
							 ASK_ok(0xff72);break;			//���ı����¶�

				case 0xff80: PCON |= 0X10;ASK_ok(0xff80);IAP_CONTR = 0X20;break;	//��λ
				
				
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
//				case 0xff00: TheQuery(1,0x0017,6);break;
				
				default: ERROR_CLASS(2);		//ָ�����

			}
//			

		}
		/*
		*���´���������ݽ����쳣ʱ�Ĵ���
		*/
		else							//������յ������ݰ�����
		{
			if(dat_buf.error == 6)	//crc����δͨ��
			{
				ERROR_CLASS(1);

			}
			else
			{
				ERROR_CLASS(3);

			}
		}
		UATR1_Allows_Rec = 1;	//����1 ���������־
		UATR1_rece_flag = 0;	//����1 ������ɱ�־
	}
}


void TheQuery(char addr,int reg,char len)
{
	Send2String(MakeModbus(addr,0x03,reg,len),8);

	ModBus_resend_count = 0;
	while(!UATR2_rece_flag)		//MODBUS�ط�
	{
		if(ModBus_resend_count >= 100)
		{
			Send2String(MakeModbus(addr,0x03,reg,len),8);
			ModBus_resend_count = 0;
//			bell = 0;
			error_4 = !error_4;
		}
	}
	bell = 1;
	modbus_struct_buf = ModbusParsing(modbus_buf);
	Send1Data(0X00);
	Send1Data(0X01);
	Send1Data(0X17);
	Send1String(AgreementPackaging(ADDRESS,NUMBER,0xff03, modbus_struct_buf.len + 10 , 
									modbus_struct_buf.dat),modbus_struct_buf.len + 10);

	UATR2_rece_flag = 0;
	modbus_count = 0;
}

void ASK_ok(int cmd)
{
	Send1Data(0X00);
	Send1Data(0X01);
	Send1Data(0X17);
	Send1String(AgreementPackaging(ADDRESS,NUMBER,cmd, 12 , 
									"OK"),12);
}

void Init()
{
	P2M0 = 0XFF;
	P2M1 = 0X00;
	P0M0 = 0X07;
	P0M1 = 0X00;
	P4M0 = 0XFF;
	P4M1 = 0X00;
	WDT_CONTR = 0x07;		//���Ź�256��Ƶ
	WDT_CONTR |= 0X20;		//�������Ź�
	Uart1Init();			//115200bps@24.000MHz		ͨѶЭ������뷢��
	Uart2Init();			//9600bps@24.000MHz		modbusЭ���շ�	
	Timer3Init();
	Timer0Init();
	relay_1 = 1;
	stat_1 = 1;
	
	ReadTemperature();
	Calculate_temperature();
	
	if(WDT_CONTR & 0x80)
	{
		error_4 = 0;
		WDT_CONTR &= ~0x80;
	}
	
	if(!(PCON & 0X10))		//POFλ���ж��Ƿ�Ϊ���Ź���λ
	{
		ERROR_CLASS(5);
	}
	PCON &= ~0X10;
	
	
	
	ReadSection(TEMPERATURE_ADDRESS, EEPROM_buf);
	if(EEPROM_buf[0] < 0 || EEPROM_buf[0] > 100)
	{
		Set_temperature = 55;
	}
	else
	{
		Set_temperature = EEPROM_buf[0];
	}
	
	EA = 1;

	Send2String(MakeModbus(0x01,0x03,0x0017,6),8);//��ABC�����ѹ��Чֵ
	ModBus_resend_count = 0;
	while(!UATR2_rece_flag)
	{
		if(ModBus_resend_count >= 100)
		{
			Send2String(MakeModbus(0x01,0x03,0x0017,6),8);
			ModBus_resend_count = 0;
			Start_Test_Conest++;
			WDT_CONTR |= 0X10;			//ι��
		}
		if(Start_Test_Conest >= 50)
		{
			bibi(200,500,20);
			while(1);
		}
	}
	
	bibi(400,0,1);
}

void TemperatureDetection()
{
	char last,now;
	ReadTemperature();
	last = (char)Calculate_temperature();
	if(last > Set_temperature)
	{
		Delay1000ms();
		now = (char)Calculate_temperature();
		if((now > Set_temperature) && ((now - last) < 5))
		{
			Temp_out++;
			if(Temp_out > 10)
			{
				while(key)
				{
					ERROR_CLASS(4);
					WDT_CONTR |= 0X10;	//ι��
					bibi(200,0,1);
					Delay1000ms();
				}
			}
		}
		else
		{
			Temp_out = 0;
		}
		
	}
}
	
void Test()
{
	Send2String(MakeModbus(0x01,0x03,0x0017,6),8);//��ABC�����ѹ��Чֵ
	stat_4 = 0;
	ModBus_resend_count = 0;
	while(!UATR2_rece_flag)		//MODBUS�ط�
	{
		if(ModBus_resend_count >= 100)
		{
			Send2String(MakeModbus(0x01,0x03,0x0017,6),8);
			ModBus_resend_count = 0;
		}
	}
	
	stat_4 = 1;
	modbus_struct_buf = ModbusParsing(modbus_buf);
	Send1Data(0X00);
	Send1Data(0X01);
	Send1Data(0X17);

	Send1String(AgreementPackaging(ADDRESS,NUMBER,0xff03, modbus_struct_buf.len + 10, 
									modbus_struct_buf.dat),modbus_struct_buf.len + 10);
	
	while(!key);
	UATR2_rece_flag = 0;
	modbus_count = 0;
	
	bibi(50,50,2);
	ERROR_CLASS(0);
	PCON |= 0X10;	//������Ź���λ��־
}
















