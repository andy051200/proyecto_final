#pragma config FOSC = INTRC_NOCLKOUT// se declara oscilador interno
#pragma config WDTE = OFF       // Watchdog Timer apagado
#pragma config PWRTE = OFF      // Power-up Timer apagado
#pragma config MCLRE = OFF      // MCLRE apagado
#pragma config CP = OFF         // Code Protection bit apagado
#pragma config CPD = OFF        // Data Code Protection bit apagado
#pragma config BOREN = OFF      // Brown Out Reset apagado
#pragma config IESO = OFF       // Internal External Switchover bit apagado
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit apagado
#pragma config LVP = ON        // low voltaje programming prendido

// CONFIG2
#pragma config BOR4V = BOR40V   // configuración de brown out reset
#pragma config WRT = OFF        // apagado de auto escritura de cÃ?Â³digo

/*-----------------------------------------------------------------------------
------------------------librerias a implementar ------------------------------
-----------------------------------------------------------------------------*/
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


#define  _XTAL_FREQ 4000000  //se define el delay con FreqOsc 4Mhz
void setup(void); 

void main(void)  //funcion principal sin retorno
{  
    setup();                    //Configuraciones generales del PIC
    //ADCON0bits.GO=1;            //conversion inicial del ADC
    while(1)
    {
        PORTAbits.RA0=0;
        __delay_ms(500);
        PORTAbits.RA0=0;
        __delay_ms(500);
       
    } 
}

void setup(void)
{
    TRISA=0;        //PortA como salida
    PORTA =0;       //se limpia puerto
    
    OSCCONbits.IRCF2 = 1;       //Freq a 4MHz, 110
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;         //se habilita oscilador interno
    
    
}


