CC = gcc
CFLAGS = -W
LDLIBS = libmyAES.so
OBJS = AES.o pkcs7_padding.o address_book.o test.o
SRCS = AES.c pkcs7_padding.c address_book.c test.c
TARGET = libmyAES.so addr_book
LIBDIR = /usr/local/lib/

all : $(TARGET)

addr_book : AES.o pkcs7_padding.o address_book.o test.o libmyAES.so
	$(CC) -o addr_book AES.o pkcs7_padding.o address_book.o test.o

libmyAES.so : AES.c AES.h
	$(CC) -fPIC -c AES.c
	$(CC) -shared -o libmyAES.so AES.o
	$(CC) -shared -Wl,-soname,libmyAES.so.1 -o libmyAES.so.1.0 AES.o
	sudo cp libmyAES.so.1.0 /usr/local/lib
	sudo ln -s -f /usr/local/lib/libmyAES.so.1.0 /usr/local/lib/libmyAES.so.1
	sudo ln -s -f /usr/local/lib/libmyAES.so.1 /usr/local/lib/libmyAES.so
	sudo ldconfig

AES.o : AES.c AES.h
	$(CC) $(CFLAGS) -c -o AES.o AES.c

pkcs7_padding.o : pkcs7_padding.c pkcs7_padding.h
	$(CC) $(CFLAGS) -c -o pkcs7_padding.o pkcs7_padding.c

address_book.o : address_book.c address_book.h
	$(CC) $(CFLAGS) -c -o address_book.o address_book.c

test.o : test.c test.h
	$(CC) $(CFLAGS) -c -o test.o test.c

clean :
	rm -f *.o
	rm -f $(TARGET)

.PHONY : clean


# $(TARGET) : AES.o test.o pkcs7_padding.o address_book.o
# 	$(CC) $(CFLAGS) -o $(TARGET) AES.o test.o pkcs7_padding.o address_book.o

# AES.o : AES.c AES.h
# 	$(CC) $(CFLAGS) -c -o AES.o AES.c

# pkcs7_padding.o : pkcs7_padding.h pkcs7_padding.c
# 	$(CC) $(CFLAGS) -c -o pkcs7_padding.o pkcs7_padding.c

# address_book.o : address_book.c address_book.h
# 	$(CC) $(CFLAGS) -c -o address_book.o address_book.c

# test.o : test.c test.h
# 	$(CC) $(CFLAGS) -c -o test.o test.c


# test5 : AES.o pkcs7_padding.o address_book.o test.o
# 	gcc -o test5 AES.o pkcs7_padding.o address_book.o test.o

# AES.o : AES.c AES.h
# 	gcc -c -o AES.o AES.c

# pkcs7_padding.o : pkcs7_padding.c pkcs7_padding.h
# 	gcc -c -o pkcs7_padding.o pkcs7_padding.c

# address_book.o : address_book.c address_book.h
# 	gcc -c -o address_book.o address_book.c

# test.o : test.c test.h
# 	gcc -c -o test.o test.c