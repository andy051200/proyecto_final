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

#include <stdint.h>
#include <xc.h>

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
#pragma config LVP = OFF        // low voltaje programming apagado

// CONFIG2
#pragma config BOR4V = BOR40V   // configuración de brown out reset
#pragma config WRT = OFF        // apagado de auto escritura de cÃ?Â³digo

#define  _XTAL_FREQ 8000000  //se define el delay con FreqOsc 4Mhz
/*-----------------------------------------------------------------------------
------------------------librerias a implementar ------------------------------
-----------------------------------------------------------------------------*/

//#define direccion_eeprom 10  //direccion de escritura/lectura EEPROM
//#include "Servo.h"

/*-----------------------------------------------------------------------------
------------------------varibales a implementar ------------------------------
-----------------------------------------------------------------------------*/
int x=0;            
int antirrebote;
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
void servos_loop(void);

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
unsigned char readFromEEPROM(int address);

//-----------

/*-----------------------------------------------------------------------------
---------------------------- interrupciones -----------------------------------
-----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
  
    A
}

/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/
void main (void)
{
    setup();                            //se llama funcion con configuracion
    writeToEEPROM('a',0);        //dato, direccion
    writeToEEPROM('n',1);        //dato, direccion
    writeToEEPROM('d',2);        //dato, direccion
    writeToEEPROM('y',3);        //dato, direccion
    //MAIN LOOP
    while (1)                           
    {
        //interrupciones sin ser interrupciones como tal
        if (INTCONbits.T0IF==1)
        {
            PORTDbits.RD0=1;    //creacion de 1ms en alto
            PORTDbits.RD1=1;
            PORTDbits.RD2=1;
            __delay_ms(1);
            PORTDbits.RD0=0;    //se apaga por 19ms mientras pasa interrupcion
            PORTDbits.RD1=0;
            PORTDbits.RD2=0;
            INTCONbits.T0IF=0;  //apaga la bandera de interrupcion
        }
        if (PIR1bits.TMR1IF==1)
        {
            PORTDbits.RD0=1;    //creacion de 1ms en alto
            PORTDbits.RD1=1;
            PORTDbits.RD2=1;
            __delay_ms(2);
            PORTDbits.RD0=0;    //se apaga por 19ms mientras pasa interrupcion
            PORTDbits.RD1=0;
            PORTDbits.RD2=0;
            PIR1bits.TMR1IF=0;  //se apaga bandera de interrupcion
        }
        
        
        //SWITCH DE TIMERS
        //swtich de timer1 -> timer0
        if (PORTBbits.RB0==1)
        {
            INTCONbits.T0IE=0;
            INTCONbits.T0IF=0;
            PIE1bits.TMR1IE=1;      //se habilita interrupcion   
        }
        else
        {
            INTCONbits.T0IE=1;
            INTCONbits.T0IF=0;
            PIE1bits.TMR1IE=0;      //se habilita interrupcion      
        }
        //switch de timer0 -> timer1
        
        if (PORTBbits.RB1==1)
        {
            INTCONbits.T0IE=1;
            INTCONbits.T0IF=0;
            PIE1bits.TMR1IE=0;      //se habilita interrupcion   
        }
        else 
        {
            INTCONbits.T0IE=0;
            INTCONbits.T0IF=0;
            PIE1bits.TMR1IE=1;      //se habilita interrupcion  
        }
        
       //TOGGLE DE CANALES DEL ADC 
        
        if (ADCON0bits.GO==0)
        {
            if (ADCON0bits.CHS==0)
            {
                CCPR1L =ADRESH;
                __delay_us(100);
                ADCON0bits.CHS=1;
            }
            else if (ADCON0bits.CHS==1)
            {
                CCPR2L =ADRESH;
                __delay_us(100);
                ADCON0bits.CHS=2;
            }
            else if (ADCON0bits.CHS==2)
            {
                PORTE = ADRESH;
                __delay_us(100);
                ADCON0bits.CHS=0;            
            }
            __delay_us(100);        //tiempo de recarga de capacitor
            ADCON0bits.GO=1;        //se reinician las mediciones
        }
        
        //FUNCIONES PARA COMUNICACION SERIAL
        USART_Cadena("\r Que accion desea ejecutar? \r");
        USART_Cadena(" 1) Mover a 0 servo1 \r");
        USART_Cadena(" 2) Mover a 45 servo1 \r");
        USART_Cadena(" 3) Mover a 0 servo2 \r");
        USART_Cadena(" 3) Mover a 45 servo2 \r");
        USART_Cadena(" 3) Mover a 0 servo3 \r");
        USART_Cadena(" 3) Mover a 45 servo3 \r");
        
        if (PIR1bits.RCIF==0)
        {
            dato_recibido = recepcion_rx; //se almacena dato recibio en variable 
        }
        
        switch(dato_recibido)
        {
            //casos para el servo1
            case ('1'):                 //en caso se reciba 1, servo1 a 0°
                servo1_19();            //se llama funcion para 0°
                transmision_tx('Servo 1 a 0');  //estado del servo a GUI
                break;
            case ('2'):                 //en caso se reciba 2, servo1 a 45°
                servo1_18();            //se llama funcion para 45°
                transmision_tx('Servo 1 a 45'); //estado del servo a GUI
                break;
                
            //casos para el servo2    
            case ('3'):                 //en caso se reciba 3, servo2 a 0°
                servo2_19();            //se llama funcion para 0°
                transmision_tx('Servo 2 a 0'); //estado del servo a GUI
                break;
            case ('4'):                 //en caso se reciba 4, servo2 a 45°
                servo2_18();            //se llama funcion para 45°
                transmision_tx('Servo 2 a 45'); //estado del servo a GUI
                break;

            //casos para el servo3      
            case ('5'):                 //en caso se reciba 5, servo3 a 0°
                servo3_19();            //se llama funcion para 0°
                transmision_tx('Servo 3 a 0'); //estado del servo a GUI
                break;
            case ('6'):                 //en caso se reciba 6, servo3 a 45°
                servo3_18();            //se llama funcion para 45°
                transmision_tx('Servo 3 a 45'); //estado del servo a GUI
                break;
                
            case ('7'):                 //en caso se reciba 7, se guardan datos
                writeToEEPROM(ADRESH,direccion_eeprom); 
                break;       
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
    
    TRISBbits.TRISB0 = 1;   //RB2 como entrada
    
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
    
    //CONFIGURACION DE TIMER0
    OPTION_REGbits.T0CS = 0;  //Internal Clock 
    OPTION_REGbits.T0SE = 0;  //TMR0 Source Edge Select bit 0 = low/high 1 = high/low
    OPTION_REGbits.PSA = 0;   //se habilita preescaler Timer0
    OPTION_REGbits.PS2 = 1;   //preescaler 111, 1:256
    OPTION_REGbits.PS1 = 1;   //preescaler 111, 1:256
    OPTION_REGbits.PS0 = 1;   //preescaler 111, 1:256
    TMR0 = 108;               //valor inicial del timer0

    //CONFIGURACION DEL TIMER1
    T1CONbits.T1CKPS1 = 1;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 0;   // bit 4
    T1CONbits.T1OSCEN = 1;   // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = 220;             // preset for timer1 MSB register
    TMR1L = 180;             // preset for timer1 LSB register

    /*//CONFIGURACION DE BOTONES
    IOCBbits.IOCB0 =1;          //IntOnChange Rb0, guardar en EEPROM
    IOCBbits.IOCB1= 1;          //IntOnCHange RB1, hacer lo que se guardo
    OPTION_REGbits.nRBPU=1;     //se habilita WPUB
    WPUBbits.WPUB0 = 1;         //WPUB en RB0
    WPUBbits.WPUB1 = 1;         //WPUB en RB1*/
    
    //CONFIGURACION DEL ADC
    ADCON1bits.ADFM = 0 ;   // se justifica a la isquierda
    ADCON1bits.VCFG0 = 0 ;  // voltajes de referencia
    ADCON1bits.VCFG1 = 0 ;  // voltaje de referencia
    
    ADCON0bits.ADCS = 0b10 ; // se usa division de 4us con F/32
    ADCON0bits.CHS = 0;     // seleccion de canal 0
    __delay_us(50);         //delay de 50us para que cargue capacitor
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
    
    //CONFIGURACION DE UART
    //transmision
    TXSTAbits.SYNC = 0;         //se habilita transmision asincrona
    TXSTAbits.BRGH = 1;         //se habilita transmision de alta velocidad
    BAUDCTLbits.BRG16 = 1;      //se habilita uso los 16 bits   
    //braudeaje
    SPBRG = 12;                //valor adecuado al SPBRG para braudeaje                  
    SPBRGH = 0;                 //pagina 168 del datasheet del 2009       
    //recepcion
    RCSTAbits.SPEN = 1;          //se enciendeel modulo
    RCSTAbits.RX9 = 0;           //se habilita transmision de 8bits
    //encendido de modulos
    RCSTAbits.CREN = 1;          //se activa la recepción
    TXSTAbits.TXEN = 1;          //se activa la transmision 
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;     //habilitan interrupciones globales
    INTCONbits.PEIE = 1;    //habilitan interrupciones por perifericos
    INTCONbits.RBIE=1;      //habilitan IntOnChange
    INTCONbits.RBIF=0;      //se apaga interrupcion
    INTCONbits.T0IE=0;      //se habilita interrupcion del timer0
    INTCONbits.T0IF=0;      //se apaga interrupcion del timer0
    //interrupciones el adc
    PIE1bits.TMR1IE=0;      //no se habilita interrupcion
    PIR1bits.TMR1IF=0;      //se apaga interrupcion
    PIE1bits.ADIE = 0;      //se habilita interrupcion del ADC
    PIR1bits.ADIF = 0;      //se apaga interrupcion del ADC
    
    //tiempo de espera para conversion inicial ADC
    __delay_us(100);
    ADCON0bits.GO=1;
    
}

