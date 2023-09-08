CC = gcc
CFLAGS = -W
TARGET = addr_book
LIBSRC = AES.o pkcs7_padding.o test.o
LIBDIR = /home/syyoo/new712

all : $(TARGET)

$(TARGET) : address_book.o libmyAES.so
	$(CC) $(CFLAGS) -o $(TARGET) address_book.o -L. -lmyAES

libmyAES.so : $(LIBSRC)
	$(CC) $(CFLAGS) -shared -o libmyAES.so $(LIBSRC)

address_book.o : address_book.c address_book.h
	$(CC) $(CFLAGS) -c address_book.c

AES.o : AES.c AES.h
	$(CC) $(CFLAGS) -fPIC -c AES.c

pkcs7_padding.o : pkcs7_padding.c pkcs7_padding.h
	$(CC) $(CFLAGS) -fPIC -c pkcs7_padding.c

test.o : test.c test.h
	$(CC) $(CFLAGS) -fPIC -c test.c

clean :
	rm -f *.o
	rm -f *.so 
	rm -f $(TARGET)

.PHONY : clean

