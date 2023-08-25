#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #pragma warning(disable:4996)
#define MAX 1000
#define DATA_LEN_LIMIT 20
#define FILE_NAME_LIMIT 50

void init_address_list();
void print_list();
void insert_list();
void insert_into_list();
void update_list();
void delete_list();
void load_file();
void save_file();

typedef struct Person {
    int id; // 중복 레코드 문제로 ID 추가
    char name[DATA_LEN_LIMIT];
    char phone[DATA_LEN_LIMIT];
    char address[DATA_LEN_LIMIT];
    struct Person *next;
} Person;

Person *head, *tail, *tmp;
FILE *fp;
int person_cnt; // 주소록에 등록된 사람 수


int main() {

    int menu;
    init_address_list();
    srand((unsigned)time(NULL));

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
        // if (scanf_s("%d", &menu, sizeof(int)) == 0 ) {
        //     printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        // }
        getchar();

        if      (menu == 1)     print_list();
        else if (menu == 2)     insert_into_list();
        else if (menu == 3)     update_list();
        else if (menu == 4)     delete_list();
        else if (menu == 5) {
            printf("프로그램을 종료합니다.\n");
            save_file();
            break;
        }
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

    while (1) {
        printf("***주소록 파일을 불러옵니다***\n");
        printf("파일 이름을 입력해주세요: ");
        scanf("%s", file_name);
        if (strlen(file_name) > FILE_NAME_LIMIT) {
            printf("파일명 길이 초과. 다시 입력해주세요.\n\n");
        }
        // if (scanf_s("%s", file_name, FILE_NAME_LIMIT) == 0) {
        //     printf("파일명 길이 초과. 다시 입력해주세요.\n");
        // }
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
        rewind(fp);
        printf("\nFile Open Success !\n\n");
        while (1) {
            if (feof(fp) != 0) break;
            // fread(temp, sizeof(Person), 1, fp);
            fscanf(fp, "%d, %[^,], %[^,], %[^\n]", &(temp->id), temp->name, temp->phone, temp->address);
            printf("[%d]번째 데이터 읽어오기 성공\n", person_cnt+1);
            insert_list(temp);
        }
    }
    else {
        printf("File Open Error !\n\n");
    }
    free(temp);
    fclose(fp);
}


// #define SAVE_MODE_TXT 1
#define SAVE_MODE_DAT 1

#if defined(SAVE_MODE_TXT)
    #define FOUT_NAME "output.txt"
#elif defined(SAVE_MODE_DAT)
    #define FOUT_NAME "output.dat"
#endif


// 프로그램 종료시 리스트에 있는 Data를 파일로 저장하기
void save_file()
{
    int i;
    FILE* fout_p = fopen(FOUT_NAME, "wb");

    if (fout_p == NULL) {
        printf("[%s] 파일 생성에 실패했습니다!\n", FOUT_NAME);
        return;
    }
    else {
        printf("[%s] 파일이 생성되었습니다.\n", FOUT_NAME);
    }

    Person *p = head->next;

    while (1) {
        p = p->next;
        if (p == NULL) break;
        // text 형태로 저장
        fprintf(fout_p, "%d,", p->id);
        fprintf(fout_p, "%s,%s,%s\n", p->name, p->phone, p->address);

        // binary 형태로 저장 (.dat)
        // fwrite(p, sizeof(Person), 1, fout_p);
    }

    // 마지막 개행문자 제거
    fseek(fout_p, -1, SEEK_END);
    fwrite("\0", 1, 1, fout_p);
    
    // // 마지막 개행문자 제거
    // for (i=sizeof(buf); i>=0; i--) {
    //     if (buf[i-1] == '\n') {
    //         buf[i-1] = '\0';
    //         break;
    //     } 
    // }

    printf("주소록 저장 성공. 프로그램을 종료합니다.\n");
    fclose(fout_p);
}



