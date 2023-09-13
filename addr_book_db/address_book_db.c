#include "address_book_db.h"

int main() {

    int menu;
    connect_to_db();
    create_table(TABLE_NAME);

    while (1) {
        printf("\n======= MENU =======\n");
        printf("  #1. 주소록 보기\n");
        printf("  #2. 주소록 추가\n");
        printf("  #3. 주소록 수정\n");
        printf("  #4. 주소록 삭제\n");
        printf("  #5. 복호화\n");
        printf("  #6. 프로그램 종료\n");
        printf("====================\n");
        printf("작업을 선택해주세요->  ");
        scanf(" %d", &menu);
        if (menu > 6 || menu < 1) {
            printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        }
        clear_buffer();

        if      (menu == 1)     print_db();
        else if (menu == 2)     insert_into_db();
        else if (menu == 3)     update_db();
        else if (menu == 4)     delete_from_db();
        else if (menu == 5)     decrypt_with_auth();      // TODO: 암호화된 개인정보 -> 복호화 하기
        else if (menu == 6) {   mysql_close(con); break;   }
        else {
            printf("1~6만 입력해주세요.\n");
        }
    }
    return 0;
}


void clear_buffer() {
    while (getchar() != '\n');
}


/* ------------------------ DB 관련 함수 ------------------------*/

/* MySQL error 처리 */
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}


/* DB 서버 연결 및 초기 작업 */
void connect_to_db()
{
    con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, DB_HOST, DB_USER, DB_PWD, DB_NAME,
                            0, NULL, CLIENT_MULTI_STATEMENTS) != NULL)
    {    
        printf("MySQL [Client] Version = %s\n", mysql_get_client_info());
        printf("MySQL [Server] Version = %s\n", mysql_get_server_info(con));
        // printf("MySQL [Server] Version = %lu\n", mysql_get_server_version(con));
        printf("[HOST] = %s\n", mysql_get_host_info(con));
    }
    else finish_with_error(con);

    // character-set (한글 포함)
    mysql_set_character_set(con, "UTF-8");
    printf("\n[DB연결 및 초기 작업 완료]\n");
}


/* 테이블 생성 */
void create_table(const char* TABLE_NAME)
{
    // // 이미 같은이름의 테이블이 있으면 삭제
    // char drop_table_query[50];
    // sprintf(drop_table_query, "DROP TABLE IF EXISTS %s", TABLE_NAME);
    // if (mysql_query(con, drop_table_query)) {
    //     printf("\n[이미 존재하는 테이블 삭제 오류]\n");
    //     finish_with_error(con);
    // }
    // else printf("\n[기존 테이블 %s 삭제]\n", TABLE_NAME);


    // 기존 테이블이 존재하지 않으면 생성
    char create_table_query[400];
    sprintf(create_table_query, "CREATE TABLE IF NOT EXISTS %s(\
            id      INT     PRIMARY KEY     AUTO_INCREMENT,\
            name    VARCHAR(%d),\
            phone   VARCHAR(%d),\
            address VARCHAR(%d))",
            TABLE_NAME, DATA_LEN_LIMIT, DATA_LEN_LIMIT, DATA_LEN_LIMIT);
    if (mysql_query(con, create_table_query)) {
        printf("\n[테이블 생성 오류]\n");
        finish_with_error(con);
    }
    else printf("\n[테이블 %s 생성]\n", TABLE_NAME);
}


/* 전체 주소록 출력하기 */
void print_db()
{
    if ( !(is_table_empty(con)) )       // table이 비어있는지 먼저 확인
    {
        printf("\n전체 주소록을 출력합니다...\n");
        printf("----------------------------------------------------------------------------\n");

        char count_row_query[50];
        sprintf(count_row_query, "SELECT COUNT(*) FROM %s", TABLE_NAME);
        mysql_query(con, count_row_query);
        MYSQL_RES* res = mysql_store_result(con);
        MYSQL_ROW res_row = mysql_fetch_row(res);
        printf("<현재 등록된 사람: %d명>\n", atoi(res_row[0]));
        mysql_free_result(res);
        printf("----------------------------------------------------------------------------\n");


        printf("ID\t\t이름\t\t전화번호\t\t주소\n");
        printf("----------------------------------------------------------------------------\n");
        char select_all_query[50];
        sprintf(select_all_query, "SELECT * FROM %s", TABLE_NAME);
        mysql_query(con, select_all_query);
        MYSQL_RES* result = mysql_store_result(con);
        MYSQL_ROW res_rows;
        while (res_rows = mysql_fetch_row(result)) {
            printf("%d\t\t%s\t\t%s\t\t%s\n", atoi(res_rows[0]), res_rows[1], res_rows[2], res_rows[3]);
            // ex) ID     이름        전화번호        주소
            //     123    홍길동      010-1234-5678   서울1
            //     456    홍길둥      010-1111-2222   서울2
        }
        mysql_free_result(result);
        printf("-----------------------------------END-------------------------------------\n");
    }
}


