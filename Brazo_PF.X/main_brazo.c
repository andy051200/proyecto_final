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
{
    //INTERRUPCION DEL ADC
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

}
/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/
void main(void)  //funcion principal sin retorno
{  
    setup();                    //Configuraciones generales del PIC
    ADCON0bits.GO=1;            //conversion inicial del ADC
    while(1)
    {
        //switch_adc();           //se llama funcion de switch
        servos_loop();          //
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
    ANSELbits.ANS2 = 0;         //AN2 como entrada analógica
    ANSELH = 0; 
    
    //CONFIGURACION ENTRADAS/SALIDAS
    //TrisA
    TRISAbits.TRISA0 = 1;       //RA0 como entrada 
    TRISAbits.TRISA1 = 1;       //RA1 como entrada
    TRISAbits.TRISA2 = 1;       //RA2 como entrada
    //TrisB
    TRISBbits.TRISB2 = 1;       //RB2 como entrada
    //TrisC
    TRISCbits.TRISC1 = 0;       //salida del CCP2
    TRISCbits.TRISC2 = 0;       //salida del CCP1
    //TrisD    
    TRISDbits.TRISD0 = 0;        //RD0 como salida
    TRISDbits.TRISD1 = 0;        //RD1 como salida
    TRISDbits.TRISD2 = 0;        //RD2 como salida
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
    
    //CONFIGURACION DEL TIMER0 (delay dentro de interrupcion)
    OPTION_REGbits.T0CS = 0;         
    OPTION_REGbits.T0SE = 0;        
    OPTION_REGbits.PSA = 0;         
    OPTION_REGbits.PS2 = 0;      
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    
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
    INTCONbits.GIE = 0;         //se habilitan interrupciones globales
    INTCONbits.PEIE = 0;        //se habilita interrupcion por perifericos
    //interrupciones del ADC
    PIE1bits.ADIE = 0 ;         //se habilita interrupcion por ADC
    PIR1bits.ADIF = 0;          // se baja bandera de conversion
    return;
}
/*-----------------------------------------------------------------------------
--------------------------------- funciones -----------------------------------
---------------------------------------------------------------------------- */
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
/*
int switch_adc()
{
    if (ADCON0bits.GO==0)
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
        ADCON0bits.GO = 1;      // se prende la siguiente conversión
    }
}*/