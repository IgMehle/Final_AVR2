#include <stdio.h>
#include "avr2final_defs.h"
#include "HDK128.h"
#include "TWI_master.h"
#include "BH1750.h"
#include "lcd_alfa.h"

extern volatile bit_t flags;
extern volatile char modo;
extern volatile char estado[];
extern volatile char brillo;
extern volatile char aux_bri[];
extern volatile unsigned int lux;
extern volatile float aux_lux;

void manual()
{
	if(flag_man)
	{
		enable_int_uart1_RX;
		enable_INT4;
		enable_INT5;
		flag_man=0;
	}

	if(flag_modo)
	{
		show_UART1_modo(modo);
		flag_modo=0;
	}

	if(flag_est)
	{
		if(!strcmp(estado,"ON:")) sprintf(estado,"OFF");
		else sprintf(estado,"ON:");
		if(!strcmp(estado,"OFF"))
		{
			timer0_comp_disc;		//desconecto la salida de PWM0
			PORTB4_off;				//LED OFF
			show_UART1_estado(estado,brillo);
		}
		else
		{
			timer0_comp_clear;		//regreso al pwm
			show_UART1_estado(estado,brillo);
		}
		flag_est=0;
	}

	if(flag_bri)
	{
		char i=0;
		for(i=0;i<3;i++)
		{
			while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
			aux_bri[i]=UART1_REG;			//uso la variable auxiliar char de brillo
		}
		while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
		enable_int_uart1_RX;			//VUELVO A HABILITAR INT RX
		//brillo=(char)atoi(aux_bri);
		brillo=100*(aux_bri[0]-48);		//paso el valor a decimal y guardo en brillo
		brillo+=10*(aux_bri[1]-48);
		brillo+=(aux_bri[2]-48);
		timer0_comp_value=(char)(brillo*2,55);		//actualizo pwm
		show_UART1_estado(estado,brillo);			//muestro por bluetooth
	}

	if(flag_bot)
	{
		show_UART1_estado(estado,brillo);			//muestro por bluetooth
	}

	if(flag_lux)	//Actualizo el valor de lux y
	{
		off_int_uart1_RX;			//apago la int rx
		lux=leer_valor_lux();
		show_LCD(modo,estado,brillo,lux);	//muestro en el LCD
		enable_int_uart1_RX;		//habilito la int RX
		flag_lux=0;
	}

	if(modo=='A') flag_auto=1;
}

void automatico()
{
	if(flag_auto)				//Solo hace la configuracion la primera vez que entra a la funcion automatico
	{
		off_INT4;
		off_INT5;
		//enable_int_uart1_RX;
		flag_auto=0;
	}

	if(flag_modo)
	{
		show_UART1_modo(modo);
		flag_modo=0;
	}

	if(flag_s)					//transmito el nivel de lux (el que esté actualmente) al recibir el caracter 'S'
	{
		show_UART1_lux(lux);
		flag_s=0;
	}

	if(flag_lux)
	{
		off_int_uart1_RX;			//apago la int rx
		lux=leer_valor_lux();

		//Ajusto el valor de lux a un maximo de 10%
		if(lux>655)
		{
			if(lux<6553) brillo=10;

			aux_lux=(lux)/FACTOR;			//Divido por un factor de 65.535 lo que nos da un float entre 10 y 100
			brillo=110-(char)aux_lux;
			/*casteado como entero (char)
			 * se lo resto a 110 para modificar el brillo de manera lineal inversa al valor de lux */
		}
		else
		{
			brillo=100;
		}
		timer0_comp_value=(char)((brillo*2,55)-2);		//actualizo pwm
		show_LCD(modo,estado,brillo,lux);	//muestro valores en el LCD
		enable_int_uart1_RX;		//habilito la int RX
		flag_lux=0;
	}

	if(modo=='M') flag_man=1;
}

