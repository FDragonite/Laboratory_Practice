#ifndef INIT_H
#define INIT_H


#include <stm32f4xx.h>
#include <stm32f429xx.h>


#define RCC_AHB1ENR (*(uint32_t*)(0x40023800UL + 0x30UL))
#define RCC_GPIOB_EN 0x02UL
#define RCC_GPIOC_EN 0x04UL

#define GPIOB_MODER (*(uint32_t*)(0x40020400UL + 0x00UL))
#define GPIOB_OTYPER (*(uint32_t*)(0x40020400UL + 0x04UL))
#define GPIOB_OSPEEDR (*(uint32_t*)(0x40020400UL + 0x08UL))
#define GPIOB_BSRR (*(uint32_t*)(0x40020400UL + 0x18UL))

#define GPIOB_IDR (*(uint32_t*)(0x40020800UL + 0x10UL))

#define GPIOB_PIN7_OUT      0x4000UL
#define GPIOB_OFF           0x00UL
#define GPIOB_PIN7_MED      0x4000UL
#define GPIOB_PIN7_RESERT   0x800000UL
#define GPIOB_PIN7_SET      0x80UL
#define GPIOB_PIN13         0x2000UL

#define BIT_READ(REG, BIT) (REG & BIT)
#define BIT_SET(REG, BIT) (REG |= BIT)

void GPIO_Init_Memory(void);
void GPIO_Init_Myself_Macros(void);
void GPIO_Init_CMSIS(void);  //gpio порты включаются с помощью библиотеки смсис




#endif