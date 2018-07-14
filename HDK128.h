/*
 * HDK128.h
 *
 *  Created on: 14/08/2015
 *      Author: Ing. Cristian Conejeros - Ing. Damian Corbalan
 *      Brief: Librerias con definiciones de placa de desarrollo HDK128 desarrollada en el Laboratorio Abierto UTN-FRA
 *      Nota: Muchas de las definicioens en este archivo son de caracter simbolico para tener la descripcion del hardware
 *      y no son de uso directo, por ejemplo SPI, I2C, UART
 *
 */

#ifndef HDK128_H_
#define HDK128_H_
#include <avr/io.h>
#include <avr/interrupt.h>

/******************************** Tipos de Variables**********************************/
typedef struct
    {
	unsigned char B0:1;
	unsigned char B1:1;
	unsigned char B2:1;
	unsigned char B3:1;
	unsigned char B4:1;
	unsigned char B5:1;
	unsigned char B6:1;
	unsigned char B7:1;
    }bit_t;			//variable tipo bit, se utiliza para tratar a una variable de 8 bits de a un bit

/******************************** Macros de Manejo de BIT **********************************/
#define REGBIT(reg,bit)		((volatile bit_t*)&reg)->B##bit	//Macro para manejar registros de a bit
/*******************************************************************************************/

/******************************** Definiciones de conexiones **********************************/
/**** DISPLAY 7 SEGMENTOS ****/
#define _7SEGPORT	PORTA				//Puerto de salida del display 7 segmentos
#define _7SEG_PORT_DIR	DDRA				//Registro de salida del display 7 segmentos
#define _7SEGA 		REGBIT(_7SEGPORT,0)		//IC 7511 - A
#define _7SEGB 		REGBIT(_7SEGPORT,1)		//IC 7511 - B
#define _7SEGC 		REGBIT(_7SEGPORT,2)		//IC 7511 - C
#define _7SEGD 		REGBIT(_7SEGPORT,3)		//IC 7511 - D
#define DISP1  		REGBIT(_7SEGPORT,4)		//ENA display DIG1 - Unidad de Mil
#define DISP2  		REGBIT(_7SEGPORT,5)		//ENA display DIG2 - centena
#define DISP3  		REGBIT(_7SEGPORT,6)		//ENA display DIG3 - decena
#define DISP4  		REGBIT(_7SEGPORT,7)		//ENA display DIG4 - unidad
#define _7SEGPUNTO	REGBIT(PORTG,4)
#define _7SEG_DIR_CONF	_7SEG_PORT_DIR=0xff;DDRG|=1<<4

#define _7SEG_DAT_OUT(valor) _7SEGPORT=(_7SEGPORT&0xF0)|(valor&0x0F)	//Macro para escribir el numero de a digitos en bcd

/*************************************/

/***************** LEDS ***************/
#define LED_1		REGBIT(PORTD,4)
#define LED_2		REGBIT(PORTD,5)
#define LED_3		REGBIT(PORTD,6)
#define LED_4		REGBIT(PORTD,7)
#define LEDS_DIR_OUT	DDRD|=0xF0		//Habilita a los leds como salidas
/*************************************/

/************ PULSADORES ************/
#define SW1		REGBIT(PINE,5)
#define SW2		REGBIT(PINE,6)
#define SW3		REGBIT(PINE,7)
#define SW_DIR_IN	DDRE&=0x1F		//Habilita a los pulsadores como entradas
/*************************************/

/************ DISPLAY LCD ************/		//Definiciones para utilizar la libreria
#define RS_LCD		0
#define RD_LCD		1
#define	EN_LCD		2
#define D4_LCD		4
#define	D5_LCD		5
#define	D6_LCD		6
#define	D7_LCD		7
#define DIR_LCD		DDRC
#define PUERTO_LCD_OUT	PORTC
#define BLGT		REGBIT(PORTG,2)
#define BLGT_DIR	REGBIT(DDRG,2)
/*************************************/

