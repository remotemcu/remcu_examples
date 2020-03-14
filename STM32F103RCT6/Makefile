
CC = gcc

C_FLAGS = -I remcu_include/
L_FLAGS = -lremcu -L./ -Wl,-rpath=$(PWD)

all:
	$(CC) DAC_DMA_Escalator.c $(C_FLAGS) $(L_FLAGS) -o DAC_DMA_Escalator
	$(CC) TriangleWave.c $(C_FLAGS) $(L_FLAGS) -o TriangleWave
	$(CC) ADC_line.c $(C_FLAGS) $(L_FLAGS) -o ADC_line

clean:
	rm -v DAC_DMA_Escalator TriangleWave ADC_line
