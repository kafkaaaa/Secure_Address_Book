# CC = gcc
# CFLAGS = -W -WALL
# TARGET = test5
# OBJS = AES.o test.o pkcs7_padding.o address_book.o

# $(TARGET) : AES.o test.o pkcs7_padding.o address_book.o
# 	$(CC) $(CFLAGS) -o $(TARGET) AES.o test.o pkcs7_padding.o address_book.o

# AES.o : AES.c AES.h
# 	$(CC) $(CFLAGS) -c -o AES.o AES.c

# test.o : test.c test.h
# 	$(CC) $(CFLAGS) -c -o test.o test.c

# pkcs7_padding.o : pkcs7_padding.h pkcs7_padding.c
# 	$(CC) $(CFLAGS) -c -o pkcs7_padding.o pkcs7_padding.c

# address_book.o : address_book.c address_book.h
# 	$(CC) $(CFLAGS) -c -o address_book.o address_book.c


test5 : AES.o test.o pkcs7_padding.o address_book.o
	gcc -o test5 AES.o test.o pkcs7_padding.o address_book.o

AES.o : AES.c
	gcc -c -o AES.o AES.c

test.o : test.c
	gcc -c -o test.o test.c

pkcs7_padding.o : pkcs7_padding.c
	gcc -c -o pkcs7_padding.o pkcs7_padding.c

address_book.o : address_book.c
	gcc -c -o address_book.o address_book.c