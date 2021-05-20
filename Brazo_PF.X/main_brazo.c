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
//#define _tmr0_value 25      //valor inicial del Timer0

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
    if (PIR1bits.ADIF==1)
    {
        //conversion de potenciometro del servo 1   
        if (ADCON0bits.CHS == 0)  
        { 
            CCPR1L = (ADRESH>>1)+62;          //rando de 180°
            CCP1CONbits.DC1B1 = ADRESH & 0b01; //resolucion de movimiento
            CCP1CONbits.DC1B0 = (ADRESL>>7);
            ADCON0bits.CHS = 1;             //se cambia al canal dse conversion
        }
        //conversion del potenciometro del servo2
        else   
        {
            CCPR2L = (ADRESH>>1)+62;           //rando de 180°
            CCP2CONbits.DC2B1 = ADRESH & 0b01;  // resolucion de movimiento
            CCP2CONbits.DC2B0 = (ADRESL>>7);
            ADCON0bits.CHS = 0;                //se cambia a canal de conversion
        } 
        __delay_us(50);         //delay de 50 us para cargar capacitor
        PIR1bits.ADIF = 0;      // se apaga bandera de interrupcion ADC
        ADCON0bits.GO = 1;      // se prende la siguiente conversión

    }
    return; 
    
}
/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/
void main(void)  //funcion principal sin retorno
{  
    setup();                    //Configuraciones generales del PIC
    //__delay_ms(20);             //espera de 20ms para conversion inicial
    ADCON0bits.GO=1;            //conversion inicial del ADC
    while(1)
    {
        //RUTINAS PARA LOS SERVOS
        //servo 1
        while (PORTBbits.RB0)
        {
            PORTDbits.RD0 = 1;
            __delay_ms(1);
            PORTDbits.RD0 = 0;
            __delay_ms(19);
        }
        while (PORTBbits.RB1)
        {
            PORTDbits.RD0 = 1;
            __delay_ms(2);
            PORTDbits.RD0 = 0;
            __delay_ms(18);
        }
        
        //servo 2
        while (PORTBbits.RB2==1)         
        {
            PORTDbits.RD1 = 1;
            __delay_ms(1);
            PORTDbits.RD1 = 0;
            __delay_ms(19);
        }
        while (PORTBbits.RB4==1)
        {
            PORTDbits.RD1 = 1;
            __delay_ms(2);
            PORTDbits.RD1 = 0;
            __delay_ms(18);
        }
        
        //servo3
        while (PORTBbits.RB5)
        {
            PORTDbits.RD2 = 1;
            __delay_ms(1);
            PORTDbits.RD2 = 0;
            __delay_ms(19);
        }
        while (PORTBbits.RB6)
        {
            PORTDbits.RD2 = 1;
            __delay_ms(2);
            PORTDbits.RD2 = 0;
            __delay_ms(18);
        }
        
        
    }
    
}
/*-----------------------------------------------------------------------------
--------------------------- configuraciones ----------------------------------
-----------------------------------------------------------------------------*/
void setup(void) //FUNCION PARA CONFIGURACION DE ENTRADAS Y SALIDAS
{
    //CONFIGURACIONES ENTRADAS ANALOGICAS
    ANSELbits.ANS0 = 1;         //AN0 como entrada analógica
    ANSELbits.ANS1 = 1;         //AN1 como entrada analógica
    ANSELbits.ANS2 = 1;         //AN2 como entrada analógica
    ANSELH = 0; 
    
    //CONFIGURACION ENTRADAS/SALIDAS
    //puertoA
    TRISAbits.TRISA0 = 1;       //RA0 como entrada 
    TRISAbits.TRISA1 = 1;       //RA1 como entrada
    TRISAbits.TRISA2 = 1;       //RA2 como entrada
    TRISB = 1;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;       //salida del CCP2
    TRISCbits.TRISC2 = 0;       //salida del CCP1
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    TRISD = 0;                  //PortD com salida
    TRISE = 0;                  //PortE como salida
    //limpieza de puertos
    PORTA = 0;                  //se limpia PortA
    PORTB = 0;                  //se limpia PortB
    PORTC = 0;                  //se limpia PortC
    PORTD = 0;                  //se limpia PortD
    PORTE = 0;                  //se limpia PortE
    
    //CONFIGURACION DE OSCILADOR
    OSCCONbits.IRCF2 = 1;       //Freq a 8MHz, 111
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //se habilita oscilador interno
    
    //CONFIGURACION DE ADC
    ADCON1bits.ADFM = 0 ;       // se justifica a la izquierda
    ADCON1bits.VCFG0 = 0 ;      // voltajes de referencia 5V
    ADCON1bits.VCFG1 = 0 ;      // voltaje de referencia gnd
    ADCON0bits.ADCS = 2 ;       // se usa division de 4us con F/32
    ADCON0bits.CHS = 0;         // seleccion de canal 1
    ADCON0bits.ADON = 1 ;       // se prende modulo ADC
    __delay_us(50);             // delay de 50us para que cargue capacitor
    
   //CONFIGURACION DEL MODULO PWM
    PR2 = 249;                  //configurando el periodo de oscilación
    //CCP1 para servo 1
    TRISCbits.TRISC2=1;         // RC2/CCP1 como entrada a motor se desconecta
    CCP1CONbits.P1M = 0;        // configuracion de una señales de salida
    CCP1CONbits.CCP1M = 0b1100; // se configura como modo PWM
    CCPR1L = 0x0f ;             // ciclo de trabajo inicial de la onda cuadrada
    CCP1CONbits.DC1B = 0;       // LSB para ciclo de trabajo
    //CCP2 para servo 2
    TRISCbits.TRISC1 = 1;       // RC1/CCP2 como entrada a motor se desconecta
    CCP2CONbits.CCP2M = 0b1100; // se configura como modo PWM
    CCPR2L = 0x0f;              // ciclo de trabajo inicial de la onda cuadrada
    CCP2CONbits.DC2B1 = 0;      // LSB para ciclo de trabajo
    
    //CONFIGURACION DEL TIMER2
    PIR1bits.TMR2IF = 0;        // apagar bandera de interrupcion del timer2
    T2CONbits.T2CKPS = 0b11;    // preescaler del timer2 1:16
    T2CONbits.TMR2ON = 1;       //se prende el timer2
    //configuracion del timer2 para el PWM
    while(PIR1bits.TMR2IF==0);  //ciclo para que nunca se prenda bandera
    PIR1bits.TMR2IF=0;          // se apaga bandera por si las moscas
    TRISCbits.TRISC2 = 0;       //salida del pwm1
    TRISCbits.TRISC1= 0;        // salida del pwm 2
   
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;         //se habilitan interrupciones globales
    INTCONbits.PEIE = 1;        //se habilita interrupcion por perifericos
    //interrupciones del Timer2
    //PIE1bits.TMR2IE = 1;        //se habilitainterrupcion por Timer2
    //PIR1bits.TMR2IF = 0;        //se apaga interrucpion del Timer2
    //interrupciones del ADC
    PIE1bits.ADIE = 1 ;         //se habilita interrupcion por ADC
    PIR1bits.ADIF = 0;          // se baja bandera de conversion
    return;
}
/*-----------------------------------------------------------------------------
------------------------------- funciones ------------------------------------
-----------------------------------------------------------------------------*/
