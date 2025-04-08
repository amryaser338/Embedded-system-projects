################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../CLCD_program.c \
../DCmotor.c \
../DIO_program.c \
../GIE_program.c \
../PIR.c \
../PWM.c \
../TIMER_program.c \
../USART_program.c \
../external_eeprom.c \
../program.c \
../twi.c 

OBJS += \
./Buzzer.o \
./CLCD_program.o \
./DCmotor.o \
./DIO_program.o \
./GIE_program.o \
./PIR.o \
./PWM.o \
./TIMER_program.o \
./USART_program.o \
./external_eeprom.o \
./program.o \
./twi.o 

C_DEPS += \
./Buzzer.d \
./CLCD_program.d \
./DCmotor.d \
./DIO_program.d \
./GIE_program.d \
./PIR.d \
./PWM.d \
./TIMER_program.d \
./USART_program.d \
./external_eeprom.d \
./program.d \
./twi.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


