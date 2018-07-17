#include "BH1750.h"

void iniciar_sensor()
{
	twi_start_cond();
	i2c_write_byte(BH1750L_WRITE);
	i2c_write_byte(PWR_on);
	twi_stop_cond();
}

void config_HRcont()
{
	twi_start_cond();
	i2c_write_byte(BH1750L_WRITE);
	i2c_write_byte(mode_HRcont);
	twi_stop_cond();
}

unsigned int leer_valor_lux()
{
	unsigned int bytes;
	char dataH, dataL;
	twi_start_cond();
	i2c_write_byte(BH1750L_READ);
	dataH=i2c_read_byte2(1);	//lectura parte alta del valor (ack=1)
	dataL=i2c_read_byte2(0);	//lectura parte baja del valor (ack=0)
	twi_stop_cond();
	bytes=(int)dataH;	//paso la parte alta
	bytes<<=8;			//roto 8 posiciones
	bytes|=(int)dataL;	//lo uno con la parte baja
	return bytes;
}

unsigned int lux_conv(unsigned int lx)
{
	lx=(unsigned int)lx/1.2;
	return lx;
}
