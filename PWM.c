
#include "lib/include.h"
// EXPERIMENTOS 1,2 y 3 PWM
extern void Configurar_PWM(uint16_t freq)
{
    SYSCTL->RCGCPWM |= (1<<1); /*Enable reloj de modulo PWM1 pag 354*/
    SYSCTL->RCGCGPIO |= (1<<5); /*Enable reloj de GPIO Puerto F pag 340 pin 5*/
    SYSCTL->RCC &= ~(1<<20);  /*Enable o Disable Divisor  Pag 254*/
   // GPIOF->AFSEL |= (1<<3)|(1<<2)|(1<<1); /*Control de registros ya sea por GPIO o Otros Pag 672*/
    GPIOF->AFSEL |= 0x0E;
    GPIOF->PCTL |= (GPIOF->PCTL&0xFFFF000F) | 0x00005550; /*Combinado con la tabla Pag 1351 y el registro PCTL le digo que es pwm Pag 689*/
    GPIOF->DEN |= (1<<3)|(1<<2)|(1<<1); /* para decirle si es digital o no Pag 682*/
    PWM0->RCC = (1<<20)|(0x4<<0);  /*Enable o Disable Divisor  Pag 1747*/
    //PWM1->RCC = (1<<20)|(0x4<<0);  
    // EXPERIMENTO 1
    PWM0->RCC = (1<<20)|(0x5<<0); //PARA EL DIVISOR
    PWM0->_1_CTL = (0<<0); // BLOQUEAR Y DESBLOQUEAR 
    PWM0->_1_GENA |= (0X2<<2)|(0X3<<6)|(0x01<17); //HABILITAR EN RCC Y DIVISOR = 64 =0x5
    PWM0->_1_LOAD = 62500;
    PWM0->_1_CMPA = 4688;
    PWM0->_1_CTL = (1<<0);

    

    // EXPERIMENTO 2

    PWM0->RCC = (1<<20)|(0x4<<0);  //enable en RCC y pwmdiv = 32 =0x4
    PWM0->_0_CTL = (0<<0);
    PWM0->_1_CTL = (0<<0);
    PWM0->_3_CTL = (0<<0);
    
    // AQUI LOS PRIMEROS TRES GENERADORES DEL M0
    PWM0->_0_GENA |= (0X2<<2)|(0X3<<6)|(0x01<17);  //GENERADOR NUMERO 1
    PWM0->_2_GENA |= (0X2<<2)|(0X3<<6)|(0x01<17);  //GENERADOR NUMERO 1
    PWM0->_6_GENA |= (0X2<<2)|(0X3<<6)|(0x01<17);  //GENERADOR NUMERO 2

    //INICIAMOS CONTADOR EN 250000 
    PWM0->_0_LOAD = 25000; 
    PWM0->_2_LOAD = 25000;
    PWM0->_6_LOAD = 25000;

    // COMPARADOR DE 
    PWM0->_0_CMPA = 1875;  
    PWM0->_2_CMPA = 1875;
    PWM0->_6_CMPA = 1875;
    PWM0->_0_CTL = (1<<0);
    PWM0->_1_CTL = (1<<0);
    PWM0->_3_CTL = (1<<0);

    // PWM 1
    PWM1->_2_CTL = (0<<0);
    PWM1->_3_CTL = (0<<0); /*Bloqueo y desbloqueo*/
    PWM1->_3_GENB = 0x0000080C; /*Registro de las acciones del pwm Pag 1285*/
    PWM1->_3_GENA = 0x0000008C; /*Registro de las acciones del pwm Pag 1282*/
    PWM1->_2_GENB = 0x0000008C;//PWM5
    PWM1->_3_LOAD = 20000; /*cuentas=fclk/fpwm  para 1khz cuentas = (16,000,000/1000)*/
    PWM1->_2_LOAD = 20000;
    PWM1->_3_CMPB = 15000;
    PWM1->_3_CMPA = 15000;
    PWM1->_2_CMPA = 15000;
    PWM1->_3_CTL = (1<<0);// Se activa el generador 3
    PWM1->_2_CTL = (1<<0);// Se activa el generador 2
    PWM1->ENABLE = (1<<7) | (1<<6) | (1<<5); /*habilitar el bloque pa que pase Pag 1247*/

    }
    // FUNCION DELAY 
 extern void delayms(int i)
{
    int a,b;
    for(a=0;a<i;a++)
    for(b=0;b<3180;b++)
    {}
    

}
