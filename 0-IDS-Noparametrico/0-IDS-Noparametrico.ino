//*************************************************************//
// 0-IDS-noparametrico.ino
//*************************************************************//
// Fac. de Ing. Mecánica y Eléctrica, 
// Universidad Autónoma de Nuevo León
// FIME-UANL
// Coordinación de Electrónica
//------------------------------------------------------------//
//------------------------------------------------------------//
//
// Programa para aplicar una entrada específica:
//  a) ruido (descomenar la linea 154 => 
//      "R  = Habilitado*(0.0001*random(1,50000));")
//  b) escalón (descomentar la linea 155 => 
//      "R  = Habilitado*(5);")
//  c) función sen(wt), (descomenar la linea 156 [ajustar "omega" en linea 86] =>
//      "R = Habilitado*(2.5+2*W1);")
// a un circuito RC-RC y hacer la adquisición de los datos 
// entrada-salida [u(t), y(t)]. Para guardar los datos en un 
// archivo hace falta utilizar un programa monitor.exe
// (alternativamente se puede correr el programa en python 2.7
// mediante "python monitor.py")
// 
//------------------------------------------------------------//
// Programa modificado de 0-Lazo-Abierto.ino de Git-Hub del 
// Dr. Miguel Platas: 
// "https://github.com/miplatas/lab-control-fime/tree/main/Arduino/0-Lazo-Abierto" 
//
// Modificación:
//  13/02/2023
//*************************************************************//
//*************************************************************//

// ******************************************************** //
//----------  Muestreo  --------//                          //
// ******************************************************** //
  unsigned long TS = 50;      // Muestreo TS miliseg        //
  float Tseg = 0.05;          // Muestreo en Tseg segundos  //
// ******************************************************** //

                             // Fin de parámetros modificables

// ******************************************************** //
//----------  Constantes  --------//                        //
// ******************************************************** //

//---------- Definición Pines IO analogicos--------//                  
  #define pR 0                // pin de referencia             
  #define pX1 1               // pin de estado 1   
  #define pX2 2               // pin de estado 2                     
  #define pU 10               // pin de salida de control (entrada a planta)      

//---------- Definición Pines IO discretos--------//                  
  #define pLED8 8             // LED 8 en tarjeta
  #define pLED9 9             // LED 9 en tarjeta   
  #define pSW2 2              // SW 2 en tarjeta                                    
  #define pSW3 3              // SW 3 en tarjeta      

//---------- Escalamientos para analogicas de 0 a 5 V --------//
  #define mX 0.004882813      // Pendiente 0-1023 -> 0 - 5
  #define bX 0                // Ajuste cero para 0-1023 -> 0 - 5
  #define mU 51               // Pendiente 0 - 5 -> 0 - 255
  #define bU 0                // Ajuste cero 0 - 5 -> 0 - 1023

// ******************************************************** //
//----------  Variables globales  --------//                //
// ******************************************************** //

//---------- Tiempo --------//
  unsigned long TS_code = 0;  // Tiempo que tarda programa
  unsigned long TIC = 0;      // Estampa de tiempo inicio ciclos
  unsigned long TC = 0;       // Faltante para TS
   
//----------  Señales --------//
  float R = 0;                // Referencia
  float Y = 0;                // Salida
  float X1 = 0;               // Estado 1
  float X2 = 0;               // Estado 2
  float U = 0;                // Salida control
  int Ui = 0;                 // Salida control tarjeta 

//---------- Otros --------//
  bool Habilitado = 0;        // Señal {0,1} para entradas escalón
// ** Generación de entrada senoidal ** //
// - - Definiciones - - //
  #define omega 0.02       // Frecuencia de la señal senoidal
  float W1 = 0; // Estado exosistema 1 en k, W1[k]
  float W2 = 1; // Estado exosistema 2 en k, W2[k]
  float WN1 = 0; // Estado exosistema 1 en k+1, W1[k+1]
  float WN2 = 0; // Estado exosistema 2 en k+1, W2[k+1]

// ******************************************************** //
//----------  Librerias  --------//                      //
// ******************************************************** //
  #include <SoftwareSerial.h>

