#include "pH_ADC.h"
void pH_ADCx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(pH_ADC_RCC,ENABLE);
	 
	GPIO_InitStructure.GPIO_Pin = pH_ADC_GPIO_Pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //����Ϊģ������
	GPIO_Init(pH_ADC_PORT, &GPIO_InitStructure); 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADCʱ�� һ�㲻����14MHZ ���򾫶Ȳ�׼ȷ  72MHZ/6=12MHZ
	ADC_DeInit(pH_ADCX);  //��λʱ��
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC ����ģʽ:����ģʽ
	ADC_InitStructure.ADC_ScanConvMode =  DISABLE; //AD ��ͨ��ģʽ;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //AD ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign =  ADC_DataAlign_Right; //ADC �����Ҷ���;
	ADC_InitStructure.ADC_NbrOfChannel = 1; //˳����й���ת���� ADC ͨ������Ŀ 1
	ADC_Init(pH_ADCX, &ADC_InitStructure); 
	
	ADC_Cmd(pH_ADCX,ENABLE);  //ʹ��ADC1
	
	ADC_ResetCalibration(pH_ADCX);//����ָ���� ADC ��У׼�Ĵ���
	ADC_StartCalibration(pH_ADCX); //��ʼָ��У׼״̬
	
	while(ADC_GetResetCalibrationStatus(pH_ADCX));   //�ȴ���λУ׼����
	while(ADC_GetCalibrationStatus(pH_ADCX));   //�ȴ�У AD ׼����

}

uint16_t Get_ADC(uint8_t ch)//��ȡADC
{	
	ADC_RegularChannelConfig(pH_ADCX,ch,1,ADC_SampleTime_239Cycles5); //�趨ADC������	
	ADC_SoftwareStartConvCmd(pH_ADCX,ENABLE);  //ʹ���������
	while(!ADC_GetFlagStatus(pH_ADCX, ADC_FLAG_EOC ));//�ȴ�ת������
	return 	ADC_GetConversionValue(pH_ADCX);  //�������һ�ι���װ�����
}



