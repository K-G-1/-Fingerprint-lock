#include "timer.h"
#include "key.h"
#include "include.h"
#include "lcd.h"
#include "beep.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

extern vu16 USART2_RX_STA;

//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART2_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
	}	    
}
 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}


//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM3_Int_Init(u16 arr,u16 psc)
{	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��

    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
    TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3

    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}


extern u8 use_machine;
extern u8 display_manual ;
extern u8 beep_count ;
extern u8 control_door_flag;
static u8 times_count_1 = 0;
static u16 times_count_2 = 0;
static u16 times_count_3 = 0;
extern u8 manual_cmd;
//��ʱ��3�жϷ������		    
void TIM3_IRQHandler(void)
{ 	
    u8 key_value ;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 	
		   
        times_count_1++;
        
        if(times_count_1 %10 ==0)
        {
            key_value = 17;
            key_value = keypress();
        }
        if(display_manual == 0)
        {
            not_use_manual(key_value);
        }
        else if (display_manual == 1)
        {
            use_manual(key_value);
        }
        
        if(times_count_1>=100)//100*10 ==1000ms
        {
            if(beep_count >= 3)
            {
                times_count_3 ++;
            }
            times_count_1 = 0;
        }
//���������ʱ        
        if(control_door_flag != 0)
        {
            times_count_2++;
        }
        if(times_count_2 >= 500&&times_count_2< 1000)
        {
            control_door_flag = 2;
        }
        else if(times_count_2>= 1000)
        {
            times_count_2=0;
            control_door_flag = 0;
        }
//������ʱ
        if(times_count_3 >=3)
        {
            warn(0);
            beep_count =0;
            times_count_3 = 0;
        }
        else if(times_count_3 > 0)
        {
            warn(1);
        }
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIM7�����жϱ�־    
	}	    
}


/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:����ʾ�˵�ʱ�������п��Ų���
manual_cmd:         1 :���������
                    2��������ָ��
                    11����ʽ�޸�����
                    12����ʽ����ָ��
*@Author		:K.G. 
****************************************************/
void use_manual(u8 key_value)
{
    static u8 num = 0;
    if(times_count_1 %10 == 0)
    {
        if(key_value != 17)
        {
            
            if(key_value<10&&num<4&&manual_cmd >= 10)                 //��������
            {
                key_input[num] = key_value+0x30;
                num++;
            }
            else if(key_value == 10&&manual_cmd >= 10)                //ɾ������                
            {
                num--;
                key_input[num]=' ';           
            }
            else if(key_value == 1&&manual_cmd <=10)             
            {
                    manual_cmd = 1;                   
            }
            else if(key_value == 2&&manual_cmd <=10)
            {
                    manual_cmd = 2;
            }
            else if(key_value == 3&&manual_cmd <=10)
            {
                    manual_cmd = 3;
            }
            else if(key_value == 4&&manual_cmd <=10)
            {
                    manual_cmd = 4;
            }
            else if(key_value == 5&&manual_cmd <=10)
            {
                    manual_cmd = 5;
            }
            else if (key_value == 11)
            {
                num = 0;
                
                
                if(manual_cmd == 1)
                {
                    manual_cmd = 11;
   
                }
                else if(manual_cmd ==2 )
                {
                    manual_cmd = 12;
                }
                else if(manual_cmd ==3 )
                {
                    manual_cmd = 13;
                }
                else if(manual_cmd ==4 )
                {
                    manual_cmd = 14;
                }
                else if(manual_cmd ==5 )
                {
                    manual_cmd = 15;
                }
                else if (manual_cmd >10)
                {
                    enter_press = 1;
                }
                
                else {
                    
                }
            }
            else if (key_value == 13)
            {
                display_manual = 0;
            }
            
            if(num!= 0 )
            {
                
                LCD_ShowString(60,130,24,12,12,key_input);
            }
            else 
                LCD_Fill(0,130,128,160,WHITE);
            LCD_ShowxNum(0,130,key_value,2,12,0);
        }
        
        
    }
        
}

/***************************************************
*@version		:V1.0
*@CreatDate		:2018/5/5
*@Description	:������ʾ�˵�ʱ����Ҫ���п��Ų�����ģʽת������
*@Author		:K.G. 
****************************************************/
void not_use_manual(u8 key_value)
{
    if(times_count_1 %10 ==0)
        {
            if(use_machine == 1)
            {
                KeyOpen(key_value);
            }else {
                ChangeMode(key_value);
            }
        }
        
        
            
        
    
}


