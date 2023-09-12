CC = c99
CFLAGS = -I$(INC)
INC = /usr/include/mysql/
LDFLAGS = -lmysqlclient

OBJ = address_book_db.o
SRC = address_book_db.c
HEADER = address_book_db.h

TARGET = addr_book

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(OBJ) : $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -c $(SRC) $(LDFLAGS)

clean :
	rm -f *.o
	rm -f $(TARGET)

.PHONY : clean

