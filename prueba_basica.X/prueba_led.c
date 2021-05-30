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
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit apagado
#pragma config LVP = ON        // low voltaje programming prendido
// CONFIG2
#pragma config BOR4V = BOR40V   // configuración de brown out reset
#pragma config WRT = OFF        // apagado de auto escritura de cÃ?Â³digo
/*-----------------------------------------------------------------------------
------------------------librerias a implementar ------------------------------
-----------------------------------------------------------------------------*/
#include <pic16f887.h>
#include <stdint.h>
#include <xc.h>
#define  _XTAL_FREQ 8000000  //se define el delay con FreqOsc 4Mhz

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
    //interrupcion por conversion de ADC
    if(PIR1bits.ADIF)
    {
        if (ADCON0bits.CHS ==5)
        {
            CCPR1L = (ADRESH>>1)+62;          
            CCP1CONbits.DC1B1 = ADRESH & 0b01; 
            CCP1CONbits.DC1B0 = (ADRESL>>7);
            ADCON0bits.CHS = 6;               
        }
        if (ADCON0bits.CHS ==6)
        {
            CCPR2L = (ADRESH>>1)+62;           
            CCP2CONbits.DC2B1 = ADRESH & 0b01;  
            CCP2CONbits.DC2B0 = (ADRESL>>7);
            ADCON0bits.CHS = 7;                
        }
        if (ADCON0bits.CHS == 7)
        {
            PORTA = ADRESH;                     
            ADCON0bits.CHS = 5;
        }
        
        
        PIR1bits.ADIF =0;
        __delay_us(50);
        ADCON0bits.GO = 1;
    }
}

/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/
void main (void)
{
    setup();
    __delay_us(50);
    ADCON0bits.GO=1;                //se inicia la conversion inicial
    while (1)
    {
        
    }
    
}
/*-----------------------------------------------------------------------------
--------------------------- configuraciones ----------------------------------
-----------------------------------------------------------------------------*/
void setup()
{
    //CONFIGURACION DE ENTRADAS Y SALIDAS
    ANSELbits.ANS5=1;               //AN5 como entrada analogica
    ANSELbits.ANS6=1;               //AN6 como entrada analogica
    ANSELbits.ANS7=1;               //AN7 como entrada analogica
    
    TRISA=0;
    TRISB=0;                        //PortB como salida
    TRISC=0;                        //PortC como salida
    TRISD=0;                        //PortD como salida
    TRISEbits.TRISE0=1;             //RE0 como entrada
    TRISEbits.TRISE1=1;             //RE1 como entrada
    TRISEbits.TRISE2=1;             //RE2 como entrada
    
    PORTA=0;                        //Se limpia PortA
    PORTB=0;                        //Se limpia PortB
    PORTC=0;                        //Se limpia PortC
    PORTD=0;                        //Se limpia PortD
    PORTE=0;                        //Se limpia PORTE
    
    //CONFIGURACION DE OSCILADOR
    OSCCONbits.IRCF2 = 1;   //Freq a 4MHz, 110
    OSCCONbits.IRCF1 = 1;   //Freq a 4MHz, 110
    OSCCONbits.IRCF0 = 1;   //Freq a 4MHz, 110
    OSCCONbits.SCS=1;       //oscilador interno
    
    //CONFIGURACION DE ADC
    ADCON1bits.ADFM = 0;        //Se justifica a la izquierda
    ADCON1bits.VCFG0 = 0;       //voltaje de referencia Vss
    ADCON1bits.VCFG1 = 0;       //Voltaje de referencia Vdd
    ADCON0bits.ADCS = 0b10;     // Fosc/32 como TAD
    ADCON0bits.CHS = 0;         // seleccion de canal 0
    __delay_us(100);            //delay para cargar capacitor
    ADCON0bits.ADON =1;         //se encinde el modulo ADC
    
    //CONFIGURACION DE CCPX
    PR2=188;                    //valor cargado a PR2 para Tosc PWM
    //ccp1
    TRISCbits.TRISC2=1;         //RC1 para configuracion de modulo
    CCP1CONbits.CCP1M=0b1100;   //como PWM en pin CCP1
    CCPR1L = 0x0f ;             // ciclo de trabajo inicial de la onda cuadrada
    CCP1CONbits.DC1B = 0;       // LSB para ciclo de trabajo
    //ccp2
    TRISCbits.TRISC1=1;         //RC1 para configuracion de modulo
    CCP2CONbits.CCP2M=0b1100;   //CCP2 como modo PWM
    CCPR2L = 0x0f;              // ciclo de trabajo inicial de la onda cuadrada
    CCP2CONbits.DC2B1 = 0;      // LSB para ciclo de trabajo
    //configuracion del timer2
    PIR1bits.TMR2IF = 0;        // apagar bandera de interrupcion del timer2
    T2CONbits.T2CKPS = 0b11;    // preescaler del timer2 1:16
    T2CONbits.TMR2ON = 1;       //se prende el timer2
    PIR1bits.TMR2IF=0;          // se apaga bandera por si las moscas
    TRISCbits.TRISC2 = 0;       //salida del pwm1
    TRISCbits.TRISC1= 0;        // salida del pwm 2
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilitan interrupciones globales
    INTCONbits.PEIE=1;          //se habilitan interrupciones por perifericos
    PIE1bits.ADIE=1;            //se habilita interrupcion por ADC
    PIR1bits.ADIF=0;            //se apaga bandera de conversion
    
}

/*-----------------------------------------------------------------------------
------------------------------ funciones -------------------------------------
-----------------------------------------------------------------------------*/