/*-----------------------------------------------------------------------------
------------------------------ funciones -------------------------------------
-----------------------------------------------------------------------------*/
//funcion para movimiento del servo1 a 0°
void servo1_19(void)
{
    for (servo1_1 = 0; servo1_1 <= 15; servo1_1++)
        {
            PORTDbits.RD0 = 1;
            __delay_ms(1);
            PORTDbits.RD0 = 0;
            __delay_ms(19);
        }
}

//funcion para movimiento del servo1 a 45°
void servo1_18(void)
{
    for (servo1_2 = 0; servo1_2 <= 15; servo1_2++)
        {
            PORTDbits.RD0 = 1;
            __delay_ms(1.5);
            PORTDbits.RD0 = 0;
            __delay_ms(18.5);
        }
}


//funcion para movimiento del servo2 a 0°
void servo2_19(void)
{
    for (servo2_1 = 0; servo2_1 <= 15; servo2_1++)
        {
            PORTDbits.RD1 = 1;
            __delay_ms(1);
            PORTDbits.RD1 = 0;
            __delay_ms(19);
        }
}

//funcion para movimiento del servo2 a 45°
void servo2_18(void)
{
    for (servo2_2 = 0; servo2_2 <= 15; servo2_2++)
        {
            PORTDbits.RD1 = 1;
            __delay_ms(1.5);
            PORTDbits.RD1 = 0;
            __delay_ms(18.5);
        }   
}

