/*---------------Proyecto final, Programacion de microcontroladores----------------
----------------------------- Andy Bonilla, 19451 ---------------------------------
-------------Interfaz grafica para control y monitoreo de proyecto----------------*/


/*----------------------------------------------------------------------------------
-------------------------------librerias a implementar------------------------------
----------------------------------------------------------------------------------*/
import processing.serial.*;

/*----------------------------------------------------------------------------------
---------------------------inicializacion de puerto serial--------------------------
----------------------------------------------------------------------------------*/
Serial myPort;

/*----------------------------------------------------------------------------------
---------------------------definicion de variables a implementar--------------------
----------------------------------------------------------------------------------*/
int dato_transmitido=0;    //variable para mandar al PIC

/*----------------------------------------------------------------------------------
-----------------------------definicion de cadenas a implementar--------------------
----------------------------------------------------------------------------------*/
char boton1[] = {100,200,60};             //punto en x, punto en y,tamaño0
char led1[] = {150,100,50};                         //puntox, puntoy,tamaño

/*----------------------------------------------------------------------------------
---------------------------------implementacion de funciones------------------------
----------------------------------------------------------------------------------*/

//--------------------------------configuracion general-----------------------------
void setup()
{
//Configuracion de puertos seriales
//String puerto = Serial.list()[0];                 //declaraccion de puerto USB para USARTcom
//myPort = new Serial(this, puerto, 9600);          //configuracion de puerto y braudeaje
  
size(550,780);                                      //tamaño de 500*500 pixeles
background(100,100,100);                            //color de fondo
textSize(20);                                       //tamaño de texto en pantalla
text("Proyecto final de Programacion", 120, 50);
text("de Micrcontroladores",160,80);


}

//--------------------------funcion de comunicacion serial----------------------------

void comunicacion_serial()
{
  fill(300,300,300);
  //rect(slider1[0],2*slider1[1],300,100);
}

//----------------------------------funcion de dibujo------------------------------------
void draw()
{
  
  fill(300,300,300);
  //square(boton1[0],boton1[1],boton1[2]);
  
  rect(boton1[0]-50,2.6*boton1[1],350,100);
  textSize(15);                                       //tamaño de texto en pantalla
  text("Valores UART",boton1[0]+90, 650);
  
  //DIBUJO DE BOTONES PARA SERVO 1
  //a 0°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 1, 0°",boton1[0]-2, boton1[1]-10);
  square(boton1[0],boton1[1],boton1[2]);
  //a 45°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 1, 45°",boton1[0]-2, boton1[1]+105);
  square(boton1[0],boton1[1]+120,boton1[2]);
  
  //DIBUJO DE BOTONES PARA SERVO 2
  //a 0°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 2, 0°",boton1[0]+120, boton1[1]-10);
  square(boton1[0]+120,boton1[1],boton1[2]);
  //a 45°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 2, 45°",boton1[0]+118, boton1[1]+105);
  square(boton1[0]+120,boton1[1]+120,boton1[2]);
  
  
  //DIBUJO DE BOTONES PARA SERVO 3
  //a 0°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 3, 0°",boton1[0]+240, boton1[1]-10);
  square(boton1[0]+240,boton1[1],boton1[2]);
  //a 45°
  fill(300,300,300);                                  //
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 2, 45°",boton1[0]+236, boton1[1]+105);
  square(boton1[0]+240,boton1[1]+120,boton1[2]);
  
  
  /*
  rect(slider1[0],slider1[1],slider1[2],slider1[3]);  
  square(led1[0],led1[1],led1[2]);
  if (mousePressed)
  {
    if (mouseX > slider1[0] && mouseX < slider1[0]+slider1[2])
    {
      if (mouseY > slider1[1] && mouseY < slider1[1]+slider1[3])
      {
        slider_var1 = mouseY-100;
        fill(slider_var1,0,0);
        square(led1[0],led1[1],led1[2]);
      }
    }
  }*/
  
  
//----fin de implementacion de funciones  
}
