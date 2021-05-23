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
int slider_var1;
int slider_var2;
int slider_var3;

/*----------------------------------------------------------------------------------
-----------------------------definicion de cadenas a implementar--------------------
----------------------------------------------------------------------------------*/
char slider1[] = {150,200,50,250};                 //puntox, puntoy1,tamaño, puntoy2
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
  
size(550,700);                                      //tamaño de 500*500 pixeles
background(100,100,100);                            //color de fondo
textSize(20);                                       //tamaño de texto en pantalla
text("Proyecto final de Programacion", 120, 50);
text("de Micrcontroladores",160,80);


}

//--------------------------funcion de comunicacion serial----------------------------

void comunicacion_serial()
{
  fill(300,300,300);
  rect(slider1[0],2*slider1[1],300,100);
}

//----------------------------------funcion de dibujo------------------------------------
void draw()
{
  fill(300,300,300);
  rect(slider1[0]-50,2.6*slider1[1],350,100);
  textSize(15);                                       //tamaño de texto en pantalla
  text("Valores UART",slider1[0]+90, 650);
  
  //dibujo de botones para servo1
  fill(300,300,300);        //relleno
  
  
  
  fill(300,300,300);
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 1",slider1[0], 2*slider1[3]);
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
  }
  
  //dibujo de pot2
  fill(300,300,300);
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 2",slider1[0]+100, 2*slider1[3]);
  rect(slider1[0]+100,slider1[1],slider1[2],slider1[3]);  
  square(led1[0]+100,led1[1],led1[2]);
  if (mousePressed)
  {
    if (mouseX > slider1[0]+100 && mouseX < slider1[0]+slider1[2]+100)
    {
      if (mouseY > slider1[1] && mouseY < slider1[1]+slider1[3])
      {
        slider_var2 = mouseY-100;
        fill(slider_var2,0,0);
        square(led1[0]+100,led1[1],led1[2]);
        myPort.write(mouseY-100);
      }
    }
  }
 
  //dibujo de pot3
  fill(300,300,300);
  textSize(15);                                       //tamaño de texto en pantalla
  text("Servo 3",slider1[0]+200, 2*slider1[3]);
  rect(slider1[0]+200,slider1[1],slider1[2],slider1[3]);  
  square(led1[0]+200,led1[1],led1[2]);
  if (mousePressed)
  {
    if (mouseX > slider1[0]+200 && mouseX < slider1[0]+slider1[2]+200)
    {
      if (mouseY > slider1[1] && mouseY < slider1[1]+slider1[3])
      {
        slider_var3 = mouseY-100;
        fill(slider_var3,0,0);
        square(led1[0]+200,led1[1],led1[2]);
      }
    }
  }
//----fin de implementacion de funciones  
}
