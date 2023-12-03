CC=gcc
CFLAGS=-fPIC -Wall -Wextra -O2
LDFLAGS=-shared
TARGET=libcosine_similarity.so

all: $(TARGET)

$(TARGET): cosine_similarity.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

install:
	cp $(TARGET) /usr/local/lib/
	ldconfig /usr/local/lib

uninstall:
	rm -f /usr/local/lib/$(TARGET)
	ldconfig /usr/local/lib
