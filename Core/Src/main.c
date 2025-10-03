//#include <stdint.h>
#include "../Inc/init.h"

int main(void){
    GPIO_Init_Myself_Macros();
    GPIO_Init_CMSIS();

    while(1){
        SET_BIT(GPIOB->BSRR,GPIO_BSRR_BS_14); //включение светодиода 
        if (BIT_READ(GPIOB_IDR,GPIOB_PIN13)){ //считываем данные с вывода РС13,к нему подключаем
            BIT_SET(GPIOB_BSRR,GPIOB_PIN7_SET); //включение светодиода, регистр BSRR, бит BR7
        }
        else
        {
            BIT_SET(GPIOB_BSRR,GPIOB_PIN7_RESERT); //выключение светодиода, регистр BSRR, бит BR7
        }
            }
    }