#ifndef BH1750_H_
#define BH1750_H_

#include "TWI_master.h"

/*Adress del sensor (adress L) - Lectura/Escritura*/
#define BH1750L_WRITE 0x46
#define BH1750L_READ 0X47

/*Instrucciones de configuración del módulo*/
#define PWR_on 0x01
#define PWR_off 0x00
#define reset 0x07
#define mode_HRcont 0x10 //modo de alta resolución lectura continua

/*PROTOTIPOS DE FUNCIONES LUXÓMETRO*/
void iniciar_sensor();
void config_HRcont();
unsigned int leer_valor_lux();
unsigned int lux_conv(unsigned int lux);


#endif /* BH1750_H_ */
