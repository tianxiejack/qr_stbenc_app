################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/main_cap.cpp \
../src/main_core.cpp \
../src/menu.cpp 

OBJS += \
./src/main.o \
./src/main_cap.o \
./src/main_core.o \
./src/menu.o 

CPP_DEPS += \
./src/main.d \
./src/main_cap.d \
./src/main_core.d \
./src/menu.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-9.0/bin/nvcc -Dlinux -I/usr/include/opencv -I/usr/include/GL -I../include -I../include/osa -I../src -I../src/capture -I../src/comm -O3 -std=c++11 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_50,code=sm_50 -m64 -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-9.0/bin/nvcc -Dlinux -I/usr/include/opencv -I/usr/include/GL -I../include -I../include/osa -I../src -I../src/capture -I../src/comm -O3 -std=c++11 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


