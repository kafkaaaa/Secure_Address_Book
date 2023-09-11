#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("MySQL Client Version: %s\n", mysql_get_client_info());
    exit(0);
}
