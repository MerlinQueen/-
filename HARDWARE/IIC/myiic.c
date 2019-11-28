#include "myiic.h"
#include "delay.h"
#include "FDC2X2X_CMD.h"


//��ʼ��IIC
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOE, ENABLE );
  //GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 �����
	
	  //GPIOA0,A1��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	IIC_SCL=1;
	IIC_SDA=1;
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(4);
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u16 ucErrTime=0;//u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(4);//delay_us(1);	   
	IIC_SCL=1;delay_us(4);//delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)//if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(4);//delay_us(2);
	IIC_SCL=1;
	delay_us(4);//delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(4);//delay_us(2);
	IIC_SCL=1;
	delay_us(4);//delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(4);//delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(4);//delay_us(2); 
		IIC_SCL=0;	
		delay_us(4);//delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
	delay_us(4);
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(4);//delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(4);//delay_us(2);//delay_us(1); 
    }					 
    if (ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}





void FDC_write_16bit(u8 regaddr,u16 data)	//дdata���Ĵ�����regaddrΪ�Ĵ����ĵ�ַ
{
	u16 data_temp = 0;
	u16 data_temp1 = 0;
	u8 Ack1,Ack2,Ack3,Ack4;
	Ack1=Ack2=Ack3=Ack4=2;
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_WRITE);	//�����豸��ַ��д
	Ack1 = IIC_Wait_Ack();
	
	IIC_Send_Byte(regaddr);	//�Ĵ�����ַ
	Ack2 = IIC_Wait_Ack();
	
	IIC_Send_Byte(data>>8);	//���͸߰�λ
	Ack3 = IIC_Wait_Ack();
	
	IIC_Send_Byte(data&0xff);	//���͵Ͱ�λ
	Ack4 = IIC_Wait_Ack();
	
	IIC_Stop();//����һ��ֹͣ����


}



u16 FDC_read_16bit(u8 regaddr)	//���Ĵ���regaddr��ֵ��regaddrΪ�Ĵ����ĵ�ַ
{
	u16 data_temp = 0;
	u16 data_temp1 = 0;
	u8 Ack1,Ack2,Ack3,Ack4;
	Ack1=Ack2=Ack3=Ack4=2;
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_WRITE);	//�����豸��ַ��д
	Ack1 = IIC_Wait_Ack();
	IIC_Send_Byte(regaddr);	//�Ĵ�����ַ
	Ack2 = IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_READ);//�����豸��ַ����
	Ack3 = IIC_Wait_Ack();
	data_temp1 = IIC_Read_Byte(0);//IIC��ȡ��λ�ֽ�
	data_temp = IIC_Read_Byte(0);//IIC��ȡ��λ�ֽ�
	IIC_Stop();//����һ��ֹͣ����
	
	//printf (" Ack1 %d\r\n Ack2 %d\r\n Ack3 %d, \r\n",Ack1,Ack2,Ack3);
	//printf ("data_temp1 %d,data_temp %d \r\n",data_temp1,data_temp);
	
	data_temp = data_temp + (data_temp1<<8) ;	//���ݺϲ�
	
	//printf("%d \r\n",data_temp); //���ڷ��Ϳ��������
	return data_temp;



}























