/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: Proyecto final de Programacion de microcontroaladores
Hardware: PIC16F887, 
    
Creado: 26 de abril de 2021    
Descripcion: 
//se usan modulo ADC, CCP1, CCP2, ida y vuelta UART, W/R EEPROM
------------------------------------------------------------------------------*/
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// se declara oscilador interno
#pragma config WDTE = OFF       // Watchdog Timer apagado
#pragma config PWRTE = OFF      // Power-up Timer apagado
#pragma config MCLRE = OFF      // MCLRE apagado
#pragma config CP = OFF         // Code Protection bit apagado
#pragma config CPD = OFF        // Data Code Protection bit apagado
#pragma config BOREN = OFF      // Brown Out Reset apagado
#pragma config IESO = OFF       // Internal External Switchover bit apagado
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enabled bit apagado
#pragma config LVP = ON        // low voltaje programming prendido

// CONFIG2
#pragma config BOR4V = BOR40V   // configuración de brown out reset
#pragma config WRT = OFF        // apagado de auto escritura de cÃ?Â³digo

/*-----------------------------------------------------------------------------
------------------------librerias a implementar ------------------------------
-----------------------------------------------------------------------------*/
#include <xc.h>
#define  _XTAL_FREQ 4000000  //se define el delay con FreqOsc 4Mhz
#define _tmr0_value 150      //valor inicial del Timer0

/*-----------------------------------------------------------------------------
------------------------varibales a implementar ------------------------------
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
-------------------------prototipos de funciones-------------------------------
-----------------------------------------------------------------------------*/
void setup(void);  //funcion para configuracion de registros del PIC

/*-----------------------------------------------------------------------------
---------------------------- interrupciones -----------------------------------
-----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    if (INTCONbits.RBIF==1)
    {
        PORTEbits.RE0 = 1;
    }
}
/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/
void main(void)  //funcion principal sin retorno
{  
    setup();                    //Configuraciones generales del PIC
    while(1)
    {
        PORTDbits.RD0=1;
        __delay_ms(1);
        PORTDbits.RD0=0;
        __delay_ms(19);
    }
}
/*-----------------------------------------------------------------------------
--------------------------- configuraciones ----------------------------------
-----------------------------------------------------------------------------*/
void setup(void) //FUNCION PARA CONFIGURACION DE ENTRADAS Y SALIDAS
{
    //CONFIGURACIONES ENTRADAS ANALOGICAS
    ANSEL=0;
    ANSELH = 0; 
    
    //CONFIGURACION ENTRADAS/SALIDAS
    TRISAbits.TRISA0 = 1;       //RA0 como entrada 
    TRISAbits.TRISA1 = 1;       //RA1 como entrada
    TRISAbits.TRISA2 = 1;       //RA2 como entrada
    TRISBbits.TRISB0 = 1;       //RB0 como entrada
    TRISBbits.TRISB1 = 1;       //RB1 como entrada
    TRISC = 0;                  //PortC com salida
    TRISD = 0;                  //PortD com salida
    TRISE = 0;                  //PortE como salida
    //limpieza de puertos
    PORTA = 0;                  //se limpia PortA
    PORTB = 0;                  //se limpia PortB
    PORTC = 0;                  //se limpia PortC
    PORTD = 0;                  //se limpia PortD
    PORTE = 0;                  //se limpia PortE
    
    //CONFIGURACION DE OSCILADOR
    OSCCONbits.IRCF2 = 1;       //Freq a 4MHz, 110
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;         //se habilita oscilador interno
    
    //CONFIGURACION DEL TIMER0
    OPTION_REGbits.T0CS = 0;    //Fosc/4
    OPTION_REGbits.PS2 = 1;     //Preescaler de 1:256, 111
    OPTION_REGbits.PS1 = 1;     //Preescaler de 1:256, 111
    OPTION_REGbits.PS0 = 1;     //Preescaler de 1:256, 111
    //interrupcion cada 50ms
    
    //CONFIGURACION DE WPUB y IntOnChangeB
    //OPTION_REGbits.nRBPU = 1;   //se habilitan los WPUB
    IOCBbits.IOCB0 = 1;         //IntOnChange en RB0
    IOCBbits.IOCB1 = 1;         //IntOnChange en RB1
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;         //se habilitan interrupciones globales
    //INTCONbits.PEIE = 1;        //se habilita interrupcion por perifericos
    INTCONbits.T0IE = 1;        //se habilita int de timer0
    INTCONbits.RBIE = 1;        //se habilita IntOnChangeB
    INTCONbits.T0IF = 0;        //se apaga interrupcion timer0
    INTCONbits.RBIF = 0;        //se apaga interrupcion IntOnChangeB
    
}