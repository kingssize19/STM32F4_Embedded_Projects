#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef	   		GPIO_InitStruct;
ADC_InitTypeDef		    	ADC_InitStruct;
ADC_CommonInitTypeDef		ADC_CommonInitStruct;

uint16_t adc_value = 0;
float adc_voltage = 0;

void GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;			//Analog Mode
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void ADC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;				//Bağımsız mode
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;			//90MHz(max) / Divx < 36MHz

	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;					//12bitlik çözünürlük

	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);
}

uint16_t Read_ADC()
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);

	ADC_SoftwareStartConv(ADC1);					//ADC1 Kanalını yazılımsal olarak başlat.

	while (!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)));

	return ADC_GetConversionValue(ADC1);

}


int main(void)
{
  GPIO_Config();
  ADC_Config();
  while (1)
  {
	  adc_value = Read_ADC();
	  adc_voltage = (0.000805) * adc_value;
	  if (adc_value > 1500)
	  {
		  GPIO_SetBits(GPIOA, GPIO_Pin_5);
	  }

	  else if (adc_value <= 1500)
	  {
		  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	  }
  }
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
