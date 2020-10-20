CC = gcc
CFLAGS = -g -Wall

all:
	$(CC) $(CFLAGS) firstFit.c -o firstFit 
	$(CC) $(CFLAGS) bestFit.c -o bestFit
	$(CC) $(CFLAGS) worstFit.c -o worstFit

clean:
	rm output.txt
	rm firstFit
	rm bestFit
	rm worstFit

