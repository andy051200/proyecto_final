/*---------------Proyecto final, Programacion de microcontroladores----------------
----------------------------- Andy Bonilla, 19451 ---------------------------------
-------------Interfaz grafica para control y monitoreo de proyecto----------------*/


/*----------------------------------------------------------------------------------
-------------------------------librerias a implementar------------------------------
----------------------------------------------------------------------------------*/
import processing.serial.*;

/*----------------------------------------------------------------------------------
---------------------------definicion de variables a implementar--------------------
----------------------------------------------------------------------------------*/
Serial myPort;                        //variable objeto para comunicacion serial

byte[] dePic = new byte[6];           //bytes recibidos desde PIC
int [] dePicInt = {0,0,0,0,0,0};        //se desconoce su valor, por eso en 0

byte [] haciaPic = new byte [6];      //byts para mandar a PIC
int [] haciaPicInt = {0,1,0,1,1,0};    //arreglo de datos a mandar al PIC, [servo1_0,servo1_45,servo2_0,servo2_45,servo3_0,servo3_45]

int interval = 100;
int previousMillis = 0;
int currentMillis = 0;
int dato_transmitido=0;    //variable para mandar al PIC
int dato_recibido;         //variable con datos desde el PIC
String puerto[];          //variable tipo string para los datos


/*----------------------------------------------------------------------------------
-----------------------------definicion de cadenas a implementar--------------------
----------------------------------------------------------------------------------*/
int boton1[] = {100,150,60};             //punto en x, punto en y,tamaño0
int led1[] = {150,100,50};                         //puntox, puntoy,tamaño
int boton_eeprom[] = {220,425, 60};              //puntox, puntoy y tamaño

/*----------------------------------------------------------------------------------
---------------------------------implementacion de funciones------------------------
----------------------------------------------------------------------------------*/

//--------------------------------configuracion general-----------------------------
void setup()
{
//------------------Configuracion de puertos seriales
//puerto = Serial.list()[0];                 //declaraccion de puerto USB para USARTcom
myPort = new Serial(this, Serial.list()[0], 9600);          //configuracion de puerto y braudeaje
myPort.buffer(6);
thread("serial_comm");

  
size(550,580);                                      //tamaño de 500*500 pixeles
background(100,100,100);                            //color de fondo
textSize(20);                                       //tamaño de texto en pantalla
text("Proyecto final de Programacion", 120, 50);
text("de Micrcontroladores",160,80);
}

//--------------------------funcion de comunicacion serial----------------------------

void serialEvent(Serial myPort)
{
  myPort.readBytes(dePic);
  for (int i=0;i<6 ;i++)
  {
    dePicInt[i] = dePic[i] & 0xFF;
  }
  
  
  //inString = p.readString();
  /*int input = myPort.read();
  datosPIC[cont] = input;
  cont++; 
  if (cont > 1){
     valpot = datosPIC[0];
     delay(5);
     println(valpot);
     //myPort.write('A');
     cont = 0;
     send = bit1 + bit2 + bit3 + bit4 + bit5;
     myPort.write(send);*/
  
  //fill(300,300,300);
  //rect(slider1[0],2*slider1[1],300,100);
}

void serial_comm()
{
  while (true)
  {
    currentMillis = millis();
  
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      for (int i = 0; i<6;i++)
      {
        haciaPic[i]=byte(haciaPicInt[i]);
      }
      myPort.write(haciaPic);
    }
  }
}

