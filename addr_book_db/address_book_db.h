#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>


// 매크로 정의
#ifndef MAX
#define MAX 1000
#endif

#ifndef PERSON_DATA_LEN 
#define PERSON_DATA_LEN 70      /* 사람 1명의 데이터 크기 고정 */
#endif

#ifndef DATA_LEN_LIMIT
#define DATA_LEN_LIMIT 20
#endif

#ifndef FILE_NAME_LIMIT
#define FILE_NAME_LIMIT 50
#endif


// #define SAVE_MODE_TXT 1
#define SAVE_MODE_BIN 1
#if defined(SAVE_MODE_TXT)
    #define FOUT_NAME "output.txt"
#elif defined(SAVE_MODE_BIN)
    #define FOUT_NAME "output.bin"
#endif


// 구조체 정의
typedef struct Person {
    int id;
    uint8_t name[DATA_LEN_LIMIT];
    uint8_t phone[DATA_LEN_LIMIT];
    uint8_t address[DATA_LEN_LIMIT];
    struct Person *next;
} Person;


// 전역 변수 선언
Person *head, *tail, *tmp;
FILE *fp;
int person_cnt; // 주소록에 등록된 사람 수

MYSQL *con;
const char *db_host = "192.168.1.40";
const char *db_user = "syyoo";
const char *db_pwd  = "one123";
const char *db_name = "address_book";
const char *table_name   = "book1";



// 함수 선언
void connect_to_db();
void finish_with_error(MYSQL *con);
int is_table_empty(MYSQL* con);

void create_table(const char* table_name);
void print_db();
void insert_into_db();
void update_db();
void delete_from_db();


void load_file();
void save_file();


void clear_buffer();

