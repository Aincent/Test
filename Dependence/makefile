
CPP_SRCS += \
./MessageStruct/ServerReturnInt.pb.cc \
./MessageStruct/CharLogin/CreateCharactor.pb.cc \
./MessageStruct/CharLogin/GetCharID.pb.cc \

OBJS += \
./DependenceObj/ServerReturnInt.pb.o \
./DependenceObj/CreateCharactor.pb.o \
./DependenceObj/GetCharID.pb.o \


# Each subdirectory must supply rules for building sources it contributes
all:
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/protobuf/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP $(OBJS) -o $CPP_SRCS
	@echo 'Finished building: $<'
	@echo ' '