// 초기작업
void init_address_list()
{
    // 초기화
    fp = NULL;
    head = (Person *)malloc(sizeof(Person));
    tail = (Person *)malloc(sizeof(Person));
    tmp = (Person *)malloc(sizeof(Person));
    head->next = tail;
    tail->next = tail;
    person_cnt = 0;

    load_file();    // 주소록 파일 불러오기
}


// 전체 주소록 출력하기
void print_list()
{
    if (person_cnt == 0) {
        printf("\n주소록이 비어있습니다.\n");
        return;
    }
    else {
        printf("\n전체 주소록을 출력합니다...\n");
        printf("\n<현재 등록된 사람: %d명>\n", person_cnt);
        printf("----------------------------------------------------------------------\n");
        printf("ID\t\t이름\t\t전화번호\t\t주소\n");
        printf("----------------------------------------------------------------------\n");

        Person* now = head->next->next;
        while (now != NULL) {
            // printf("%d\t\t%-20s%-20s%-20s\n", now->id, now->name, now->phone, now->address);
            printf("%d\t\t%s\t\t%s\t\t%s\n", now->id, now->name, now->phone, now->address);
            now = now->next;
        }
        printf("----------------------------------END----------------------------------\n");
    }
}


// 연결리스트에 한 노드씩 추가하는 함수
void insert_list(Person* p)
{
    Person* new = (Person *)malloc(sizeof(Person)); // 리스트에 추가할 노드
            new->id =       p->id;
    strcpy(new->name,       p->name);
    strcpy(new->phone,      p->phone);
    strcpy(new->address,    p->address);

    if (head == NULL) {
        // head = new;
        head->next = new;
    }
    else {
        tail->next = new;
    }
    tail = new;
    person_cnt++;
}


// 사용자 입력으로 주소록에 추가하는 함수
void insert_into_list()
{
    // 주소록에 추가할 노드 생성 및 값 세팅
    Person* new_person = (Person*)malloc(sizeof(Person));
    new_person->next = NULL;
    new_person->id = rand() % MAX + 1;  // id값은 1 ~ MAX

    printf("주소록에 추가할 데이터를 입력하세요.\n");
    while (1) {
        printf("이름: ");
        scanf(" %[^\n]s", new_person->name);
        // getchar();
        if (strlen(new_person->name) > DATA_LEN_LIMIT) {
            printf("입력 길이 초과!! 다시 입력해주세요.\n");
        }
        else break;
        // if (scanf_s("%s", new_person->name, DATA_LEN_LIMIT) == 0) {
        //     printf("입력 길이 초과!! 다시 입력해주세요.\n");
        // }
    }

    while (1) {
        printf("전화번호: ");
        scanf(" %[^\n]s", new_person->phone);
        // getchar();
        if (strlen(new_person->phone) > DATA_LEN_LIMIT) {
            printf("입력 길이 초과!! 다시 입력해주세요.\n");
        }
        else break;
    }
        // if (scanf_s("%s", new_person->phone, DATA_LEN_LIMIT) == 0) {
        //     printf("입력 길이 초과!! 다시 입력해주세요.\n");
        // }

    while (1) {
        printf("주소: ");
        scanf(" %[^\n]s", new_person->address);
        // getchar();
        if (strlen(new_person->address) > DATA_LEN_LIMIT) {
            printf("입력 길이 초과!! 다시 입력해주세요.\n");
        }
        else break;
        // if (scanf_s("%s", new_person->address, DATA_LEN_LIMIT) == 0) {
            // printf("입력 길이 초과!! 다시 입력해주세요.\n");
        // }
    }

    if (head == NULL) {
        head->next = new_person;
    } else  {
        tail->next = new_person;
    }
    tail = new_person;
    person_cnt++;
}