/* (사용자 입력으로) 주소록 테이블에 데이터 추가하기 */
void insert_into_db()
{
    Person* new_person = (Person*)malloc(sizeof(Person));
    printf("주소록에 추가할 데이터를 입력하세요.\n");
    while (1) {
        printf("이름: ");
        scanf(" %[^\n]s", new_person->name);
        clear_buffer();
        if (strlen(new_person->name) > DATA_LEN_LIMIT) {
            printf("입력 길이 초과!! (20자 제한) 다시 입력해주세요.\n");
        }
        else break;
    }

    while (1) {
        printf("전화번호: ");
        scanf(" %[^\n]s", new_person->phone);
        clear_buffer();
        if (strlen(new_person->phone) > DATA_LEN_LIMIT) {
            printf("입력 길이 초과!! (20자 제한) 다시 입력해주세요.\n");
        }
        else break;
    }

    while (1) {
        printf("주소: ");
        scanf(" %[^\n]s", new_person->address);
        clear_buffer();
        if (strlen(new_person->address) > DATA_LEN_LIMIT) {
            printf("입력 길이 초과!! (20자 제한) 다시 입력해주세요.\n");
        }
        else break;
    }
        // // TEST CODE
        // printf("\n[TEST]\n");
        // printf("name: %s\nphone: %s\naddress: %s\n", new_person->name, new_person->phone, new_person->address);



    // TODO: DB에 넣기 전에 암호화 작업 수행

    // <전체적인 순서> ???
    // -암호화 (insert) : plain text -> encrypt -> binary -> base64 encoding -> DB에 insert
    // -복호화 (select) : base64 decoding -> binary -> decrypt (선택) -> plain text

    uint8_t plain_name[DATA_LEN_LIMIT] = {0, };
    uint8_t plain_phone[DATA_LEN_LIMIT] = {0, };
    uint8_t plain_address[DATA_LEN_LIMIT] = {0, };
    strcat(plain_name, new_person->name);
    strcat(plain_phone, new_person->phone);
    strcat(plain_address, new_person->address);
    
    uint8_t *encrypted_name;
    uint8_t *encrypted_phone;
    uint8_t *encrypted_address;
    encrypted_name      = (uint8_t*) calloc(AES_BLOCKLEN * 8 + 1, sizeof(uint8_t));
    encrypted_phone     = (uint8_t*) calloc(AES_BLOCKLEN * 8 + 1, sizeof(uint8_t));
    encrypted_address   = (uint8_t*) calloc(AES_BLOCKLEN * 8 + 1, sizeof(uint8_t));
    // encrypt_private_data(new_person->name, encrypted_name);
    // encrypt_private_data(new_person->phone, encrypted_phone);
    // encrypt_private_data(new_person->address, encrypted_address);
    encrypt_private_data(plain_name, encrypted_name);
    encrypt_private_data(plain_phone, encrypted_phone);
    encrypt_private_data(plain_address, encrypted_address);

        // TEST CODE
        printf("\n\n[TEST] 암호화된 데이터\n");
        printf("%s\n%s\n%s\n", encrypted_name, encrypted_phone, encrypted_address);



    // DB 테이블에 값 넣기
    char insert_query[1000];
    // sprintf(insert_query, "INSERT INTO %s VALUES(DEFAULT, '%s', '%s', '%s')",
    //     TABLE_NAME, new_person->name, new_person->phone, new_person->address);   // ID값은 AUTO_INCREMENT
    sprintf(insert_query, "INSERT INTO %s VALUES(DEFAULT, '%s', '%s', '%s')",
        TABLE_NAME, encrypted_name, encrypted_phone, encrypted_address);   // ID값은 AUTO_INCREMENT

    if (mysql_query(con, insert_query)) {
        finish_with_error(con);
    }


    free(new_person);
    free(encrypted_name);
    free(encrypted_phone);
    free(encrypted_address);
}




