#include "lib/include.h"
// ADC CHRISTIAN 
extern void ADC_CONFIG(void){
    SYSCTL -> RCGCADC = (1 << 0); // HABILITAR MODULO 0
    //SYSCTL -> RCGCADC = (1 << 1); // HABILITAR MODULO 1

    SYSCTL -> RCGCGPIO = (1 << 4) | (1 << 1) | (1 << 3) | (1 << 2); 
                //       PUERTO E   PUERTO B   PUERTO D    PUERTO C
    // DESHABILITAMOS PINES DIGITALES 
    GPIOE -> DIR = (0 << 0) | (0 << 2) | (0 << 4);
    GPIOB -> DIR = (0 << 5);  
    GPIOD -> DIR = (0 << 2) | (0 << 3);
    
    GPIOE -> AFSEL = (1 << 0) | (1 << 2) | (1 << 4);
    GPIOB -> AFSEL = (1 << 5);
    GPIOD -> AFSEL = (1 << 2) | (1 << 3); // igual, checar en el data los pines 
    
    GPIOE -> DEN = (0 << 0) | (0 << 2) | (0 << 4);  // PUERTO E
    GPIOB -> DEN = (0 << 5);   // PUERTO B
    GPIOD -> DEN = (0 << 2) | (0 << 3);  // PUERTO D
    
    //PA PROBAR EL LED
    GPIOC -> DEN = 0xff;
    GPIOC -> AFSEL = (1 << 1);
    GPIOC -> DIR = 0xff;
    GPIOC -> DATA = (1 << 1); 
    
    GPIOE->PCTL = GPIOE->PCTL & (0xFFF0F0F0); // PUERTO E PINES 0, 2 y 4
    //(GPIOAMSEL) pag.786 habilitar analogico // copiar y pegar para cada puerto
    GPIOB->PCTL = GPIOB->PCTL & (0xFF0FFFFF); // PUERTO B PIN 5
    GPIOD->PCTL = GPIOD->PCTL & (0xFFFF00FF); // PUERTO D PINES 2 y 3 
    
    GPIOE -> AMSEL = (1 << 0) | (1 << 2) | (1 << 4); //convierte que ahora el pin es ADC, 1 en cada pin usado
    GPIOB -> AMSEL = (1 << 5); //convierte que ahora el pin es ADC, 1 en cada pin usado
    GPIOD -> AMSEL = (1 << 2) | (1 << 3); //convierte que ahora el pin es ADC, 1 en cada pin usado

// CONFIGURACION ADC 0
    ADC0->PC = 0x7;// 1 millon de muestras
    //Pag 1099 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC0->SSPRI = 0x3012; // prioridades de derecha a izquierda
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0); //desactivador de secuenciadores
    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0xF<<4) | (0xF<<8); //*como quiero que inicien los seq pag 835
    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    ADC0->SSMUX2 = 0x431;// 0b0000010000110001; // (1 << 0) | (3 << 4) | (4 << 8);MUXn indica el seq y el num de la izq de la funcion indica el ANn el num de la derecha entra en el rango de bits pag 852
    ADC0->SSMUX1 = 0xB95;// 0b0000101110010101; // (5 << 0) | (9 << 4) | (11<< 8)
    // AN1,3,4,5,9 y 11. De seqs tenemos SS1 y SS2 y ocupamos que den 6 salidas
    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    ADC0->SSCTL2 = (1<<2) | (1<<1) | (1<<6) | (1<<5) | (1<<10) | (1<<9); // pag 869 que acciones quiero que haga
    ADC0->SSCTL1 = (1<<2) | (1<<1) | (1<<6) | (1<<5) | (1<<10) | (1<<9);
    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<2) | (1<<1); /* Unmask ADC0 sequence 2 interrupt pag 1082*/
    NVIC->IP[3] = (NVIC->IP[3]&0x00FFFFFF) | 0x20000000; // (4n+3) n = 3 en mi caso porque la interrupcion es 15 y 16
    NVIC->ISER[0] = (1UL << 15); //pag 147

    NVIC->IP[4] = (NVIC->IP[4]&0xFFFFFF00) | 0x00000020; // (4n) n = 4 en mi caso porque la interrupcion es 15 y 16
    NVIC->ISER[0] = (1UL << 16); //pag 147
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000; //NO LE MOVEMOS
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (1<<1) | (0<<0); //ACTIVAMOS SEQ
    ADC0->PSSI |= (1<<2) | (1<<1); //ACTIVAMOS PSSI de cada SEQ
    
} 

extern void SEQ_CONFIG(uint16_t *Result){
    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI |= (1<<2) | (1<<1);

       //SEQ2
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[2] = ADC0->SSFIFO2&0xFFF;
       Result[1] = ADC0->SSFIFO2&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[0] = ADC0->SSFIFO2&0xFFF;
       //Rojo = (uint16_t)readString('%');
       //printChar('A');
        PWM1->_3_CMPB = (int)((1*Result[0])/4095) - 1;
        PWM1->_3_CMPA = (int)((1*Result[1])/4095) - 1;
        PWM1->_2_CMPA = 0;
        ADC0->ISC = 0x0004;  //Conversion finalizada

      //SEQ1
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[5] = ADC0->SSFIFO2&0xFFF;
       Result[4] = ADC0->SSFIFO2&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[3] = ADC0->SSFIFO2&0xFFF;
       //Rojo = (uint16_t)readString('%');
       //printChar('A');
        PWM1->_3_CMPB = (int)((1*Result[0])/4095) - 1;
        PWM1->_3_CMPA = (int)((1*Result[1])/4095) - 1;
        PWM1->_2_CMPA = 0;
        ADC0->ISC = 0x0004;  //Conversion finalizada

 

}