//----------------------------------funcion de dibujo------------------------------------
void draw()
{
  println("---------------------------------------------");
  println("ESTADO DE LOS SERVO MOTORES EN EL CIRCUITO");
  println("---------------------------------------------");
  println("El servo 1 está en:");       //texto de referencia
  println(dePicInt[0]);                 //estado del servo 1
  println("El servo 2 está en:");      //texto de referencia
  println(dePicInt[1]);                 //estado del servo 2
  println("El servo 3 está en:");      //texto de referencia
  println(dePicInt[2]);                //estado del servo3
  
  
  
  //idealmente tambien debería mostrar los valores que se estan modificando de la eeprom y demas
  
  
  fill(1300,1300,1300);
  //square(boton1[0],boton1[1],boton1[2]);
  
  /*rect(boton1[0]-50,2.6*boton1[1],400,100);
  textSize(15);                                       //tamaño de texto en pantalla
  text("Valores UART recibidos",boton1[0]+90, 650);*/
  
  //------------------------DIBUJO DE BOTONES PARA SERVO 1
  //servo1 a 0°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 1, 0°",boton1[0]-2, boton1[1]-10);
  square(boton1[0],boton1[1],boton1[2]);
  if (mousePressed)
  {
    if (mouseX > boton1[0] && mouseX < boton1[0]+boton1[2])
    {
      if (mouseY > boton1[1] && mouseY < boton1[1]+boton1[2])
      {
        fill(255,0,0);
        square(boton1[0],boton1[1],boton1[2]);
        dato_transmitido=1;
        myPort.write(dato_transmitido);       //valor
      }
    }
  }
  
  //servo 1 a 45°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 1, 45°",boton1[0]-2, boton1[1]+105);
  square(boton1[0],boton1[1]+120,boton1[2]);
  if (mousePressed)
  {
    if (mouseX > boton1[0] && mouseX < boton1[0]+boton1[2])
    {
      if (mouseY > boton1[1]+120 && mouseY < boton1[1]+boton1[2]+120)
      {
        fill(255,0,0);
        square(boton1[0],boton1[1]+120,boton1[2]);
        dato_transmitido=2;
        myPort.write(dato_transmitido);
      }
    }
  }
  
  
  //------------------------DIBUJO DE BOTONES PARA SERVO 2
  //servo 2 a 0°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 2, 0°",boton1[0]+120, boton1[1]-10);
  square(boton1[0]+120,boton1[1],boton1[2]);
  if (mousePressed)
  {
    if (mouseX > boton1[0]+120 && mouseX < boton1[0]+boton1[2]+120)
    {
      if (mouseY > boton1[1] && mouseY < boton1[1]+boton1[2])
      {
        fill(255,0,0);
        square(boton1[0]+120,boton1[1],boton1[2]);
        dato_transmitido=3;
        myPort.write(dato_transmitido);
      }
    }
  }
  //servo 2 a 45°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 2, 45°",boton1[0]+118, boton1[1]+105);
  square(boton1[0]+120,boton1[1]+120,boton1[2]);
  if (mousePressed)
  {
    if (mouseX > boton1[0]+120 && mouseX < boton1[0]+boton1[2]+120)
    {
      if (mouseY > boton1[1]+120 && mouseY < boton1[1]+boton1[2]+120)
      {
        fill(255,0,0);
        square(boton1[0]+120,boton1[1]+120,boton1[2]);
        dato_transmitido=4;
        myPort.write(dato_transmitido);
      }
    }
  }
  
  //------------------------DIBUJO DE BOTONES PARA SERVO 3
  //servo 3 a 0°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 3, 0°",boton1[0]+240, boton1[1]-10);
  square(boton1[0]+240,boton1[1],boton1[2]);
  if (mousePressed)
  {
    if (mouseX > boton1[0]+240 && mouseX < boton1[0]+boton1[2]+240)
    {
      if (mouseY > boton1[1] && mouseY < boton1[1]+boton1[2])
      {
        fill(255,0,0);
        square(boton1[0]+240,boton1[1],boton1[2]);
        dato_transmitido=5;
      }
    }
  }
  
  //servo 3 a 45°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 3, 45°",boton1[0]+236, boton1[1]+105);
  square(boton1[0]+240,boton1[1]+120,boton1[2]);
  if (mousePressed)
  {
    if (mouseX > boton1[0]+240 && mouseX < boton1[0]+boton1[2]+240)
    {
      if (mouseY > boton1[1]+120 && mouseY < boton1[1]+boton1[2]+240)
      {
        fill(255,0,0);
        square(boton1[0]+240,boton1[1]+120,boton1[2]);
        dato_transmitido=6;
      }
    }
  }
  
  //------------------------DIBUJO DE BOTON PARA DATOS EN EEPROM
  fill(300,300,300);
  textSize(15);
  text("Guardar en EEPROM", boton_eeprom[0]-32,boton_eeprom[1]-5);
  square(boton_eeprom[0],boton_eeprom[1], boton_eeprom[2]);
  if (mousePressed)
  {
    if (mouseX > boton_eeprom[0] && mouseX < boton_eeprom[0]+boton_eeprom[2] )
    {
      if (mouseY > boton_eeprom[1] && mouseX < boton_eeprom[1]+boton_eeprom[2])
      {
        fill(255,0,0);
        square(boton_eeprom[0],boton_eeprom[1], boton_eeprom[2]);
        //poner variable que va a 
        dato_transmitido=7;
      }
    }
  }
  
  
 
println(dato_transmitido);
}