/* 주소록 테이블 수정하기 */
void update_db()
{
    if (is_table_empty(con)) {      // 테이블이 비어있는지 먼저 확인
        printf("[주소록이 비어 있습니다] 수정할 데이터가 없습니다.\n");
        return;
    }

    int id, update_option;
    printf("----주소록을 수정합니다----\n");

    while (1) {
        printf("\n수정할 사람의 ID를 입력해주세요: ");
        scanf("%d", &id);
        clear_buffer();
        if (id < 1 || id > MAX) {
            printf("[잘못 입력하셨습니다!!] 다시 입력해주세요.\n");
        }
        else break;
    }

    while (1) {
        printf("\n[ID=%d]인 사람의 데이터를 수정합니다.\n", id);
        printf("1.이름  2.전화번호  3.주소\n");
        printf("\n수정할 항목을 선택해주세요: ");
        scanf("%d", &update_option);
        clear_buffer();
        if (update_option < 1 || update_option > 3) {
            printf("[잘못 입력하셨습니다!!] 다시 입력해주세요.\n");
        }
        else break;
    }

    switch (update_option) {
        case 1:
            while (1) {
                uint8_t new_name[DATA_LEN_LIMIT] = {0, };
                printf("새로운 이름을 입력해주세요: ");
                scanf("%s", new_name);
                clear_buffer();
                if (strlen(new_name) > DATA_LEN_LIMIT) {
                    printf("[입력 길이 초과!!] 다시 입력해주세요.\n");
                }
                else {
                    char update_name_query[200];
                    sprintf(update_name_query, "UPDATE %s SET name='%s' WHERE id=%d",
                                                    TABLE_NAME,     new_name,    id);
                    if (mysql_query(con, update_name_query)) {
                        printf("[수정 실패!!]\n");
                        finish_with_error(con);
                        break;
                    }
                    else {
                        printf("[수정 완료!]\n");
                        break;
                    }
                }
            }
            break;

        case 2:
            while (1) {
                uint8_t new_phone[DATA_LEN_LIMIT] = {0, };
                printf("새로운 전화번호를 입력해주세요: ");
                scanf("%s", new_phone);
                clear_buffer();
                if (strlen(new_phone) > DATA_LEN_LIMIT) {
                    printf("[입력 길이 초과!!] 다시 입력해주세요.\n");
                }
                else {
                    char update_phone_query[200];
                    sprintf(update_phone_query, "UPDATE %s SET name='%s' WHERE id=%d",
                                                    TABLE_NAME,     new_phone,    id);
                    if (mysql_query(con, update_phone_query)) {
                        printf("[수정 실패!!]\n");
                        finish_with_error(con);
                        break;
                    }
                    else {
                        printf("[수정 완료!]\n");
                        break;
                    }
                }
            }
            break;


        case 3:
            while (1) {
                uint8_t new_addr[DATA_LEN_LIMIT] = {0, };
                printf("새로운 주소를 입력해주세요: ");
                scanf("%s", new_addr);
                clear_buffer();
                if (strlen(new_addr) > DATA_LEN_LIMIT) {
                    printf("[입력 길이 초과!!] 다시 입력해주세요.\n");
                }
                else {
                    char update_addr_query[200];
                    sprintf(update_addr_query, "UPDATE %s SET name='%s' WHERE id=%d",
                                                    TABLE_NAME,     new_addr,    id);
                    if (mysql_query(con, update_addr_query)) {
                        printf("[수정 실패!!]\n");
                        finish_with_error(con);
                        break;
                    }
                    else {
                        printf("[수정 완료!]\n");
                        break;
                    }
                }
            }
            break;
    }
}