// ******************************************************** //
//---------- Rutinia de inicio --------//                   //
// ******************************************************** //
void setup() {
  //--Inicia serial--//
  Serial.begin(9600);

  //--Configura pines digitales--//  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

// ******************************************************** //
//---------- Rutinia principal  --------//                  //
// ******************************************************** //
void loop() {                     
  proc_entradas();                    // Procesamiento de Entradas
  control();                          // Control
  proc_salidas();                     // Procesado de Salidas
  // coms_arduino_ide();               // Comunicaciones
  coms_python(0,&Y,&U);
//  coms_python(&R,&Y,&U);
  espera();
}


// ******************************************************** //
//---------- Rutinias de control y observador--------       //                          
// ******************************************************** //
//-- Control --//
void control(){
  // Ley de control
    U = R;       // La referencia se aplica a la entrada de la planta
   
  // Saturacion
  if(U >= 5.0) U = 5.0;               // Saturacion de control en rango 0 a 5V                      
  else if(U < 0) U = 0;
}


// ******************************************************** //
//---------- Rutinias de IO y control de tiempo     --------//                          
// ******************************************************** //
//-- Procesado de entradas --//
void proc_entradas(){
//-- Obtención de la función senoidal --//
  WN1 = W1 + Tseg*omega*W2;
  WN2 = W2 - Tseg*omega*W1;
  W1 = WN1;
  W2 = WN2;
  // Lectura de los estados del modelo
  X1 = analogRead(pX1)*mX+bX;               // Lectura de salida de planta en pin pX3
  X2 = analogRead(pX2)*mX+bX;               // Lectura de salida de planta en pin pX2
  // Se cambia la lectura de un potencipmetro y se asigna valores específicos
  // ** R = Habilitado*(analogRead(pR)*mX+bX);    // Lectura de referencia en pin pR, 
  // R  = Habilitado*(0.0001*random(1,50000));  // Referencia para analisis de correlacion
  // R  = Habilitado*(4);   // Entrada para realizar respuuesta al pulso
  // R  = Habilitado*(2.5+2*W1);   // Entrada para realizar respuuesta al pulso
  Y = X2;
}


//-- Procesado de salidas --//
void proc_salidas(){
  Ui = int(U*mU+bU);                    // Escalamiento
  analogWrite(pU, Ui);                  // Salida PWM en pin pU
  botonesyleds();                       // Manejo de IO discretas
}


//-- Memoria {0,1} para entrada escalón --//
void botonesyleds(){

  static int n = 0; 
  if(n >= 1000/TS) n=0;                                // Señal cuadrada para led blink
  else n = n+1;

  if(digitalRead(pSW2) == 1) Habilitado = 1;      // Memoria on/off en Habilitado
  else if(digitalRead(pSW3) == 1) Habilitado = 0; // Set con SW2. Reset con SW3

  if(n >= 500/TS && Habilitado == 1) digitalWrite(pLED8,HIGH); // Led blink en LED8
  else digitalWrite(pLED8, LOW);                           // Cuando Habilitado = 1

  if(Habilitado == 0) digitalWrite(pLED9,HIGH);            // LED9 = 1
  else digitalWrite(pLED9, LOW);                           // Cuando Habilitado = 0
}


//-- Para muestreo uniforme --//
void espera(){   
  TS_code = millis()- TIC;                 // Tiempo de ciclo
  TC = TS - TS_code;                       // Calcula faltante para TS
  if (TS_code < TS) delay(TC);             // Espera para completar ciclo de TS   
  TIC = millis();
}


//-- Comunicación con monitor --//
void coms_arduino_ide(){  
  Serial.print("y_d(t):");            // Referencia
  Serial.print(R);                    // Referencia
  Serial.print(",");                  // Separador     
  Serial.print("y(t):");              // Salida
  Serial.println(Y);                  // Salida (terminar con "serial.println")
}


void coms_python(float* Rp, float* Yp, float* Up)
{
  byte* byteData1 = (byte*)(Rp);
  byte* byteData2 = (byte*)(Yp);
  byte* byteData3 = (byte*)(Up);
  byte buf[12] = {byteData1[0], byteData1[1], byteData1[2], byteData1[3],
                 byteData2[0], byteData2[1], byteData2[2], byteData2[3],
                 byteData3[0], byteData3[1], byteData3[2], byteData3[3]};
  Serial.write(buf, 12);
}
