CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 
SRCS = main.c pcap_reader.c logger.c print_packets.c type_link.c 
TARGET = dieanalyzd

all: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean: 
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin
