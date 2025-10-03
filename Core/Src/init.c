#include "../Inc/init.h"

void GPIO_Init_Memory(void){
     *(uint32_t*)(0x40023800UL + 0x30UL) |= 0x06UL; //Включение тактирования порта GPIOB

    *(uint32_t*)(0x40020400UL+0x00UL) |= 0x4000UL; //Настройка работы 7-го пина GPIOB в режиме вывода сигнала
    *(uint32_t*)(0x40020400UL+0x04UL) |= 0x00UL; //Настройка на PushPull работу 7-го пина GPIOB
    *(uint32_t*)(0x40020400UL+0x08UL) |= 0x4000UL; //Настройка скорости работы 7-го пина GPIOB на среднюю
    *(uint32_t*)(0x40020400UL+0x18UL) |= 0x800000UL; //Отключение PU/PDрезисторов для 7-го пина GPIOB предварительное включение светодиода
}

void GPIO_Init_Myself_Macros(void){
    RCC_AHB1ENR |= RCC_GPIOB_EN | RCC_GPIOC_EN;

    BIT_SET(GPIOB_MODER, GPIOB_PIN7_OUT);
    BIT_SET(GPIOB_OSPEEDR, GPIOB_PIN7_MED);
    BIT_SET(GPIOB_BSRR, GPIOB_PIN7_RESERT);
}

void GPIO_Init_CMSIS(void){
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); //AHB1ENR-ПОЛЕ СТРУКТУРЫ,  RCC_AHB1ENR_GPIOBEN-адрес 

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE14_0); // включение красного светодиода, настройка пина PB14 на вывод
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT14); //
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR14_0); //
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14); //
}











//память выделяется последовательно 
//ИО - компилятор должен рассматривать поотдельности, волатад считывает заново, заходит в память, может её читать, менять