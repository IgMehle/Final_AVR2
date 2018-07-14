################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BH1750.c \
../TWI_master.c \
../avr2final_funciones.c \
../avr2final_main.c \
../lcd_alfa.c 

OBJS += \
./BH1750.o \
./TWI_master.o \
./avr2final_funciones.o \
./avr2final_main.o \
./lcd_alfa.o 

C_DEPS += \
./BH1750.d \
./TWI_master.d \
./avr2final_funciones.d \
./avr2final_main.d \
./lcd_alfa.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