void show_LCD(char modo, char estado[3], char brillo, unsigned int lux)
{
	static char bri_show[]="000",lux_show[]="00000";
	lcd_clrscr();
	lcd_gotoxy(0,0);

	if(modo=='A')
	{
		lcd_puts("AUTO");
	}
	else
	{
		lcd_puts("MANUAL");
	}

	lcd_gotoxy(10,0);
	lcd_puts(estado);
	if(!strcmp(estado,"ON:"))
	{
		sprintf(bri_show,"%d",brillo);
		lcd_puts(bri_show);
	}

	lcd_gotoxy(2,1);
	lcd_puts("LUX:");
	sprintf(lux_show,"%i",lux_conv(lux));
	lcd_puts(lux_show);
}

void show_UART1_modo(char modo)
{
	off_int_uart1_RX;		//apago la int rx
	off_uart1_RX;			//Apago rx
	char aux_modo[]="OFF";
	if(modo=='M') sprintf(aux_modo,"OFF");
	else sprintf(aux_modo,"ON");
	char i=0;					//CONTADOR (GLOBAL)
	enable_uart1_TX;	//habilito tx
	//voy a mandar caracteres hasta que encuentre el caracter nulo, que no lo manda
	while(aux_modo[i]!='\0')
	{
		while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
		UART1_REG=aux_modo[i];			//Envío la cadena
		i++;
	}
	while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
	off_uart1_TX;		//Apago la tx
	enable_uart1_RX;	//vuelvo a habilitar la rx
	enable_int_uart1_RX;		//habilito la int RX
}

void show_UART1_estado(char estado[4], char brillo)
{
	off_int_uart1_RX;		//apago la int rx
	off_uart1_RX;			//Apago rx
	char aux_bri[4], i;
	if(!strcmp(estado,"OFF"))
	{
		enable_uart1_TX;	//habilito tx
		for(i=0;i<3;i++)	//envio 5 caracteres
		{
			while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
			UART1_REG=estado[i];			//Envío la cadena
		}
		while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
	}
	else
	{
		enable_uart1_TX;	//habilito tx
		for(i=0;i<3;i++)	//envio 5 caracteres
		{
			while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
			UART1_REG=estado[i];			//Envío la cadena
		}
		while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
		sprintf(aux_bri,"%d",brillo);		//convierto a cadena de caracteres
		for(i=0;i<3;i++)	//envio 5 caracteres
		{
			while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
			UART1_REG=aux_bri[i];			//Envío la cadena
		}
		while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
	}
	off_uart1_TX;			//Apago la tx
	enable_uart1_RX;		//vuelvo a habilitar la rx
	enable_int_uart1_RX;	//habilito la int RX
}

void show_UART1_lux(unsigned int lux)
{
	off_int_uart1_RX;			//apago la int rx
	off_uart1_RX;						//Apago rx
	char lux_show[]="00000", i;
	sprintf(lux_show,"%i",lux_conv(lux));	//convierto a cadena de caracteres
	enable_uart1_TX;						//habilito tx
	for(i=0;i<5;i++)	//envio 5 caracteres
	{
		while(!get_bit(UCSR1A,UDRE1));		//Bloqueo manual hasta buffer vacío
		UART1_REG=lux_show[i];			//Envío la cadena
	}
	while(!get_bit(UCSR1A,UDRE1));	//Bloqueo manual hasta buffer vacío
	off_uart1_TX;		//Apago la tx
	enable_uart1_RX;	//vuelvo a habilitar la rx
	enable_int_uart1_RX;		//habilito la int RX
}

void UART1_init(unsigned int ubrr)
{
	//Configuración de USART
	UBRR1H=(unsigned char)(MYUBRR>>8);	//set baud rate
	UBRR1L=(unsigned char)(MYUBRR);
	UCSR1B=(1<<RXEN)|(1<<TXEN);			//habilito TX y RX
	UCSR1C=(1<<USBS)|(3<<UCSZ0);		//frame format, datos=8bits, stop=2bits
}


