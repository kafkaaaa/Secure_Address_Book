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
  FILE *fp = fopen("test2.jpg", "wb");

  if (fp == NULL)
  {
      fprintf(stderr, "cannot open image file\n");
      exit(1);
  }

  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "abcde", "abcde",
          "testdb", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "SELECT data FROM images WHERE id=1"))
  {
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL)
  {
      finish_with_error(con);
  }

  MYSQL_ROW row = mysql_fetch_row(result);
  unsigned long *lengths = mysql_fetch_lengths(result);

  if (lengths == NULL) {
      finish_with_error(con);
  }

  fwrite(row[0], lengths[0], 1, fp);

  if (ferror(fp))
  {
      fprintf(stderr, "fwrite() failed\n");
      mysql_free_result(result);
      mysql_close(con);

      exit(1);
  }

  int r = fclose(fp);

  if (r == EOF) {
      fprintf(stderr, "cannot close file handler\n");
  }

  mysql_free_result(result);
  mysql_close(con);

  exit(0);
}