#include "FDC2214.h"
#include "myiic.h"
#include "delay.h"
#include "FDC2X2X_CMD.h"
#include "usart.h" 

 u32 data_Fdc2214_H;
 u32 data_Fdc2214_L;
extern uint16_t buf[4];
float FDC2214_read_CHx(u8 CHx)	//��Channel x ת�����
{
	u8 reg_addr1 = 0;	//MSB��ַ
	u8 reg_addr = 0;	//LSB��ַ
	u32 data_H = 0;			//���յ�������
	u32 data1_L = 0; 
	float   conver = 0;   //���յ�ת���Ľ��
	switch(CHx)
	{
		case 0:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH0;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH0;	
		}break;
		case 1:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH1;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH1;
		}break;
		case 2:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH2;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH2;
		}break;
		case 3:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH3;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH3;
		}break;
		default: return 0;	//�������
		
	}
	
	data_H = FDC_read_16bit(reg_addr1);	//��ȡ�Ĵ���ֵ  ����λ  
	data1_L = FDC_read_16bit(reg_addr);	//��ȡ�Ĵ���ֵ   ����λ   

	conver = ( ((data_H & 0x0FFF)<<16)|(data1_L) ); 	//�ںϽ�� 
	//return conver;
	data_Fdc2214_H = data_H;
	data_Fdc2214_L = data1_L;
	
//***ע��Init�и�ͨ���Ĵ���0x10-0x17��ʱ�ӷ�Ƶ***//

	conver = (conver * 40000000.0F)/268435456.0F;//�õ�F(sensorx)
	
	conver = conver * 2.0F *3.1415926F;
	conver = 1.0F/((conver * conver) * 0.000018F);
	conver = conver - 0.000000000033F;
	conver = conver * 1000000000000.0F;//Fת��pF;
	
	
	return conver;
		


}
void FDC2214_Init(void)	//��ʼ��FDC2214��I2C
{
	IIC_Init();							//IIC��ʼ��
	LDC_ADDR = 0;	//LDC��7bit��ַΪ0X2A��ע����7bit �� 0101 010_  ���λΪ��дλ
	LDC_SD = 0;		//�˳�Shutdown Mode
	delay_ms(500);	//��ʱ500ms
	
		//����ʱ��Ҫ��100 SPS������Ĳο������ֱ���Ҫ��
	FDC_write_16bit(LDC13xx16xx_CMD_REF_COUNT_CH0,0Xffff);
	FDC_write_16bit(LDC13xx16xx_CMD_REF_COUNT_CH1,0Xffff);
	FDC_write_16bit(LDC13xx16xx_CMD_REF_COUNT_CH2,0Xffff);
	FDC_write_16bit(LDC13xx16xx_CMD_REF_COUNT_CH3,0Xffff);
	
	FDC_write_16bit(LDC13xx16xx_CMD_SETTLE_COUNT_CH0,0X0400);
	FDC_write_16bit(LDC13xx16xx_CMD_SETTLE_COUNT_CH1,0X0400);
	FDC_write_16bit(LDC13xx16xx_CMD_SETTLE_COUNT_CH2,0X0400);
	FDC_write_16bit(LDC13xx16xx_CMD_SETTLE_COUNT_CH3,0X0400);
	
	FDC_write_16bit(LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH0,0X1001);
	FDC_write_16bit(LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH1,0X1001);
	FDC_write_16bit(LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH2,0X1001);
	FDC_write_16bit(LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH3,0X1001);
	
	FDC_write_16bit(LDC13xx16xx_CMD_ERROR_CONFIG,0X0001);
	
	FDC_write_16bit(LDC13xx16xx_CMD_MUX_CONFIG,0xC20D);
	
	FDC_write_16bit(LDC13xx16xx_CMD_DRIVE_CURRENT_CH0,0x8c40);
	FDC_write_16bit(LDC13xx16xx_CMD_DRIVE_CURRENT_CH1,0x8c40);
	FDC_write_16bit(LDC13xx16xx_CMD_DRIVE_CURRENT_CH2,0x8c40);
	FDC_write_16bit(LDC13xx16xx_CMD_DRIVE_CURRENT_CH3,0x8c40);

	FDC_write_16bit(LDC13xx16xx_CMD_CONFIG,0x1701);		//bit13 ���� �˳�˯��ģʽ
	


}





