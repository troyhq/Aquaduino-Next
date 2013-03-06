################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Actuators/DigitalOutput.cpp \
../Actuators/PWMOutput.cpp 

OBJS += \
./Actuators/DigitalOutput.o \
./Actuators/PWMOutput.o 

CPP_DEPS += \
./Actuators/DigitalOutput.d \
./Actuators/PWMOutput.d 


# Each subdirectory must supply rules for building sources it contributes
Actuators/%.o: ../Actuators/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	avr-g++ -DF_CPU=16000000L -DDEBUG -D__AVR_LIBC_DEPRECATED_ENABLE__=1 -DUSB_VID=NULL -DUSB_PID=NULL -DARDUINO=103 -I"D:\workspaces\privat\Aquaduino" -I"D:\workspaces\privat\Arduino" -I"D:\workspaces\privat\OneWire" -I"D:\workspaces\privat\Ethernet" -I"D:\workspaces\privat\Ethernet\utility" -I"D:\workspaces\privat\SD" -I"D:\workspaces\privat\SD\utility" -I"D:\workspaces\privat\SPI" -I"D:\workspaces\privat\Time" -I"D:\workspaces\privat\Webduino" -I"D:\workspaces\privat\TemplateParser" -Os -ffunction-sections -fdata-sections -fno-exceptions -g3 -Wall -c -fmessage-length=0 -mmcu=atmega2560 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