// 주소록 수정
void update_list()
{
    if (person_cnt <= 0) {
        printf("주소록이 비어 있습니다. 수정할 데이터가 없습니다.\n");
        return;
    }

    int id, update_option;
    char name[20] = {0};
    char new_name[20] = {0};
    char phone[20] = {0};
    char new_phone[20] = {0};
    char address[100] = {0};
    char new_address[100] = {0};

    printf("----주소록을 수정합니다----\n");
    while (1) {
        printf("수정할 사람의 ID를 입력해주세요: ");
        scanf("%d", &id);
        if (id < 1 || id > MAX) {
            printf("잘못 입력하셨습니다!! 다시 입력해주세요.\n");
        }
        else break;
        // if (scanf_s("%d", id, sizeof(id)) == 0) {
        //     printf("입력 길이 초과!! 다시 입력해주세요.\n");
        // }
        // else break;
    }

    while (1) {
        printf("[ID=%d]인 사람의 데이터를 수정합니다.\n", id);
        printf("1.이름  2.전화번호  3.주소\n");
        printf("수정할 항목을 선택해주세요: ");
        // printf("수정할 사람의 이름을 입력해주세요: ");
        // scanf("%s", name);
        // printf("[%s]의 데이터를 수정합니다.\n", name);
        scanf("%d", &update_option);
        if (update_option < 1 || update_option > 3) {
            printf("잘못된 입력입니다!! 다시 입력해주세요.\n");
        }
        else break;
        // if (scanf_s("%d", update_option, 1) == 0) {
        //     printf("잘못된 입력입니다!! 다시 입력해주세요.\n");
        // }
        // else break;
    }

    // 데이터 수정하기
    Person *update_p = head;

    switch (update_option) {
        case 1:
            while (1) {
                printf("새로운 이름을 입력해주세요: ");
                scanf("%s", new_name);
                if (strlen(new_name) > DATA_LEN_LIMIT) {
                    printf("입력 길이 초과!! 다시 입력해주세요.\n");
                }
                else break;
            }
            // while (1) {
            //     if (scanf_s("%s", new_name, DATA_LEN_LIMIT) == 0) {
            //         printf("입력 길이 초과! 다시 입력해주세요.\n");
            //     }
            //     else break;
            // }

            // ID가 일치하는 사람 찾아서 수정하기
            while (update_p != NULL) {
                if (update_p->id == id) {
                // if (strcmp(update_p->name, name) == 0) {
                    strcpy(update_p->name, new_name);
                    printf("[수정 완료!]\n");
                    break;
                }
                update_p = update_p->next;
            }
            if (update_p == NULL) {
                printf("[수정 실패!] 해당하는 사람이 없습니다.\n");
            }
            break;

        case 2:
            while (1) {
                printf("새로운 전화번호를 입력해주세요: ");
                scanf("%s", new_phone);
                if (strlen(new_phone) > DATA_LEN_LIMIT) {
                    printf("입력 길이 초과!! 다시 입력해주세요.\n");
                }
                else break;
                // while (1) {
                //     if (scanf_s("%s", new_phone, DATA_LEN_LIMIT) == 0) {
                //         printf("입력 길이 초과. 다시 입력해주세요.\n");
                //     }
                //     else break;
                // }
            }

            while (update_p != NULL) {
                if (update_p->id == id) {
                // if (strcmp(update_p->name, name) == 0) {
                    strcpy(update_p->phone, new_phone);
                    printf("[수정 완료!]\n");
                    break;
                }
                update_p = update_p->next;
            }
            if (update_p == NULL) {
                printf("[수정 실패!] 해당하는 사람이 없습니다.\n");
            }
            break;


        case 3:
            while (1) {
                printf("새로운 주소를 입력해주세요: ");
                scanf("%s", new_address);
                if (strlen(new_address) > DATA_LEN_LIMIT) {
                    printf("입력 길이 초과!! 다시 입력해주세요.\n");
                }
                else break;
                // while (1) {
                //     if (scanf_s("%s", new_address, DATA_LEN_LIMIT) == 0) {
                //         printf("입력 길이 초과. 다시 입력해주세요.\n");
                //     }
                //     else break;
                // }
            }

            while (update_p != NULL) {
                if (update_p->id == id) {
                // if (strcmp(update_p->name, name) == 0) {
                    strcpy(update_p->address, new_address);
                    printf("[수정 완료!]\n");
                    break;
                }
                update_p = update_p->next;
            }
            if (update_p == NULL) {
                printf("[수정 실패!] 해당하는 사람이 없습니다.\n");
            }
            break;
    }
}


