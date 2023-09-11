#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int main(int argc, char **argv)
{
  int status = 0;

  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "abcde", "abcde",
          "testdb", 0, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "SELECT name FROM cars WHERE id=2;\
      SELECT name FROM cars WHERE id=3;SELECT name FROM cars WHERE id=6"))
  {
      finish_with_error(con);
  }

  do {
      MYSQL_RES *result = mysql_store_result(con);

      if (result == NULL)
      {
          finish_with_error(con);
      }

      MYSQL_ROW row = mysql_fetch_row(result);

      printf("%s\n", row[0]);

      mysql_free_result(result);

      status = mysql_next_result(con);

      if (status > 0) {
          finish_with_error(con);
      }

  } while(status == 0);

  mysql_close(con);
  exit(0);
}