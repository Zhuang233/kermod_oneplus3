CC := /home/zbw/Desktop/aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
CFLAGS := -g
TARGET := main
SRCS := $(wildcard *.c)
OBJS := $(patsubst %c,%o,$(SRCS))
all:$(TARGET)
%.o:%.c
	$(CC) $(CFLAGS) -static -c $<
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -static -o $@ $^
clean:
	rm -rf $(TARGET) *.o