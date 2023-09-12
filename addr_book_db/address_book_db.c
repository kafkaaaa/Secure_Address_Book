#include "address_book_db.h"

extern size_t Encrypt_test(uint8_t[], uint8_t[]);
extern void Decrypt_test(uint8_t len, uint8_t[], uint8_t[]);


int main() {

    int menu;
    connect_to_db();

    while (1) {
        printf("\n======= MENU =======\n");
        printf("  #1. 주소록 보기\n");
        printf("  #2. 주소록 추가\n");
        printf("  #3. 주소록 수정\n");
        printf("  #4. 주소록 삭제\n");
        printf("  #5. 종료\n");
        printf("====================\n");
        printf("작업을 선택해주세요->  ");
        scanf(" %d", &menu);
        if (menu > 5 || menu < 1) {
            printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        }
        clear_buffer();

        if      (menu == 1)     print_db();
        else if (menu == 2)     insert_into_db();
        else if (menu == 3)     update_db();
        else if (menu == 4)     delete_from_db();
        else if (menu == 5) {   save_file();    break;  }
        else {
            printf("1~5만 입력해주세요.\n");
        }
    }
    return 0;
}




// 파일로부터 주소록 데이터 자동 불러오기
void load_file()
{
    char file_name[FILE_NAME_LIMIT];
    Person* temp = (Person *)malloc(sizeof(Person));
    size_t i;

    while (1) {
        printf("***주소록 파일을 불러옵니다***\n");
        printf("파일 이름을 입력해주세요: ");
        scanf("%s", file_name);
        if (strlen(file_name) > FILE_NAME_LIMIT) {
            printf("파일명 길이 초과. 다시 입력해주세요.\n\n");
        }
        else if (fopen(file_name, "rb") == NULL) {
            printf("파일 열기 실패!! 파일명을 확인해주세요.\n\n");
        }
        else break;
    }

    fp = fopen(file_name, "rb");
    if (fgetc(fp) == EOF) {
        printf("파일이 비어있습니다. 프로그램을 종료합니다.\n");
        exit(1);
    }
    else if (fp != NULL) {
        printf("\nFile Open Success !\n");
        rewind(fp);

        printf("\n[%s] 파일을 복호화 합니다...", file_name);
        printf("\n---------------------------------------------------------------------\n");

        /* binary로 저장된 output 파일 복호화 */
        while (1) {
            uint8_t len = PERSON_DATA_LEN;
            if (len % 16 != 0) {
                len += 16 - (len % 16);
            }
            uint8_t enc_tmp[len];
            uint8_t result[len];
            fread(enc_tmp, sizeof(uint8_t), len, fp);
            if (feof(fp) != 0) break;

            Decrypt_test(len, enc_tmp, result);

            // 복호화한 내용 다시 리스트에 넣기
            sscanf(result, "%d, %[^,], %[^,], %[^\n]", &(temp->id), temp->name, temp->phone, temp->address);
            printf("[%d]번째 데이터 읽어오기 성공", person_cnt+1);
            printf("\n---------------------------------------------------------------------\n");

            // 주소록 연결리스트에 삽입하기
            insert_list(temp);
        }
    }
    else {
        printf("File Open Error !\n\n");
    }
    free(temp);
    fclose(fp);
}


/* 프로그램 종료시 주소록 리스트에 있는 Data들을 파일로 저장하기 */
void save_file()
{
    size_t i;
    FILE* fout_p = fopen(FOUT_NAME, "wb");
    
    if (fout_p == NULL) {
        printf("\n[%s] 파일 생성에 실패했습니다!\n\n", FOUT_NAME);
        return;
    }
    else {
        printf("\n[%s] 파일이 생성되었습니다.\n", FOUT_NAME);
    }

    printf("[AES-256] [MDOE= CBC] [Padding= PKCS7] 암호화를 시작합니다...\n\n");

    /* 1명씩 암호화 해서 저장 */
    Person *p = head->next;
    while (1) {
        p = p->next;
        if (p == NULL) break;

        uint8_t str_tmp[PERSON_DATA_LEN] = {0, };
        uint8_t result[PERSON_DATA_LEN + 16] = {0, };
        sprintf(str_tmp, "%d", p->id);         strcat(str_tmp, ",");
        strcat(str_tmp, p->name);              strcat(str_tmp, ",");
        strcat(str_tmp, p->phone);             strcat(str_tmp, ",");
        strcat(str_tmp, p->address);           strcat(str_tmp, "\n");

        /* Encrypt Process */
        size_t result_len = Encrypt_test(str_tmp, result);      // result_len = (패딩 포함) 암호화된 결과의 길이
        fwrite(result, sizeof(uint8_t), result_len, fout_p);
    }
    printf("\n\n[%s] 주소록 저장 성공.\n", FOUT_NAME); 
    printf("프로그램을 종료합니다.\n");
    fclose(fout_p);
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

    if (mysql_real_connect(con, db_host, db_user, db_pwd, db_name,
                            0, NULL, CLIENT_MULTI_STATEMENTS) != NULL) {    
        printf("[DB connect success!]\n");
        printf("MySQL [Client] Version = %s\n", mysql_get_client_info());
        printf("MySQL [Server] Version = %s\n", mysql_get_server_info(con));
        // printf("MySQL [Server] Version = %lu\n", mysql_get_server_version(con));
        printf("[HOST] = %s\n", mysql_get_host_info(con));
    }
    else finish_with_error(con);
    
    create_table(table_name);
}



