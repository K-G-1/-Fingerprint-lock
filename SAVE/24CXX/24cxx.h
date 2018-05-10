#ifndef __24CXX_H
#define __24CXX_H
#include "IIC.h"   
	
/************************************************/
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

#define EE_TYPE AT24C256
/***************************************************/
#define save_gyro_x          0x30
#define save_gyro_y          0x33
#define save_gyro_z          0x35

#define save_acc_x           0x37
#define save_acc_y           0x3A
#define save_acc_z           0x3C

#define save_5883_x           0x40
#define save_5883_y           0x42
#define save_5883_z           0x44





u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u16 WriteAddr,u32 DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC

void AT24CXX_Write_save_Byte(u8 addr,u32 data);
u32 AT24CXX_Read_save_Byte(u8 addr);
void AT24cxx_save_Acc_Gyro_offest(void);
void AT24cxx_save_5883_offest(void);


void AT24cxx_read_Acc_Gyro_offest(void);
void AT24cxx_read_5883_offest(void);


#endif
















