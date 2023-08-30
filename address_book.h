#pragma once

#ifndef _ADDRESS_BOOK_H_
#define _ADDRESS_BOOK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "address_book.h"
#include "AES.h"
#include "pkcs7_padding.h"

// 매크로 정의
#ifndef MAX
#define MAX 1000
#endif

#ifndef DATA_LEN_LIMIT
#define DATA_LEN_LIMIT 20
#endif

#ifndef FILE_NAME_LIMIT
#define FILE_NAME_LIMIT 50
#endif

// #define SAVE_MODE_TXT 1
#define SAVE_MODE_DAT 1

#if defined(SAVE_MODE_TXT)
    #define FOUT_NAME "output.txt"
#elif defined(SAVE_MODE_DAT)
    #define FOUT_NAME "output.dat"
#endif

// 구조체 정의
typedef struct Person {
    int id; // 중복 레코드 문제로 ID 추가
    uint8_t name[DATA_LEN_LIMIT];
    uint8_t phone[DATA_LEN_LIMIT];
    uint8_t address[DATA_LEN_LIMIT];
    struct Person *next;
} Person;

// 전역 변수 선언
Person *head, *tail, *tmp;
FILE *fp;
int person_cnt; // 주소록에 등록된 사람 수

// 함수 선언
void init_address_list();
void print_list();
void insert_list();
void insert_into_list();
void update_list();
void delete_list();
void load_file();
void save_file();

#endif

