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
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#define  _XTAL_FREQ 8000000  //se define el delay con FreqOsc 4Mhz
#define _tmr0_value 254     //valor del Timer0, interrupcion cada 8us

/*-----------------------------------------------------------------------------
------------------------varibales a implementar ------------------------------
-----------------------------------------------------------------------------*/
int x=0;
int servo1_1;
int servo1_2;
int servo2_1;
int servo2_2;
int servo3_1;
int servo3_2;

/*-----------------------------------------------------------------------------
-------------------------prototipos de funciones-------------------------------
-----------------------------------------------------------------------------*/
void setup(void);  //funcion para configuracion de registros del PIC

/*-----------------------------------------------------------------------------
---------------------------- interrupciones -----------------------------------
-----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{}

/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/
void main (void)
{
    setup();
    while (1)
    {
        servos_loop();
    }

}
/*-----------------------------------------------------------------------------
--------------------------- configuraciones ----------------------------------
-----------------------------------------------------------------------------*/
void setup()
{
    //CONFIGURACION DE ENTRADAS Y SALIDAS
    ANSELbits.ANS0 = 1;     //AN0 como entrada analogica
    ANSELbits.ANS1 = 1;     //AN1 como entrada analogica
    ANSELbits.ANS2 = 1;     //AN2 como entrada analogica

    TRISAbits.TRISA0 = 1;   //RA0 como entrada
    TRISAbits.TRISA1 = 1;   //RA0 como entrada
    TRISAbits.TRISA2 = 1;   //RA0 como entrada
    
    TRISBbits.TRISB2 = 1;   //RB2 como entrada
    
    TRISCbits.TRISC1 = 0;   //RC1 como salida CCP2
    TRISCbits.TRISC2 = 0;   //RC1 como salida CCP1
    
    TRISDbits.TRISD0 = 0;   //RD0 como salida, Servo1
    TRISDbits.TRISD1 = 0;   //RD1 como salida, Servo2
    TRISDbits.TRISD2 = 0;   //RD2 como salida, Servo3
    
    PORTA = 0;              //se limpia puerto
    PORTB = 0;              //se limpia puerto
    PORTC = 0;              //se limpia puerto
    PORTD = 0;              //se limpia puerto
    
    //CONFIGURACION DE OSCILADOR
    OSCCONbits.IRCF2 = 1;   //Freq a 8MHz, 111
    OSCCONbits.IRCF1 = 1;   //Freq a 8MHz, 111
    OSCCONbits.IRCF0 = 1;   //Freq a 8MHz, 111
    OSCCONbits.SCS=1;       //oscilador interno
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;     //habilitan interrupciones globales
    //INTCONbits.
    
}

int servos_loop()
{
    if (PORTBbits.RB2 ==1)     //se incrementa variable de control
    {
        x++;
    }
      
    if (x == 1)                 //para servo1 a 0°
    {
        for (servo1_1 = 0; servo1_1 <= 20; servo1_1++)
        {
            PORTDbits.RD0 = 1;
            __delay_ms(1);
            PORTDbits.RD0 = 0;
            __delay_ms(19);
        }
    }
          
    if (x == 2)                 //para servo1 a 45°
    {
        for (servo1_2 = 0; servo1_2 <= 20; servo1_2++)
        {
            PORTDbits.RD0 = 1;
            __delay_ms(1.5);
            PORTDbits.RD0 = 0;
            __delay_ms(18.5);
        }
    }
                
    if (x ==3)                  //para servo2 a 0°
    {
        for (servo2_1 = 0; servo2_1 <= 20; servo2_1++)
        {
            PORTDbits.RD1 = 1;
            __delay_ms(1);
            PORTDbits.RD1 = 0;
            __delay_ms(19);
        }
    }
           
    if (x==4)                   //para servo2 a 45°
    {
        for (servo2_2 = 0; servo2_2 <= 20; servo2_2++)
        {
            PORTDbits.RD1 = 1;
            __delay_ms(1.5);
            PORTDbits.RD1 = 0;
            __delay_ms(18.5);
        }   
    }
            
    if (x==5)                   //para servo3 a 0°
    {
        for (servo3_1 = 0; servo3_1 <= 20; servo3_1++)
        {
            PORTDbits.RD2 = 1;
            __delay_ms(1);
            PORTDbits.RD2 = 0;
            __delay_ms(19);
        }
    }
            
    if (x==6)                   //para servo3 a 45°
    {
        for (servo3_2 = 0; servo3_2 <= 20; servo3_2++)
        {
            PORTDbits.RD2 = 1;
            __delay_ms(1.5);
            PORTDbits.RD2 = 0;
            __delay_ms(18.5);        
        }
    }
    if (x==7)           //se estable limite
    {
        x=0;
    }
}