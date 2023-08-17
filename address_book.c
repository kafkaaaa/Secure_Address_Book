#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_address_list();
void print_list();
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

Person *head, *tail;
Person *list, *p, *now, *tmp;
int person_cnt = 0; // 주소록에 등록된 사람 수
FILE *fp;


int main() {

    init_address_list();
    int option;

    do {
        printf("======= MENU =======\n");
        printf("  #1. 주소록 조회\n");
        printf("  #2. 주소록 추가\n");
        printf("  #3. 주소록 수정\n");
        printf("  #4. 주소록 삭제\n");
        printf("  #5. 종료\n");
        printf("======================\n");
        printf("Select an option:  ");
        scanf("%d", &option);

        if (option == 1) {
            print_list();
        }
        else if (option == 2) {
            insert_into_list();
        }
        else if (option == 3) {
            update_list();
        }
        else if (option == 4) {
            delete_list();
        }
        else if (option == 5) {
            printf("프로그램을 종료합니다.\n");
            // 종료할 때 리스트에 있는 Data는 File로 저장
            save_file();
            break;
        }
        else {
            printf("1~5만 입력해주세요.\n");
        }
    } while (1);

    return 0;
}



// 주소록 파일 불러오기
void load_file()
{
    char file_name[30];

    printf("***주소록 파일을 불러옵니다***\n");
    printf("파일 이름을 입력해주세요:  ");
    scanf("%s", file_name);
    fp = fopen(file_name, "r");

    if (fp != NULL) {
        printf("File Open Success!\n");

        Person *temp = (Person*)malloc(sizeof(Person));
        Person *p = NULL;

        while (1) {
            int isRead = fread(temp, sizeof(Person), 1, fp);
            if (!isRead) break;
            else {
                if (head == NULL) {
                    head = temp;
                    p = temp;
                }
                else {
                    p->next = temp;
                    p = p->next;
                    p->next = NULL;
                }
            }
        }
        free(temp);
        fclose(fp);
    }
    else {
        printf("File Open Error!\n");
    }
    

    //     while (feof(fp)) {
    //         fscanf(fp, "%[^\n]s %[^\n]s %[^\n]s", tmp->name, tmp->phone, tmp->address);
    //         if (head == NULL) {
    //             head = tmp;
    //             tail = tmp;
    //         }
    //         else {
    //             tail->next = tmp;
    //             tail = tmp;
    //         }
    //         tmp = tmp->next;
    //         person_cnt++;
    //     }
    //     break;
    // }
}



// 리스트에 있는 데이터를 파일에 저장하기
void save_file()
{
    FILE* fp = fopen("output.txt", "w");
    int i;
    Person *p = head;

    for (i=0; i<person_cnt; i++) {
        fputs(p->name, fp);
        fputs(p->phone, fp);
        fputs(p->address, fp);
        p = p->next;
    }
}



// 초기화 및 주소록 파일 불러오기
void init_address_list()
{
    // 초기화
    fp = NULL;
    head = NULL, tail = NULL;
    person_cnt = 0;
    tmp = (Person *)malloc(sizeof(Person));

    load_file();
}




// 주소록 출력하기
void print_list()
{
    if (head == NULL) {
        printf("\n주소록이 비어있습니다.\n");
    }
    else {
        printf("\n***전체 주소록을 출력합니다***\n");
        printf("---현재 등록된 사람: %d명---\n", person_cnt);
        printf("이름\t 전화번호\t 주소\n");
        now = head;
        while (now != NULL) {
            // printf("------------------\n");
            printf("%s\t", now->name);
            printf("%s\t", now->phone);
            printf("%s\t", now->address);
            printf("------------------\n");
            now = now->next;
        }
        // printf("\n***전체 주소록을 출력합니다***\n");
        // char buf[MAX] = {0, };
        // fread(buf, 1, MAX, fp);
        // printf("%s\n", buf);
    }
}


// 파일에서 자동으로 불러올때 사용
void insert_list(Person* p) {
    Person* new = (Person*)malloc(sizeof(Person));
    strcpy(new->name, p->name);
    strcpy(new->phone, p->phone);
    strcpy(new->address, p->address);
    if (head == NULL) {
        head = new;
    }
    tail->next = new;
    tail = new;
    person_cnt++;
}


// 직접 수동으로 추가할때
void insert_into_list()
{
    // 주소록에 추가할 사람 노드
    Person* new_person = (Person*)malloc(sizeof(Person));
    new_person->next = NULL;

    printf("주소록에 추가할 데이터를 입력하세요.\n");
    printf("이름: ");
    scanf("%[^\n]s", new_person->name);
    // gets(new_person->name);

    printf("전화번호: ");
    scanf("%[^\n]s", new_person->phone);
    // gets(new_person->phone);

    printf("주소: ");
    scanf("%[^\n]s", new_person->address);
    // gets(new_person->address);

    if (person_cnt == 0) {      // 주소록이 비어있으면 새로운 노드가 헤드임
        head = new_person;
    } else  {
        tail->next = new_person;
    }
    tail = new_person;
    person_cnt++;
}



void update_list()
{

}



void delete_list()
{
    char name[20];
    char phone[20];
    char address[100];
    int del_option;
    Person *prev = NULL, *temp = head;
    

    printf("삭제할 데이터를 선택하세요.\n");
    printf("1.이름 || 2.전화번호 || 3.주소\n");
    scanf("%d", &del_option);
    // 연결리스트의 처음부터 대조하면서 데이터에 해당하는 사람을 주소록에서 삭제
    switch (del_option) {
        case 1:
            printf("주소록에서 삭제할 사람의 [이름]을 입력하세요: ");
            scanf("%[^\n]s", name);
            for (temp = head;  temp->next != NULL;  ) {
                if (strcmp(temp->name, name) == 0) {
                    printf("[%s]을(를) 주소록에서 삭제합니다.", name);
                    prev->next = temp->next;
                    free(temp);
                }
                prev = temp;
                temp = temp->next;
            }
            if (temp->next == NULL) {
                printf("[삭제 실패] 해당하는 사람이 없습니다.\n");
            }
            break;

        case 2:
            printf("주소록에서 삭제할 사람의 [전화번호]를 입력하세요: ");
            scanf("%[^\n]s", phone);
            for (temp = head;  temp->next != NULL;  ) {
                if (strcmp(temp->phone, phone) == 0) {
                    printf("[%s]을(를) 주소록에서 삭제합니다.", phone);
                    prev->next = temp->next;
                    free(temp);
                }
                prev = temp;
                temp = temp->next;
            }
            if (temp->next == NULL) {
                printf("[삭제 실패] 해당하는 사람이 없습니다.\n");
            }
            break;


        case 3:
            printf("주소록에서 삭제할 사람의 [주소]를 입력하세요: ");
            scanf("%[^\n]s", address);
            for (temp = head;  temp->next != NULL;  ) {
                if (strcmp(temp->address, address) == 0) {
                    printf("[%s]을(를) 주소록에서 삭제합니다.", address);
                    prev->next = temp->next;
                    free(temp);
                }
                prev = temp;
                temp = temp->next;
            }
            if (temp->next == NULL) {
                printf("[삭제 실패] 해당하는 사람이 없습니다.\n");
            }
            break;
        

        default:
            printf("잘못 입력하셨습니다.\n");
            break;
    }
}

