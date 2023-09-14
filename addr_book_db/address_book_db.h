#include <stdio.h>
#include <stdlib.h>     // atoi, malloc, free, exit ...
#include <stdint.h>     // uint8_t
#include <stddef.h>     // size_t
#include <string.h>     // strlen
#include <termios.h>    // pwd input (echo enable)
#include <mysql.h>      // mysql c api
#include "AES.h"
#include "pkcs7_padding.h"
#include "Base64.h"


#ifndef MAX
#define MAX 1000
#endif

#ifndef PERSON_DATA_LEN 
#define PERSON_DATA_LEN 70      /* 사람 1명의 데이터 크기 고정 */
#endif

#ifndef DATA_LEN_LIMIT
#define DATA_LEN_LIMIT 20
#endif

#ifndef PWD_LEN_LIMIT
#define PWD_LEN_LIMIT 20
#endif

#define CBC 1

#ifndef IV_LEN
#define IV_LEN 16
#endif


typedef struct Person {
    int id;
    uint8_t name[DATA_LEN_LIMIT];
    uint8_t phone[DATA_LEN_LIMIT];
    uint8_t address[DATA_LEN_LIMIT];
} Person;


MYSQL *con;
const char *DB_USER     = "syyoo";
const char *DB_HOST     = "192.168.1.40";
const char *DB_PWD      = "one123";
const char *DB_NAME     = "address_book";
const char *TABLE_NAME  = "book1";
// uint8_t iv[IV_LEN];
// char* key;
const uint8_t iv[IV_LEN] = {0x65, 0x67, 0x6c, 0x6f, 0x62, 0x61, 0x6c, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x6d, 0x0d, 0x0a};
const char* key = "eglobalsystemeglobalsystem";


void connect_to_db();
void create_table(const char* TABLE_NAME);
void print_db();
void insert_into_db();
void update_db();
void delete_from_db();

void finish_with_error(MYSQL *con);
int is_table_empty(MYSQL* con);

void clear_buffer();


void encrypt_private_data(uint8_t plain[], uint8_t result[]);
void decrypt_private_data(uint8_t base64_encoded_str[], uint8_t result[]);
void decrypt_with_auth();
int pwd_auth();
void print_db_decrypted();
