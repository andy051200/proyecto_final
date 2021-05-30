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
#define direccion_eeprom 10  //direccion de escritura/lectura EEPROM
#define _tmr0_value 99       //valor para interccupcion cada 20ms

/*-----------------------------------------------------------------------------
------------------------varibales a implementar ------------------------------
-----------------------------------------------------------------------------*/
int x=0;    
int wenas=0;
int servo1_1;           //control de for en servo1 a 0°
int servo1_2;           //control de for en servo1 a 45°
int servo2_1;           //control de for en servo2 a 0°
int servo2_2;           //control de for en servo2 a 45°
int servo3_1;           //control de for en servo3 a 0°
int servo3_2;           //control de for en servo3 a 45°
char dato_recibido;     //valor recibido de interfaz
int eeprom_sino;        //variable para guardar o no a EEPROM
//----
char dato ;
//-----
const char data = 10;
char out_str;


/*-----------------------------------------------------------------------------
-------------------------prototipos de funciones-------------------------------
-----------------------------------------------------------------------------*/
//setup del pic
void setup(void);  //funcion para configuracion de registros del PIC
void loop_servos(void);

//transmision
void transmision_tx(char data);  //funcion para transmitir datos via UART

//cadena
void USART_Cadena(char *str);

//recepcion
char recepcion_rx();    //funcion para recepcion datos via UART

//servo1
void servo1_19(void);   //funcion para mover servo1 a 0° 
void servo1_18(void);   //funcion para mover servo1 a 45°
//servo2
void servo2_19(void);   //funcion para mover servo2 a 0°
void servo2_18(void);   //funcion para mover servo2 a 45° 
//servo3
void servo3_19(void);   //funcion para mover servo3 a 0°
void servo3_18(void);   //funcion para mover servo3 a 45°

//-----prueba de eeprom
void writeToEEPROM(char data, int address); //los ints en teoria son de 8bits
//unsigned char readFromEEPROM(int address);

//-----------

/*-----------------------------------------------------------------------------
---------------------------- interrupciones -----------------------------------
-----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
}

/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/
void main (void)
{
    setup();                   //se llama funcion con configuracion
    
    //MAIN LOOP
    while (1)                           
    {
        if (INTCONbits.T0IF=1)
        {
            if (!PORTBbits.RB0)     //ver si está en 0
            {
                for (servo1_1 = 0;servo1_1 <20; servo1_1++  )
                {
                    PORTDbits.RD0=1;
                    __delay_ms(2);
                    PORTDbits.RD0=1;
                }
            }
        }
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
    
    TRISB = 1;              //PortB como entrada
    
    TRISCbits.TRISC1 = 0;   //RC1 como salida CCP2
    TRISCbits.TRISC2 = 0;   //RC1 como salida CCP1
    
    TRISDbits.TRISD0 = 0;   //RD0 como salida, Servo1
    TRISDbits.TRISD1 = 0;   //RD1 como salida, Servo2
    TRISDbits.TRISD2 = 0;   //RD2 como salida, Servo3
    
    TRISE = 0;              //PortE como salida
    
    PORTA = 0;              //se limpia puertoA
    PORTB = 0;              //se limpia puertoB
    PORTC = 0;              //se limpia puertoC
    PORTD = 0;              //se limpia puertoD
    PORTE = 0;              //se limpia puertoE
    
    //CONFIGURACION DE OSCILADOR
    OSCCONbits.IRCF2 = 1;   //Freq a 8MHz, 111
    OSCCONbits.IRCF1 = 1;   //Freq a 8MHz, 111
    OSCCONbits.IRCF0 = 1;   //Freq a 8MHz, 111
    OSCCONbits.SCS=1;       //oscilador interno
 
    
    //CONFIGURACION DEL TIMER0
    OPTION_REGbits.T0CS = 0;    //Timer0 Clock select 
    OPTION_REGbits.T0SE = 0;    //en flanco de subida
    OPTION_REGbits.PSA = 0;     //preescaler en modulo
    OPTION_REGbits.PS2 = 1;     //preescaler 1:256, 111 
    OPTION_REGbits.PS1 = 1;     //preescaler 1:256, 111
    OPTION_REGbits.PS0 = 1;     //preescaler 1:256, 111
    TMR0 = 89;                  //valor inicial, interrupcion cada 20ms
    
    //Timer1 Registers Prescaler= 8 - TMR1 Preset = 60536 - Freq = 50.00 Hz - Period = 0.020000 seconds
    T1CONbits.T1CKPS1 = 1;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 1;   // bit 4
    T1CONbits.T1OSCEN = 1;   // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = 0xEC;            // valor inicial de MSB de timer1
    TMR1L = 0x78;            // valor inicial de LSB de timer1
    
    //CONFIGURACION DE BOTONES
    OPTION_REGbits.nRBPU=1;     //se habilita WPUB
    WPUBbits.WPUB0 = 1;         //WPUB en RB0
    WPUBbits.WPUB1 = 1;         //WPUB en RB1
    WPUBbits.WPUB2 = 1;         //WPUB en RB1
    
    //CONFIGURACION DEL ADC
    ADCON1bits.ADFM = 0 ;   // se justifica a la isquierda
    ADCON1bits.VCFG0 = 0 ;  // voltajes de referencia
    ADCON1bits.VCFG1 = 0 ;  // voltaje de referencia
    ADCON0bits.ADCS = 0b10 ; // se usa division de 4us con F/32
    ADCON0bits.CHS = 0;     // seleccion de canal 0
    __delay_us(100);         //delay de 50us para que cargue capacitor
    ADCON0bits.ADON = 1 ;   // se prende modulo ADC
    
    //CONFIGURACION DE PWM
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
    //while(PIR1bits.TMR2IF==0);  //ciclo para que nunca se prenda bandera
    PIR1bits.TMR2IF=0;          // se apaga bandera por si las moscas
    TRISCbits.TRISC2 = 0;       //salida del pwm1
    TRISCbits.TRISC1= 0;        // salida del pwm 2
    
    //CONFIGURACION DE COMUNICACION UART
    //transmision
    TXSTAbits.SYNC = 0;         //se transmicion asincrona
    TXSTAbits.BRGH = 1;         //transmision de alta velocidad
    BAUDCTLbits.BRG16 = 1;      //se habilitan los 16 bits
    //braudeaje
    SPBRG = 207;                //segun tabla se asigna valor de 208             
    SPBRGH = 0;                 //plt, no tiene valores en MSB
    //recepcion
    RCSTAbits.SPEN = 1;         //enciendo el modulo
    RCSTAbits.RX9 = 0;          //No trabajo a 9 bits
    RCSTAbits.CREN = 1;         //activo recepción
    TXSTAbits.TXEN = 1;         //activo transmision 
    
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;     //habilitan interrupciones globales
    INTCONbits.PEIE = 1;    //habilitan interrupciones por perifericos
    INTCONbits.T0IE=0;      //no se habilita la interrupcion como tal
    INTCONbits.T0IF=0;      //se apaga bandera de interrupcion
    PIE1bits.ADIE = 0;      //no se habilita interrupcion del ADC
    PIR1bits.ADIF = 0;      //se apaga bandera de interrupcion del ADC
    
    //tiempo de espera para conversion inicial ADC
    __delay_us(100);
    ADCON0bits.GO=1;
    
}

/*-----------------------------------------------------------------------------
------------------------------ funciones -------------------------------------
-----------------------------------------------------------------------------*/

