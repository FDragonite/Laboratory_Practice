#include "../Inc/init.h"

void GPIO_Init_Memory(void){
    *(uint32_t*)(0x40023800UL + 0x30UL) |= 0x06UL; //Включение тактирования порта GPIOB и GPIOC

    *(uint32_t*)(0x40020400UL + 0x00UL) |= 0x10004001UL; //Настройка работы 0-го, 7-го и 14-го пинов GPIOB в режиме вывода сигнала
    *(uint32_t*)(0x40020400UL + 0x04UL) &= ~0x4081UL; //Настройка на PushPull работу 0-го, 7-го и 14-го пинов GPIOB
    *(uint32_t*)(0x40020400UL + 0x08UL) |= 0x10004001UL; //Настройка скорости работы 0-го, 7-го и 14-го пинов GPIOB на среднюю
    *(uint32_t*)(0x40020400UL + 0x0CUL) &= ~0x3000C003UL; //Отключение PU/PD резисторов для 0-го, 7-го и 14-го пинов GPIOB

    *(uint32_t*)(0x40020400UL + 0x00UL) &= ~0x04UL; //Настройка работы 1-го пина GPIOB в режиме ввода сигнала
    *(uint32_t*)(0x40020400UL + 0x04UL) &= ~0x02UL; //Настройка на PushPull работу 1-го пина GPIOB
    *(uint32_t*)(0x40020400UL + 0x08UL) |= 0x04UL; //Настройка скорости работы 1 пина GPIOB на среднюю
    *(uint32_t*)(0x40020400UL + 0x0CUL) &= ~0x0CUL; //Отключение PU/PD резисторов для 1 пина GPIOB

    *(uint32_t*)(0x40020400UL + 0x18UL) |= 0x4081; //Установка единицы в 0-ой, 7-ой и 14-ый биты регистра BSRR
}

void GPIO_Init_Macros(void){
    SET_BIT(RCC_GPIO_EN, RCC_GPIOB_EN | RCC_GPIOC_EN);

    SET_BIT(GPIOB_MODER, GPIOB_PIN0_OUT | GPIOB_PIN7_OUT | GPIOB_PIN14_OUT);
    RESET_BIT(GPIOB_OTYPER, GPIOB_OTYPE_PIN0_OD | GPIOB_OTYPE_PIN1_OD |
                            GPIOB_OTYPE_PIN7_OD | GPIOB_OTYPE_PIN14_OD);
    SET_BIT(GPIOB_OSPEEDR, GPIOB_PIN0_MID | GPIOB_PIN7_MID | GPIOB_PIN14_MID);
    RESET_BIT(GPIOB_PUPDR, GPIOB_PUPDR_PIN0_NOPUPD_RESET | GPIOB_PUPDR_PIN1_NOPUPD_RESET | 
                           GPIOB_PUPDR_PIN7_NOPUPD_RESET | GPIOB_PUPDR_PIN14_NOPUPD_RESET);

    SET_BIT(GPIOB_BSRR, GPIOB_BSRR_PIN0_SET | GPIOB_BSRR_PIN7_SET | GPIOB_BSRR_PIN14_SET);
}

void delay_ml(uint32_t ml) {
    for(volatile uint32_t i = 0; i < ml * 16000; i++) {
        __NOP();
    }
}

//память выделяется последовательно 
//ИО - компилятор должен рассматривать поотдельности, волатад считывает заново, заходит в память, может её читать, менять