//funcion para el movimiento del servo3 a 0°
void servo3_19(void)
{
    for (servo3_1 = 0; servo3_1 <= 15; servo3_1++)
        {
            PORTDbits.RD2 = 1;
            __delay_ms(1);
            PORTDbits.RD2 = 0;
            __delay_ms(19);
        }
}

//funcion para el movimiento del servo3 a 45°
void servo3_18(void)
{
    for (servo3_2 = 0; servo3_2 <= 15; servo3_2++)
        {
            PORTDbits.RD2 = 1;
            __delay_ms(1.5);
            PORTDbits.RD2 = 0;
            __delay_ms(18.5);        
        }
}

//funcion para transladar datos recibidos
char recepcion_rx()
{
    return RCREG;       //el valor de RCREG lo pongo en ese caracter
}

//funcion para transmitir datos a interfaz
void transmision_tx(char data)
{
    while(TXSTAbits.TRMT == 0)
    {
        TXREG = data;
    }
}

//funcion con cadena de caracteres
void USART_Cadena(char *str)
{
    while(*str != '\0')
    {
        transmision_tx(*str);
        str++;
    }
}

//funcion para escritura en EEPROM
void writeToEEPROM(char data, int address)
{
    EEADR = address;             //se seleccion direccion a escribir
    EEDATA=data;                 //se asgina dato a escribir
    
    EECON1bits.EEPGD = 0;       //se accede a la memoria EEPROm
    EECON1bits.WREN = 1;        //se habilita escritura
    INTCONbits.GIE =0;          //se deshabilitan interrupciones
    
    EECON2 = 0x55;              //escritura de valores por defecto  
    EECON2 = 0x0AA;              //escritura de valores por defecto
    
    EECON1bits.WR =1;           //inicializa la escritura
    INTCONbits.GIE =1;          //se vuelve a habilitar interrupciones
    
    //while(PIR2bits.EEIF==0);
    PIR2bits.EEIF=0;
    
    EECON1bits.WREN = 0;        //se deshabilita escritura   
}

//funcion para lectura en EEPROM
char readFromEEPROM(unsigned address)
{
    EEADR =address;             //se selecciona direccion a leer
    EECON1bits.EEPGD = 0;       //se accede a la memoria EEPROm
    EECON1bits.RD=1;            //inicializando la lectura
    return EEDATA;
    
}

//muy probable que me lo vuele, pero lo dejo por si me sirve
void servos_loop(void)
{
    for(x=0;x<=7;x++)
    {
        if (x == 1)                 //para servo1 a 0°
        {
            for (servo1_1 = 0; servo1_1 <= 15; servo1_1++)
            {
                PORTDbits.RD0 = 1;
                __delay_ms(1);
                PORTDbits.RD0 = 0;
                __delay_ms(19);
            }
        }

        if (x == 2)                 //para servo1 a 45°
        {
            for (servo1_2 = 0; servo1_2 <= 15; servo1_2++)
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
}
