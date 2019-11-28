#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 

//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}	//PB11����ģʽ
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;} //PB11���ģʽ
//IO��������	 
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //SDA 
#define LDC_ADDR   PAin(0)  //FDC��ַѡ��//Ӳ����·J7���̲�����Ĭ�ϵ�ַΪ�ߵ�ƽ
#define LDC_SD   	PAin(1)  //FDC�ض� 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź� 

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

void FDC_write_16bit(u8 regaddr,u16 data);	//дdata���Ĵ�����regaddrΪ�Ĵ����ĵ�ַ
u16 FDC_read_16bit(u8 regaddr);	//���Ĵ���regaddr��ֵ��regaddrΪ�Ĵ����ĵ�ַ

#endif


















