#include <stdio.h>
#include "avr2final_defs.h"
#include "HDK128.h"
#include "TWI_master.h"
#include "BH1750.h"
#include "lcd_alfa.h"

volatile int cont=0;
volatile bit_t flags;

volatile char modo='A';
volatile char estado[]="ON:";
volatile char brillo=50;
volatile char aux_bri[4];
volatile unsigned int lux=0;
volatile float aux_lux=0;

/*Interrupción TIMER 2 contador 100-300mseg*/

ISR(TIMER2_COMP_vect)
{
	cont++;
	/*if(cont==500)
		flag_lcd=1;	//Habilito la reescritura del lcd solo despues de aprox 100ms
	*/
	if(cont==1500)	//Cada 300ms
	{
		flag_lux=1;		//Permite actualizar valor lux
		cont=0;
	}
}

/*Interrupción BOTON DECREMENTAR BRILLO*/

ISR(INT4_vect)
{
	if(Get_PINE4)
	{
		if(brillo>10)
		{
			brillo--;
			timer0_comp_value=(char)((brillo*2,55)-2);		//actualizo pwm
		}
	}
}

/*Interrupción BOTON INCREMENTAR BRILLO*/

ISR(INT5_vect)
{
	if(Get_PINE5)
	{
		if(brillo<100)
		{
			brillo++;
			timer0_comp_value=(char)((brillo*2,55)-2);		//actualizo pwm
		}
	}
}

/*Interrupción RECEPCIÓN COMPLETA USART1*/

ISR(USART1_RX_vect)
{
	switch(UART1_REG)
	{
		case 'A':
			flag_modo=1;
			if(modo=='A') modo='M';
			else modo='A';
			break;

		case 'L':
			if(modo=='M') flag_est=1;	//solo modo manual
			break;

		case 'B':
			if(modo=='M')	//solo modo manual
			{
				flag_bri=1;
				off_int_uart1_RX;	//BLOQUEO INT RX
			}
			break;

		case 'S':
			flag_s=1;
			break;

		default:
			break;
	}
}

/****CUERPO PRINCIPAL***/

int main()
{
	PortB4_Output;			//Habilito salida OC0
	PinE4_Input;			//BOTON DESCONTAR BRILLO
	PinE5_Input;			//BOTON INCREMENTAR BRILLO

	//flag_lcd=1;
	flag_man=0;
	flag_auto=1;			//arranca en automático
	flag_lux=0;				//luxometro en 0
	flag_bot=0;
	flag_modo=0;
	flag_est=0;
	flag_bri=0;
	flag_s=0;

	lcd_init(LCD_DISP_ON);		//inicializo el display lcd
	twi_init();					//inicializo el puerto i2c
	iniciar_sensor();			//configuracion de inicio bh1750
	config_HRcont();			//configuracion high res mode bh1750
	UART1_init(MYUBRR);			//configuracion uart

	timer0_mode_fastpwm;		//Configuración TMR0
	timer0_prescaler_8;
	timer0_comp_clear;			//salida OC0 clear on compare match
	timer0_comp_value=130;		//Cargo estado inicial 50% en compare

	timer2_mode_compare;		//Configuración TMR2
	timer2_prescaler_8;
	timer2_compare_value=200;	//Compara cada 200us

	enable_int_tmr0cmp;			//habilito y configuro interrupciones
	enable_int_tmr2cmp;
	INT4_falling_edge;
	INT5_falling_edge;
	sei();

	/******BUCLE DE FUNCIONAMIENTO (Máquina de Estados)******/

	while(1)
	{
		switch(modo)
		{
			case 'A':
				automatico();
				break;
			case 'M':
				manual();
				break;
			default:
				automatico();
				break;
		}
	}
	return 0;
}

