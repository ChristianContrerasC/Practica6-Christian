#include "lib/include.h"
// HOLA by CHRIS
uint16_t Result[2];
uint16_t duty[2];
void Hilo_1(void)
{
 // PUERTOS
    GPIOB->DATA = (1<<0);
    SEQ_CONFIG(Result,duty);
    GPIOB->DATA = (0<<0);
    

}

int main(void)
 unsigned int LECTURAADC; 
 volatile uint16_t freq;
     freq=1250;
uint16_t Result[3];
uint16_t duty[3];
int i;
{

    
    Configurar_UART0();
    Configurar_PWM(50);
    Configurar_Timer0A(&Hilo_1,10000);
    Configurar_PLL(_20MHZ);  
    Configurar_GPIO();
    ADC_CONFIG();
    while(1)
    {
        ADC0 -> PSSI |= (1 << 3);
        while ((ADC0 -> RIS & 8) == 0);
        LECTURAADC = ADC0 -> SSFIFO2&0xFFF; //ENTRADA SALIDA
        ADC0 -> ISC = 8; 
        if (LECTURAADC >= 2048){
            GPIOC -> DATA |= (1 << 1); 
        }
        else if (LECTURAADC < 2048){
            GPIOC -> DATA &= ~(1 << 1); 
        
        }
        SEQ_CONFIG(Result,duty);
        PWM0->_1_CMPA = duty[1];
        // PEQUEÑO DELAY
        delayms(10);
    }

}
