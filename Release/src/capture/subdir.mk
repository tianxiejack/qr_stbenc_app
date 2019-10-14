################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/capture/MultiChVideo.cpp \
../src/capture/v4l2camera.cpp 

OBJS += \
./src/capture/MultiChVideo.o \
./src/capture/v4l2camera.o 

CPP_DEPS += \
./src/capture/MultiChVideo.d \
./src/capture/v4l2camera.d 


# Each subdirectory must supply rules for building sources it contributes
src/capture/%.o: ../src/capture/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-9.0/bin/nvcc -Dlinux -I/usr/include/opencv -I/usr/include/GL -I../include -I../include/osa -I../src -I../src/capture -I../src/comm -O3 -std=c++11 -ccbin aarch64-linux-gnu-g++ -gencode arch=compute_50,code=sm_50 -m64 -odir "src/capture" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-9.0/bin/nvcc -Dlinux -I/usr/include/opencv -I/usr/include/GL -I../include -I../include/osa -I../src -I../src/capture -I../src/comm -O3 -std=c++11 --compile -m64 -ccbin aarch64-linux-gnu-g++  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


