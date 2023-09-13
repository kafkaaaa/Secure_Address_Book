CC = c99
CFLAGS = -I$(INC)
INC = /usr/include/mysql/
LDFLAGS = -lmysqlclient

OBJ = address_book_db.o pkcs7_padding.o AES.o
SRC = address_book_db.c
HEADER = address_book_db.h pkcs7_padding.h AES.h

TARGET = addr_book

all : $(TARGET)

$(TARGET) : $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

address_book_db.o : $(SRC) address_book_db.h
	$(CC) $(CFLAGS) -c $(SRC) $(LDFLAGS)

# $(OBJ) : $(SRC) $(HEADER)
# 	$(CC) $(CFLAGS) -c $(SRC) $(LDFLAGS)

pkcs7_padding.o : pkcs7_padding.c pkcs7_padding.h
	gcc -W -c pkcs7_padding.c

AES.o : AES.c AES.h
	gcc -W -c AES.c

clean :
	rm -f *.o
	rm -f $(TARGET)


.PHONY : clean

