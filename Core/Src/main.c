
#include "../Inc/init.h"
int main(void) {
    PIO_Init_Memory();

    uint32_t button_press_count = 0; //счётчик нажатий на вторую кнопку
    uint32_t past_button_state_1 = 0; //была ли кнопка 1 нажата до текущего момента
    uint32_t past_button_state_2 = 0; //была ли кнопка 2 нажата до текущего момента
    uint32_t active_leds = 3;

    while(1){
        if((*(uint32_t*)(0x40020800UL + 0x10UL) & 0x2000UL) != 0){ //нажата ли кнопка 2
            delay_us(1); //задержка в 1 мл
            if(((*(uint32_t*)(0x40020800UL + 0x10UL) & 0x2000UL) != 0) && past_button_state_2 == 0){ //нажата ли кнопка 2 и была ли не нажата ранее
                past_button_state_2 = 1;
            }
        }
        else{
            if(past_button_state_2 == 1){ //кнопка 2 была нажата, а сейчас нет
                button_press_count++;
                past_button_state_2 = 0;
            }
        }
        if(button_press_count == 5){ //счётчик 5 нажатий
            button_press_count = 0;
            if((*(uint32_t*)(0x40020400UL + 0x00UL) & 0x04UL) != 0){
                *(uint32_t*)(0x40020400UL + 0x00UL) &= ~0x04UL; //изменение режима работы PB1 на ввод
            }
            else{
                *(uint32_t*)(0x40020400UL + 0x00UL) |= 0x04UL; //изменение режима работы PB1 на вывод
            }
        }
        if((*(uint32_t*)(0x40020400UL + 0x10UL) & 0x02UL) != 0){ //кнопка 1 нажата?
            delay_us(1); //задержка в 1 мл
            if(((*(uint32_t*)(0x40020400UL + 0x10UL) & 0x02UL) != 0) && past_button_state_1 == 0){
                past_button_state_1 = 1;
            }
        }
        else{
            if(past_button_state_1 == 1){ //кнопка 1 была нажата, а сейчас нет
                past_button_state_1 = 0;
                switch(active_leds){
                    case 0:
                        *(uint32_t*)(0x40020400UL + 0x18UL) |= 0x01; //Установка единицы в 0-ой бит регистра BSRR
                        active_leds++;
                        break;
                    case 1:
                        *(uint32_t*)(0x40020400UL + 0x18UL) |= 0x80; //Установка единицы в 7-ой бит регистра BSRR
                        active_leds++;
                        break;
                    case 2:
                        *(uint32_t*)(0x40020400UL + 0x18UL) |= 0x4000; //Установка единицы в 14-ой бит регистра BSRR
                        active_leds++;
                        break;
                    case 3:
                        *(uint32_t*)(0x40020400UL + 0x18UL) |= 0x40810000; //Установка нуля в 0-ой, 7-ой и 14-ый биты регистра BSRR
                        active_leds = 0;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