/* ID값으로 주소록에서 사람 삭제하기 */
void delete_from_db()
{
    if (is_table_empty(con)) {
        printf("[주소록이 비어 있습니다!] 삭제할 데이터가 없습니다.\n");
        return;
    }

    int target_id = 0;   // 삭제 대상 ID
    while (1) {
        printf("주소록에서 삭제할 ID를 입력해주세요: ");
        scanf("%d", &target_id);
        clear_buffer();
        if (target_id < 1 || target_id > MAX) {
            printf("[잘못된 입력입니다!!] 다시 입력해주세요.\n");
        }
        else break;
    }

    char delete_query[50];
    sprintf(delete_query, "DELETE FROM %s WHERE id=%d", TABLE_NAME, target_id);
    if (mysql_query(con, delete_query)) {
        printf("[삭제 실패!!] 해당하는 사람이 없습니다.\n");
        finish_with_error(con);
        return;
    }
}


/* DB 테이블이 비어있는지 확인하는 함수 */
int is_table_empty(MYSQL* con)
{
    char is_empty_query[50];
    sprintf(is_empty_query, "SELECT EXISTS (SELECT 1 FROM %s);", TABLE_NAME);  
    mysql_query(con, is_empty_query);
    MYSQL_RES* res = mysql_store_result(con);

    if (res->data == 0) {
        printf("\n주소록이 비어있습니다.\n");
        mysql_free_result(res);
        return 1;
    }
    else return 0;
}
/* ---------------------------------------------------------------*/


/* AES 암/복호화 함수 */
void encrypt_private_data(uint8_t plain[], uint8_t result[])
{
    size_t i;
    printf("\n------------ Encrypt Data... ------------\n");

    // size_t plain_len = PERSON_DATA_LEN;         // 고정크기로 암호화
    size_t plain_len = DATA_LEN_LIMIT;         // 고정크기로 암호화
    size_t key_len = strlen(key);              // Length of Key
        // TEST CODE
        printf("[Plain Text] = ");
        for (i=0; i<plain_len; i++)
            printf("%c", plain[i]);
        printf("\n");

    // 평문 길이를 16의 배수로 맞춤
    size_t hex_plain_len = plain_len;
    if (plain_len % 16) {
        hex_plain_len += 16 - (plain_len % 16);
        printf("The original Length of (Plain Text) = %zd\n", plain_len);
        printf("The Length of (padded Plain Text) = %zd\n", hex_plain_len);
    }

    // 키의 길이를 16의 배수로 맞춤
    size_t hex_key_len = key_len;
    if (key_len % 16) {
        hex_key_len += 16 - (key_len % 16);
        printf("The original Length of (KEY) = %zd\n", key_len);
        printf("The Length of (padded KEY) = %zd\n", hex_key_len);
    }

    // 패딩된 데이터를 저장할 배열
    uint8_t padded_plain[hex_plain_len];
    uint8_t padded_key[hex_key_len];
    memset(padded_plain, 0, hex_plain_len);
    memset(padded_key, 0, hex_key_len);

    for (i=0; i<plain_len; i++)
        padded_plain[i] = plain[i];
    
    for (i=0; i<key_len; i++)
        padded_key[i] = key[i];
    
    /* padding with PKCS7 */
    // pkcs7_padding_pad_buffer -> returns the number of paddings it added
    pkcs7_padding_pad_buffer(padded_plain, plain_len, sizeof(padded_plain), AES_BLOCKLEN);
    pkcs7_padding_pad_buffer(padded_key, key_len, sizeof(padded_key), AES_BLOCKLEN);

    printf("\nThe padded Plain Text (HEX) is...");
    for (i=0; i<hex_plain_len; i++) {
        if (i % 16 == 0) puts("");
        printf("%02x ", padded_plain[i]);
    }
    puts("");

    // printf("\nThe padded Key (HEX) is...");
    // for (i=0; i<hex_key_len; i++) {
    //     if (i % 16 == 0) puts("");
    //     printf("%02x ", padded_key[i]);
    // }
    // puts("");
    

    // ** Encryption Test **
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, padded_key, iv);
    AES_CBC_encrypt_buffer(&ctx, padded_plain, hex_plain_len);
    printf("\n[Encrypted] String is... ");
    for (i=0; i<hex_plain_len; i++) {
        if (i % 16 == 0) puts("");
        result[i] = padded_plain[i];
        printf("%02x ", padded_plain[i]);
    }
    puts("");
}





void decrypt_private_data()
{

}



/* 비밀번호로 인증 성공 시 복호화 */
void decrypt_with_auth() {
    
}


