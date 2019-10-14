################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/comm/CNetProc.cpp \
../src/comm/CUartProc.cpp \
../src/comm/PortFactory.cpp 

OBJS += \
./src/comm/CNetProc.o \
./src/comm/CUartProc.o \
./src/comm/PortFactory.o 

CPP_DEPS += \
./src/comm/CNetProc.d \
./src/comm/CUartProc.d \
./src/comm/PortFactory.d 


# Each subdirectory must supply rules for building sources it contributes
src/comm/%.o: ../src/comm/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-9.0/bin/nvcc -Dlinux -I/usr/include/opencv -I/usr/include/GL -I../include -I../include/osa -I../src -I../src/capture -I../src/comm -G -g -O0 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_50,code=sm_50 -m64 -odir "src/comm" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-9.0/bin/nvcc -Dlinux -I/usr/include/opencv -I/usr/include/GL -I../include -I../include/osa -I../src -I../src/capture -I../src/comm -G -g -O0 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


