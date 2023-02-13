# Practicas de identificación

## Tabla de contenidos
* [Información general](#Información general)
* [Descripción del Hardware usado](#Descripción del Hardware)
* [Descripción de las variables](#Descripción de las variables)

## Información general
Se presenta el codigo para realizar diversas prácticas de identificación de sistemas.
El desarrollo esta basado en Arduino-UNO y un circuito que consta de dos resitencias y dos capacitores, como se muestra en la figura. 
![image](https://user-images.githubusercontent.com/118855539/218335409-9f37482f-e839-41c5-9f1c-64b2c9032c8b.png)

## Descripción del Hardware 
Se utilizó un protoboar con dos resistencias y dos capacitores, dos interruptores momentaneos normalmente abiertos así como dos resistencias 
para dar el nivel de voltaje a ias entradas de Arduino.
![image](https://user-images.githubusercontent.com/118855539/218335623-c65309eb-b717-4607-ba09-dde551481a08.png)

## Descripción de las variables
Las entradas y salidas del sistema están conectadas a los siguientes pines.

Entradas analógicas

* Pin A1. Voltaje en el capacitor 1, denotado por X1 en el software y por 
 en la descripción del algoritmo.
* Pin A2. Voltaje en el capacitor 2, denotado por X2 en el software y por 
 en la descripción del algoritmo.

Salidas analógicas (PWM)
* Pin 10. Señal de control, denotado por U en el software y por 
 en la descripción del algoritmo.

Entradas discretas
* Pin 2. Entrada de arranque, denotado por SW2 en el software.
* Pin 3. Entrada de paro, denotado por SW3 en el software.
