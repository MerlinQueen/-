#include "mytime.h"
#include "sys.h"
	

void Time_Init(u16 yfp,u16 tz)  //yfp Ԥ��Ƶ     tz ���ֵ
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitType;
    NVIC_InitTypeDef NVIC_InitType;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    
    TIM_TimeBaseInitType.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitType.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitType.TIM_Period=tz;
    TIM_TimeBaseInitType.TIM_Prescaler=yfp;
     
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitType);
    
    TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
                );
	NVIC_InitType.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitType.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitType.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitType.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitType);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����

}


