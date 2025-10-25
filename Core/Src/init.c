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

void RCC_Init(void){ /* Предварительная очистка регистров RCC */ 
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
    CLEAR_REG(RCC->CFGR); 
    while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET); 
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON); 
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET); 
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON); 
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET); 
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); /* Настройка главного регистра RCC */ 
    SET_BIT(RCC->CR, RCC_CR_HSEON); //Запускаем внешний кварцевый резонатор 
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET); //Ждём пока он запустится
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); //Сбросим бит байпаса в 0, если вдруг там что-то лежит 
    SET_BIT(RCC->CR, RCC_CR_CSSON); //Запустим Clock detector 
    /* Настройка регистров PLL 
    * Предварительная очистка регистра 
    * В качестве источника тактирования для PLL выбирается HSE 
    * Мы сначала делим входную частоту (HSE) на 4 (получаем 2 МГц), затем умножаем на 180 и снова делим на 2, 
    таким образом получаем 180МГц 
    * Включаем работу PLL */ 
    CLEAR_REG(RCC->PLLCFGR); 
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE); 
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_2); //Выставляем предделитель входной частоты PLL на 4 
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_4 | 
                             RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7); 
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk); //Настраиваем предделитель получившейся частоты после умножения. 
    // Иными словами, получаем итоговую частоту PLL 
    SET_BIT(RCC->CR, RCC_CR_PLLON); //Запустим PLL 
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY)); //Ждём запуска PLL 
    /* Настройка основных конфигураций RCC 
    * В качетсве системных часов выбираем выход PLL 
    * Настраиваем предделители шин AHB и APB 
    * Настраиваем выходы MCO1 и MCO2 для внешней оценки настроенной системы тактирования */ 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL); //Выбираем PLL в качестве System Clock 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1); //Предделитель AHB, без делителя 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4); //Предделитель APВ1, делим на 4 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2); //Предделитель APВ2, делим на 2 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO2PRE, RCC_CFGR_MCO2PRE_Msk); //Предделитель на выходе MCO2 (PC9) = 5 
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2); //Настраиваем на выход MCO2 - System clock 
    /* Настройка задержки внутренней памяти 
    * Выставление битов LATENCY регистра FLASH_ACR в позицию 5SW (6 CPU cycles). 
    * Данная настройка необходима при увеличении системной частоты тактирования свыше 20 МГц. Таблица */
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS); }










//память выделяется последовательно 
//ИО - компилятор должен рассматривать поотдельности, волатад считывает заново, заходит в память, может её читать, менять