void delete_list()
{
    if (person_cnt <= 0) {
        printf("주소록이 비어 있습니다. 삭제할 데이터가 없습니다.\n");
        return;
    }

    int id, del_option;
    char name[DATA_LEN_LIMIT] = "";
    char phone[DATA_LEN_LIMIT] = "";
    char address[DATA_LEN_LIMIT] = "";
    Person *prev = NULL, *temp = head;
    
    // printf("\n삭제할 데이터를 선택하세요.\n");
    // printf("1.이름  |  2.전화번호  |  3.주소\n");
    // scanf("%d", &del_option);
    while (1) {
        printf("주소록에서 삭제할 ID를 입력해주세요: ");
        scanf("%d", &id);
        if (id < 1 || id > MAX) {
            printf("잘못된 입력입니다!! 다시 입력해주세요.\n");
        }
        else break;
        // if (scanf_s("%d", id, MAX) == 0) {
        //     printf("입력 길이 초과. 다시 입력해주세요.\n");
        // }
        // else break;
    }
    
    while (temp != NULL) {
        if(temp->id == id) {
            if (temp == head) {     // 첫 번째 노드를 삭제하는 경우
                if (head->next == NULL) {
                    head = tail = NULL;
                    person_cnt--;
                    printf("[ID= %d]인 사람을 삭제했습니다.\n", id);
                }
                else {
                    head = head->next;
                    person_cnt--;
                    printf("[ID= %d]인 사람을 삭제했습니다.\n", id);
                }
            }
            else if (tail == temp) {    // 마지막 노드를 삭제하는 경우
                prev->next = NULL;
                tail = prev;
                person_cnt--;
                printf("[ID= %d]인 사람을 삭제했습니다.\n", id);
            }
            else {                      // 중간 노드를 삭제하는 경우
                prev->next = temp->next;
                person_cnt--;
                printf("[ID= %d]인 사람을 삭제했습니다.\n", id);
            }
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    
    if (temp == NULL) {
        printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
    }
}



    // 연결리스트의 처음부터 대조하면서 일치하는 노드를 제거
    // switch (del_option) {
    //     case 0:            
    //         printf("주소록에서 삭제할 사람의 [ID]를 입력하세요: ");
    //         getchar();  // scanf 입력버퍼 문제
    //         scanf("%d", id);

    //         while (temp != NULL) {
    //             if (strcmp(temp->id, id) == 0) {
    //                 if (temp == head) {         // 첫 번째 노드를 삭제하는 경우
    //                     if (head->next == NULL) {
    //                         printf("[%s] ([ID=%d])을(를) 주소록에서 삭제합니다.\n", name, id);
    //                         head = tail = NULL;
    //                         person_cnt--;
    //                     }
    //                     else {
    //                         printf("[%s] ([ID=%d])을(를) 주소록에서 삭제합니다.\n", name, id);
    //                         head = head->next;
    //                         person_cnt--;
    //                     }
    //                 }
    //                 else if (tail == temp) {    // 마지막 노드를 삭제하는 경우
    //                     printf("[%s] ([ID=%d])을(를) 주소록에서 삭제합니다.\n", name, id);
    //                     prev->next = NULL;
    //                     tail = prev;
    //                     person_cnt--;
    //                 }
    //                 else {                      // 중간 노드를 삭제하는 경우
    //                     printf("[%s] ([ID=%d])을(를) 주소록에서 삭제합니다.\n", name, id);
    //                     prev->next = temp->next;
    //                     person_cnt--;
    //                 }
    //                 break;
    //             }
    //             prev = temp;
    //             temp = temp->next;
    //         }

    //         if (temp == NULL) {
    //             printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
    //         }
    //         break;
          

    //     case 1:
    //         printf("주소록에서 삭제할 사람의 [이름]을 입력하세요: ");
    //         getchar();
    //         scanf("%s", name);

    //         while (temp != NULL) {
    //             if (strcmp(temp->name, name) == 0) {
    //                 if (temp == head) {         // 첫 번째 노드를 삭제하는 경우
    //                     if (head->next == NULL) {
    //                         printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
    //                         head = tail = NULL;
    //                         person_cnt--;
    //                     }
    //                     else {
    //                         printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
    //                         head = head->next;
    //                         person_cnt--;
    //                     }
    //                 }
    //                 else if (tail == temp) {    // 마지막 노드를 삭제하는 경우
    //                     printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
    //                     prev->next = NULL;
    //                     tail = prev;
    //                     person_cnt--;
    //                 }
    //                 else {                      // 중간 노드를 삭제하는 경우
    //                     printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
    //                     prev->next = temp->next;
    //                     person_cnt--;
    //                 }
    //                 break;
    //             }
    //             prev = temp;
    //             temp = temp->next;
    //         }

    //         if (temp == NULL) {
    //             printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
    //         }
    //         break;

    //     case 2:
    //         printf("주소록에서 삭제할 사람의 [전화번호]를 입력하세요: ");
    //         getchar();
    //         scanf("%s", phone);

    //         while (temp != NULL) {
    //             if (strcmp(temp->phone, phone) == 0) {
    //                 if (temp == head) {         // 첫 번째 노드를 삭제하는 경우
    //                     if (head->next == NULL) {
    //                         printf("[%s]을(를) 주소록에서 삭제합니다.\n", phone);
    //                         head = tail = NULL;
    //                         person_cnt--;
    //                     }
    //                     else {
    //                         printf("[%s]을(를) 주소록에서 삭제합니다.\n", phone);
    //                         head = head->next;
    //                         person_cnt--;
    //                     }
    //                 }
    //                 else if (tail == temp) {    // 마지막 노드를 삭제하는 경우
    //                     printf("[%s]을(를) 주소록에서 삭제합니다.\n", phone);
    //                     prev->next = NULL;
    //                     tail = prev;
    //                     person_cnt--;
    //                 }
    //                 else {                      // 중간 노드를 삭제하는 경우
    //                     printf("[%s]을(를) 주소록에서 삭제합니다.\n", phone);
    //                     prev->next = temp->next;
    //                     person_cnt--;
    //                 }
    //                 break;
    //             }
    //             prev = temp;
    //             temp = temp->next;
    //         }

    //         if (temp == NULL) {
    //             printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
    //         }
    //         break;        

    //     case 3:
    //         printf("주소록에서 삭제할 사람의 [주소]를 입력하세요: ");
    //         getchar();
    //         scanf("%s", address);

    //         while (temp != NULL) {
    //             if (strcmp(temp->address, address) == 0) {
    //                 if (temp == head) {         // 첫 번째 노드를 삭제하는 경우
    //                     if (head->next == NULL) {
    //                         printf("[%s]을(를) 주소록에서 삭제합니다.\n", address);
    //                         head = tail = NULL;
    //                         person_cnt--;
    //                     }
    //                     else {
    //                         printf("[%s]을(를) 주소록에서 삭제합니다.\n", address);
    //                         head = head->next;
    //                         person_cnt--;
    //                     }
    //                 }
    //                 else if (tail == temp) {    // 마지막 노드를 삭제하는 경우
    //                     printf("[%s]을(를) 주소록에서 삭제합니다.\n", address);
    //                     prev->next = NULL;
    //                     tail = prev;
    //                     person_cnt--;
    //                 }
    //                 else {                      // 중간 노드를 삭제하는 경우
    //                     printf("[%s]을(를) 주소록에서 삭제합니다.\n", address);
    //                     prev->next = temp->next;
    //                     person_cnt--;
    //                 }
    //                 break;
    //             }
    //             prev = temp;
    //             temp = temp->next;
    //         }

    //         if (temp == NULL) {
    //             printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
    //         }
    //         break;
        
    //     default:
    //         printf("잘못된 입력입니다.\n");
    //         break;
    // }
// }

