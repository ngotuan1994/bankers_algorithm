CC=g++
TARGET=bankers
all:
	$(CC) $(TARGET).cpp -o $(TARGET) -lpthread
	./$(TARGET) bankers_tinier.txt bankers_tiny.txt bankers_small.txt bankers_medium.txt bankers_large.txt > output.txt
clean:
	rm $(TARGET)
	rm output.txt
