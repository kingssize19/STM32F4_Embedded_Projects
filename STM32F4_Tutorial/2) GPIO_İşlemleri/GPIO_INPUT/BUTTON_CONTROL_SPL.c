#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;

void GPIO_Config(void)
{

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);



    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);



    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void delay(uint32_t time)
{
    while (time--);
}

int main(void)
{
    GPIO_Config();

    while (1)
    {

        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
        {
            delay(50000);
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_5);
            }
            while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
        }


        else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
        {
            delay(50000);
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_5);
            }
            while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4));
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
