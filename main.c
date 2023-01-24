#include "lib/include.h"
// HOLA by CHRIS
uint16_t Result[2];
uint16_t duty[2];
void Hilo_1(void)
{
    /*
 //llamada a la conversion por procesador
    PWM0->_1_CMPB = duty[0];
    PWM0->_1_CMPA = duty[1];
    PWM0->_0_CMPA = 1500;    
    */
    GPIOB->DATA = (1<<0);
    ADC0_InSeq2(Result,duty);
    GPIOB->DATA = (0<<0);
    

}

int main(void)
{

    Configurar_PLL();  //Confiuracion de velocidad de reloj 20MHZ
    Configurar_GPIO();
    Configura_Reg_ADC0();
    Configurar_UART0();
    Configura_Reg_PWM1(50);//Configuro a 1khz el pwm
    Configurar_Timer0A(&Hilo_1,10000);
    while(1)
    {
        
    }

}
