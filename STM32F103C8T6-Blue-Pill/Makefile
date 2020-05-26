
CC = gcc

C_FLAGS = -I remcu_include/
L_FLAGS = -lremcu -L./ -Wl,-rpath=$(PWD)

all:
	$(CC) ADC_line.c $(C_FLAGS) $(L_FLAGS) -o ADC_line
	$(CC) CAN_send.c $(C_FLAGS) $(L_FLAGS) -o CAN_send
	$(CC) CAN_receiver.c $(C_FLAGS) $(L_FLAGS) -o CAN_receiver

clean:
	rm -v CAN_send CAN_receiver ADC_line