/************ Memoria SD ************/
#define SD_CS		REGBIT(PORTB,0)
#define SD_CS_DIR	REGBIT(DDRB,0)
#define SD_SCK		REGBIT(PORTB,1)		//Definicion Simbolica, usar SPI del micro
#define SD_MOSI		REGBIT(PORTB,2)		//Definicion Simbolica, usar SPI del micro
#define SD_MISO		REGBIT(PORTB,3)		//Definicion Simbolica, usar SPI del micro
/*************************************/

/***********Comunicacion SPI**********/
#define SPI_CS2		REGBIT(PORTG,3)		//CHip Select, para dispositivo SPI externo
#define SPI_CS2_DIR	REGBIT(DDRG,3)
#define SPI_SCK		REGBIT(PORTB,1)		//Definicion Simbolica, usar SPI del micro
#define SPI_MOSI	REGBIT(PORTB,2)		//Definicion Simbolica, usar SPI del micro
#define SPI_MISO	REGBIT(PORTB,3)		//Definicion Simbolica, usar SPI del micro
/*************************************/

/**************** RTC ****************/
#define SQW_PIN		REGBIT(PORTE,4)		//Definicion Simbolica, usar i2c del micro
#define SDA_PIN		REGBIT(PORTD,1)		//Definicion Simbolica, usar i2c del micro
#define SCL_PIN		REGBIT(PORTD,0)		//Definicion Simbolica, usar i2c del micro
/*************************************/

/*********SALIDA Open Colector********/
#define OPC01		REGBIT(PORTB,7)		//OC2/OC1C salidas de PWM
#define OPC02		REGBIT(PORTB,6)		//OC1B 	   Salidas de PWM
#define OPC03		REGBIT(PORTB,5)		//OC1A	   Salidas de PWM
#define OPC04		REGBIT(PORTB,4)		//OC0	   Salida de PWM
#define OPC05		REGBIT(PORTB,3)		//MISO - Si se usa SD no estara disponible como salida
#define OPC06		REGBIT(PORTB,2)		//MOSI - Si se usa SD no estara disponible como salida
#define OPC07		REGBIT(PORTB,1)		//SCK  - Si se usa SD no estara disponible como salida
#define OPC08		REGBIT(PORTB,0)		//SS   - Si se usa SD no estara disponible como salida

#ifdef USO_SD		//Si se usa Memoria SD solo se tienen disponibles 4 salidas
#define OPC_DAT_OUT_DIR	DDRB|=0xF0
#else			//Si no se usa Memoria SD se pueden usar todos los pines
#define OPC_DAT_OUT_DIR	DDRB=0xFF
#endif
/*************************************/

/************* POTENCIOMETRO ANALOGICO ***************/
#define POTENCIOMETRO	3	//PINF,3 Canal de ADC3, SE DEBEN UTILIZAR RUTINAS DE ADC
/*****************************************************/

/*************** SENSOR DE TEMPERATURA ***************/
#define LM35_SENSOR_T	2	//PINF,2 Canal de ADC2, SE DEBEN UTILIZAR RUTINAS DE ADC
/*****************************************************/

/*************** ENTRADAS ANALOGICAS *****************/
#define ADC_IN0		0	//PINF,0 Canal de ADC0, SE DEBEN UTILIZAR RUTINAS DE ADC
#define ADC_IN1		1	//PINF,0 Canal de ADC0, SE DEBEN UTILIZAR RUTINAS DE ADC
/*****************************************************/

/************** COMPARADOR ANALOGICO *****************/
#define AIN_IN0		REGBIT(PORTE,4)		//Definicion Simbolica
#define AIN_IN1		REGBIT(PORTE,5)		//Definicion Simbolica
/*****************************************************/

/********************* USART USB **********************/
#define uRX		REGBIT(PORTE,0)		//Definicion Simbolica
#define uTX		REGBIT(PORTE,1)		//Definicion Simbolica
#define USART_USB_RX	USART0_RX_vect
#define USART_USB_TX	USART0_TX_vect
/*****************************************************/

/********************* USART RS232 **********************/
#define RS232_RX	REGBIT(PORTD,2)		//Definicion Simbolica
#define RS232_TX	REGBIT(PORTD,3)		//Definicion Simbolica
#define USART_RS232_RX	USART1_RX_vect
#define USART_RS232_TX	USART1_TX_vect
/*****************************************************/

/*****************************************************/
#endif /* HDK128_H_ */
