#include "pH_ADC.h"
void pH_ADCx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(pH_ADC_RCC,ENABLE);
	 
	GPIO_InitStructure.GPIO_Pin = pH_ADC_GPIO_Pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //设置为模拟输入
	GPIO_Init(pH_ADC_PORT, &GPIO_InitStructure); 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC时钟 一般不超过14MHZ 否则精度不准确  72MHZ/6=12MHZ
	ADC_DeInit(pH_ADCX);  //复位时钟
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC 工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode =  DISABLE; //AD 单通道模式;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //AD 单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign =  ADC_DataAlign_Right; //ADC 数据右对齐;
	ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的 ADC 通道的数目 1
	ADC_Init(pH_ADCX, &ADC_InitStructure); 
	
	ADC_Cmd(pH_ADCX,ENABLE);  //使能ADC1
	
	ADC_ResetCalibration(pH_ADCX);//重置指定的 ADC 的校准寄存器
	ADC_StartCalibration(pH_ADCX); //开始指定校准状态
	
	while(ADC_GetResetCalibrationStatus(pH_ADCX));   //等待复位校准结束
	while(ADC_GetCalibrationStatus(pH_ADCX));   //等待校 AD 准结束

}

uint16_t Get_ADC(uint8_t ch)//获取ADC
{	
	ADC_RegularChannelConfig(pH_ADCX,ch,1,ADC_SampleTime_239Cycles5); //设定ADC规则组	
	ADC_SoftwareStartConvCmd(pH_ADCX,ENABLE);  //使能软件启动
	while(!ADC_GetFlagStatus(pH_ADCX, ADC_FLAG_EOC ));//等待转换结束
	return 	ADC_GetConversionValue(pH_ADCX);  //返回最后一次规则装换结果
}



