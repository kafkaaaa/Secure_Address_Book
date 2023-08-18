#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_address_list();
void print_list();
void insert_list();
void insert_into_list();
void update_list();
void delete_list();
void load_file();
void save_file();

typedef struct Person {
    char name[20];
    char phone[20];
    char address[100];
    struct Person *next;
} Person;

Person *head, *tail, *tmp;
int person_cnt = 0; // 주소록에 등록된 사람 수
FILE *fp;

int main() {
    init_address_list();
    int menu;

    while (1) {
        printf("\n======= MENU =======\n");
        printf("  #1. 주소록 보기\n");
        printf("  #2. 주소록 추가\n");
        printf("  #3. 주소록 수정\n");
        printf("  #4. 주소록 삭제\n");
        printf("  #5. 종료\n");
        printf("====================\n");
        printf("Select an option->  ");
        scanf("%d", &menu);
        // getchar();

        if (menu == 1) {
            print_list();
        }
        else if (menu == 2) {
            insert_into_list();
        }
        else if (menu == 3) {
            update_list();
        }
        else if (menu == 4) {
            delete_list();
        }
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



// 파일로부터 주소록 데이터 불러오기
void load_file()
{
    char file_name[30];

    printf("***주소록 파일을 불러옵니다***\n");
    printf("파일 이름을 입력해주세요: ");
    scanf("%s", file_name);
    fp = fopen(file_name, "r");
    Person* temp = (Person *)malloc(sizeof(Person));

    if (fp != NULL) {
        printf("File Open Success !\n");

        while (1) {
            if (feof(fp) != 0) break;
            printf("[%d]번째 데이터 읽어오기 성공.\n", person_cnt+1);
            // fscanf(fp, "%s", temp->name);
            // fscanf(fp, "%s", temp->phone);
            // fscanf(fp, "%s", temp->address);
            fscanf(fp, "%s %s %s", temp->name, temp->phone, temp->address);
            insert_list(temp);
        }
    }
    else {
        printf("File Open Error !\n\n");
    }
    free(temp);
    fclose(fp);
}



// 종료할 때 리스트에 있는 Data를 파일로 저장하기
void save_file()
{
    int i;
    FILE* fout_p = fopen("output.txt", "w");
    if (fout_p == NULL) {
        printf("[output.txt] 파일 생성에 실패했습니다!\n");
        return;
    }
    else {
        printf("[output.txt] 파일이 생성되었습니다.\n");
    }

    Person *p = head->next;
    char buf[1000] = "";

    while (1) {
        p = p->next;
        if (p == NULL) break;

        strcat(buf, p->name);
        strcat(buf, " ");
        strcat(buf, p->phone);
        strcat(buf, " ");
        strcat(buf, p->address);
        strcat(buf, "\n");
    }
    // 마지막 개행문자 제거
    for (i=sizeof(buf); i>=0; i--) {
        if (buf[i-1] == '\n') {
            buf[i-1] = '\0';
            break;
        } 
    }

    if (fputs(buf, fout_p) < 0) {
        printf("[ERROR] 파일로 저장하는데 실패했습니다.\n");
    }
    else {
        printf("주소록 저장 성공\n");
    }

    fclose(fout_p);
}



// 초기화 및 주소록 파일 불러오기
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

    load_file();
}




// 주소록 출력하기
void print_list()
{
    if (head == NULL) {
        printf("\n주소록이 비어있습니다.\n");
        return;
    }
    else {
        printf("\n전체 주소록을 출력합니다...\n");
        printf("<현재 등록된 사람: %d명>\n", person_cnt);
        printf("-------------------------------\n");
        printf("이름\t  전화번호\t  주소\n");
        printf("-------------------------------\n");

        Person* now = head->next;
        while (now != NULL) {
            printf("%s\t  %s\t  %s\n", now->name, now->phone, now->address);
            now = now->next;
        }
        printf("--------------END--------------\n");
    }
}


// 파일에서 주소록을 읽어 연결리스트에 (한 노드씩) 추가하는 함수
void insert_list(Person* p) {
    Person* new = (Person *)malloc(sizeof(Person)); // 리스트에 추가할 노드
    strcpy(new->name, p->name);
    strcpy(new->phone, p->phone);
    strcpy(new->address, p->address);

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


// 사용자 입력으로 추가할때 사용하는 함수
void insert_into_list()
{
    // 주소록에 추가할 사람 노드
    Person* new_person = (Person*)malloc(sizeof(Person));
    new_person->next = NULL;

    printf("주소록에 추가할 데이터를 입력하세요.\n");
    printf("이름: ");
    scanf("%s", new_person->name);

    printf("전화번호: ");
    scanf("%s", new_person->phone);

    printf("주소: ");
    scanf("%s", new_person->address);

    if (head == NULL) {
        head->next = new_person;
    } else  {
        tail->next = new_person;
    }
    tail = new_person;
    person_cnt++;
}



void update_list()
{
    if (person_cnt <= 0) {
        printf("삭제할 데이터가 없습니다.\n");
        return;
    }

    int update_option;
    char name[20] = "";
    char new_name[20] = "";
    char phone[20] = "";
    char new_phone[20] = "";
    char address[100] = "";
    char new_address[100] = "";

    printf("----주소록을 수정합니다----\n");
    printf("수정할 사람의 이름을 입력해주세요: ");
    scanf("%s", name);
    printf("[%s]의 데이터를 수정합니다.\n", name);

    printf("1.이름  2.전화번호  3.주소\n");
    printf("수정할 항목을 선택해주세요: ");
    scanf("%d", &update_option);

    Person *update_p = head;
    // 데이터 수정하기
    switch (update_option) {
        case 1: // 이름 수정
            printf("새로운 이름을 입력해주세요: ");
            scanf("%s", new_name);

            // 수정할 사람 찾기
            while (update_p != NULL) {
                if (strcmp(update_p->name, name) == 0) {
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

        case 2: // 전화번호 수정
            printf("새로운 전화번호를 입력해주세요: ");
            scanf("%s", new_phone);

            // 수정할 사람 찾기
            while (update_p != NULL) {
                if (strcmp(update_p->phone, phone) == 0) {
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


        case 3: // 주소 수정
            printf("새로운 주소를 입력해주세요: ");
            scanf("%s", new_address);

            // 수정할 사람 찾기
            while (update_p != NULL) {
                if (strcmp(update_p->address, address) == 0) {
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
        printf("삭제할 데이터가 없습니다.\n");
        return;
    }

    char name[20] = "";
    char phone[20] = "";
    char address[100] = "";
    int del_option;
    Person *prev = NULL, *temp = head;
    
    printf("\n삭제할 데이터를 선택하세요.\n");
    printf("1.이름  |  2.전화번호  |  3.주소\n");
    scanf("%d", &del_option);

    // 연결리스트의 처음부터 대조하면서 일치하는 노드를 제거
    switch (del_option) {
        case 1:
            printf("주소록에서 삭제할 사람의 [이름]을 입력하세요: ");
            getchar();  // scanf 입력버퍼 문제
            scanf("%s", name);

            while (temp != NULL) {
                if (strcmp(temp->name, name) == 0) {
                    if (temp == head) {         // 첫 번째 노드를 삭제하는 경우
                        if (head->next == NULL) {
                            printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
                            head = tail = NULL;
                            person_cnt--;
                        }
                        else {
                            printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
                            head = head->next;
                            person_cnt--;
                        }
                    }
                    else if (tail == temp) {    // 마지막 노드를 삭제하는 경우
                        printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
                        prev->next = NULL;
                        tail = prev;
                        person_cnt--;
                    }
                    else {                      // 중간 노드를 삭제하는 경우
                        printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
                        prev->next = temp->next;
                        person_cnt--;
                    }
                    break;
                }
                prev = temp;
                temp = temp->next;
            }

            if (temp == NULL) {
                printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
            }
            break;

            // prev = head;
            // temp = head->next;

            // // 삭제할 노드가 첫 노드인 경우
            // if (strcmp(temp->name, name) == 0) {
            //     printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
            //     head = head->next->next;
            //     person_cnt--;
            // }
            // else {
            //     while (1) {
            //         if (strcmp(temp->name, name) == 0) {
            //             printf("[%s]을(를) 주소록에서 삭제합니다.\n", name);
            //             prev->next = temp->next;
            //             temp = temp->next;
            //             person_cnt--;
                    
            //             if (temp == NULL) {
            //                 printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
            //             }
            //             break;
            //         }
            //     }
            //     break;
            // }
            // break;







            // for (temp = head;  temp->next != NULL;  ) {
            //     if (strcmp(temp->name, name) == 0) {
            //         printf("[%s]을(를) 주소록에서 삭제합니다.", name);
            //         prev->next = temp->next;
            //         free(temp);
            //     }
            //     prev = temp;
            //     temp = temp->next;
            // }
            // if (temp->next == NULL) {
            //     printf("[삭제 실패] 해당하는 사람이 없습니다.\n");
            // }
            // break;

        case 2:
            printf("주소록에서 삭제할 사람의 [전화번호]를 입력하세요: ");
            getchar();  // scanf 입력버퍼 문제
            scanf("%s", phone);

            while (temp != NULL) {
                if (strcmp(temp->phone, phone) == 0) {
                    if (temp == head) {         // 첫 번째 노드를 삭제하는 경우
                        if (head->next == NULL) {
                            printf("[%s]을(를) 주소록에서 삭제합니다.\n", phone);
                            head = tail = NULL;
                            person_cnt--;
                        }
                        else {
                            printf("[%s]을(를) 주소록에서 삭제합니다.\n", phone);
                            head = head->next;
                            person_cnt--;
                        }
                    }
                    else if (tail == temp) {    // 마지막 노드를 삭제하는 경우
                        printf("[%s]을(를) 주소록에서 삭제합니다.\n", phone);
                        prev->next = NULL;
                        tail = prev;
                        person_cnt--;
                    }
                    else {                      // 중간 노드를 삭제하는 경우
                        printf("[%s]을(를) 주소록에서 삭제합니다.\n", phone);
                        prev->next = temp->next;
                        person_cnt--;
                    }
                    break;
                }
                prev = temp;
                temp = temp->next;
            }

            if (temp == NULL) {
                printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
            }
            break;        
        
            // printf("주소록에서 삭제할 사람의 [전화번호]를 입력하세요: ");
            // scanf("%[^\n]s", phone);
            // for (temp = head;  temp->next != NULL;  ) {
            //     if (strcmp(temp->phone, phone) == 0) {
            //         printf("[%s]을(를) 주소록에서 삭제합니다.", phone);
            //         prev->next = temp->next;
            //         free(temp);
            //     }
            //     prev = temp;
            //     temp = temp->next;
            // }
            // if (temp->next == NULL) {
            //     printf("[삭제 실패] 해당하는 사람이 없습니다.\n");
            //     break;
            // }
            // break;


        case 3:
            printf("주소록에서 삭제할 사람의 [주소]를 입력하세요: ");
            getchar();  // scanf 입력버퍼 문제
            scanf("%s", address);

            while (temp != NULL) {
                if (strcmp(temp->address, address) == 0) {
                    if (temp == head) {         // 첫 번째 노드를 삭제하는 경우
                        if (head->next == NULL) {
                            printf("[%s]을(를) 주소록에서 삭제합니다.\n", address);
                            head = tail = NULL;
                            person_cnt--;
                        }
                        else {
                            printf("[%s]을(를) 주소록에서 삭제합니다.\n", address);
                            head = head->next;
                            person_cnt--;
                        }
                    }
                    else if (tail == temp) {    // 마지막 노드를 삭제하는 경우
                        printf("[%s]을(를) 주소록에서 삭제합니다.\n", address);
                        prev->next = NULL;
                        tail = prev;
                        person_cnt--;
                    }
                    else {                      // 중간 노드를 삭제하는 경우
                        printf("[%s]을(를) 주소록에서 삭제합니다.\n", address);
                        prev->next = temp->next;
                        person_cnt--;
                    }
                    break;
                }
                prev = temp;
                temp = temp->next;
            }

            if (temp == NULL) {
                printf("[삭제 실패!] 해당하는 사람이 없습니다.\n");
            }
            break;

            // printf("주소록에서 삭제할 사람의 [주소]를 입력하세요: ");
            // scanf("%[^\n]s", address);
            // for (temp = head;  temp->next != NULL;  ) {
            //     if (strcmp(temp->address, address) == 0) {
            //         printf("[%s]을(를) 주소록에서 삭제합니다.", address);
            //         prev->next = temp->next;
            //         free(temp);
            //     }
            //     prev = temp;
            //     temp = temp->next;
            // }
            // if (temp->next == NULL) {
            //     printf("[삭제 실패] 해당하는 사람이 없습니다.\n");
            //     break;
            // }
            // break;
        
        default:
            printf("잘못된 입력입니다.\n");
            break;
    }
}