/* 테이블 생성 */
void create_table(const char* table_name)
{
    // // 이미 같은이름의 테이블이 있으면 삭제
    // char drop_table_query[50];
    // sprintf(drop_table_query, "DROP TABLE IF EXISTS %s", table_name);
    // if (mysql_query(con, drop_table_query));    

    char create_table_query[200];
    sprintf(create_table_query, "CREATE TABLE %s(\
            id      INT     PRIMARY KEY,\
            name    VARCHAR(%d),\
            phone   VARCHAR(%d),\
            address VARCHAR(%d))",
            table_name, DATA_LEN_LIMIT, DATA_LEN_LIMIT, DATA_LEN_LIMIT);
    if (mysql_query(con, create_table_query)) {
        finish_with_error(con);
    }

    mysql_close(con);
    exit(0);
}



/* 전체 주소록 출력하기 */
void print_db()
{
    if ( !(is_table_empty(con)) )       // table이 비어있는지 먼저 확인
    {
        printf("\n전체 주소록을 출력합니다...\n");
        printf("----------------------------------------------------------------------------\n");

        char count_row_query[50];
        sprintf(count_row_query, "SELECT COUNT(*) FROM %s);", table_name);
        mysql_query(con, count_row_query);
        MYSQL_RES* res = mysql_store_result(con);
        printf("<현재 등록된 사람: %d명>\n", res->data);
        mysql_free_result(res);
        printf("----------------------------------------------------------------------------\n");


        printf("ID\t\t이름\t\t전화번호\t\t주소\n");
        printf("----------------------------------------------------------------------------\n");
        MYSQL_RES* result;
        MYSQL_ROW res_rows;
        char select_all_query[50];
        sprintf(select_all_query, "SELECT * FROM %s);", table_name);
        mysql_query(con, select_all_query);
        MYSQL_RES* result = mysql_store_result(con);
        while (res_rows = mysql_fetch_row(result)) {
            printf("%d\t\t%s\t\t%s\t\t%s\n", res_rows[0], res_rows[1], res_rows[2], res_rows[3]);
            // ex) ID       이름        전화번호        주소
            //     123      홍길동      010-1234-5678   서울1
            // TODO: 출력 TEST CODE
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
            printf("입력 길이 초과!! 다시 입력해주세요.\n");
        }
        else break;
    }

    while (1) {
        printf("전화번호: ");
        scanf(" %[^\n]s", new_person->phone);
        clear_buffer();
        if (strlen(new_person->phone) > DATA_LEN_LIMIT) {
            printf("입력 길이 초과!! 다시 입력해주세요.\n");
        }
        else break;
    }

    while (1) {
        printf("주소: ");
        scanf(" %[^\n]s", new_person->address);
        clear_buffer();
        if (strlen(new_person->address) > DATA_LEN_LIMIT) {
            printf("입력 길이 초과!! 다시 입력해주세요.\n");
        }
        else break;
    }

    // DB 테이블에 값 넣기
    char* insert_query;
    sprintf(insert_query, "INSERT INTO %s VALUES('%s', '%s', '%s')",
        table_name, new_person->name, new_person->phone, new_person->address);   // ID값은 AUTO_INCREMENT

    if (mysql_query(con, insert_query)) {
        finish_with_error(con);
    }

    free(new_person);
}



/* DB 테이블이 비어있는지 확인하는 함수 */
int is_table_empty(MYSQL* con)
{
    char is_empty_query[50];
    sprintf(is_empty_query, "SELECT EXISTS (SELECT 1 FROM %s);", table_name);  
    mysql_query(con, is_empty_query);
    MYSQL_RES* res = mysql_store_result(con);

    if (res->data == 0) {
        printf("\n주소록이 비어있습니다.\n");
        mysql_free_result(res);
        return 1;
    }
    else return 0;
}




/* 주소록 DB 테이블 수정하기 */
void update_db()
{
    if (is_table_empty(con)) {      // 테이블이 비어있는지 먼저 확인
        printf("[주소록이 비어 있습니다] 수정할 데이터가 없습니다.\n");
        return;
    }

    int id, update_option;

    printf("----주소록을 수정합니다----\n");

    while (1) {
        printf("수정할 사람의 ID를 입력해주세요: ");
        scanf("%d", &id);
        clear_buffer();
        if (id < 1 || id > MAX) {
            printf("[잘못 입력하셨습니다!!] 다시 입력해주세요.\n");
        }
        else break;
    }

    while (1) {
        printf("[ID=%d]인 사람의 데이터를 수정합니다.\n", id);
        printf("1.이름  2.전화번호  3.주소\n");
        printf("수정할 항목을 선택해주세요: ");
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
                    char* update_name_query;
                    sprintf(update_name_query, "UPDATE %s SET name='%s' WHERE id=%d",
                                                    table_name,     new_name,    id);
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
                    char* update_phone_query;
                    sprintf(update_phone_query, "UPDATE %s SET name='%s' WHERE id=%d",
                                                    table_name,     new_phone,    id);
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
                    char* update_addr_query;
                    sprintf(update_addr_query, "UPDATE %s SET name='%s' WHERE id=%d",
                                                    table_name,     new_addr,    id);
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

    char* delete_query;
    sprintf(delete_query, "DELETE FROM %s WHERE id=%d", table_name, target_id);
    if (mysql_query(con, delete_query)) {
        printf("[삭제 실패!!] 해당하는 사람이 없습니다.\n");
        finish_with_error(con);
        return;
    }
}


