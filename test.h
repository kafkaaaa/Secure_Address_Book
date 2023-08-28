#pragma once

#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
#define CBC 1   // Cipher Block Chaining (블럭을 체인으로 묶는 방식, 초기벡터IV 사용)
// #define CTR 1   // Counter (1씩 증가하는 counter와 nonce 사용)
// #define ECB 1   // Electronic Code Block (모든 블럭이 같은 암호화 키 사용)

#include "address_book.h"
#include "AES.h"
#include "time.h"
#include "pkcs7_padding.h"


static void phex(uint8_t* str);
static int test_encrypt_cbc(void);
static int test_decrypt_cbc(void);
static int test_encrypt_ctr(void);
static int test_decrypt_ctr(void);
static int test_encrypt_ecb(void);
static int test_decrypt_ecb(void);
static void test_encrypt_ecb_verbose(void);
extern char* Encrypt_test(char*);


#define IV_LEN 16

#endif

