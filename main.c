#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

enum str_stat {
  ST0, /* Initial state */
  ST1, /* first slash */
  ST2, /* in line comment */
  ST3, /* in comments */
  ST4, /* pre comments */
  ST5, /* in character reteral '' */
  ST6, /* in string reteral "" */
  ST7, /* backslash */
  ST8, /* backslash in reteral '' */
};

int main(int argc, char *argv[]){
  FILE *fp;
  char c;
  enum str_stat st;

  if ( argc != 2 ) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if ( (fp = fopen(argv[1], "r")) == NULL ) {
    perror("fopen error");
    fprintf(stderr, "errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }

  st = ST0;
  while ( (c = getc(fp)) != EOF ) {
    switch (st) {
    case ST0:
      if ( c == '/' ) { st = ST1; }
      else if ( c == '\'' ) { st = ST6; putc(c,stdout); }
      else if ( c == '\"' ) { st = ST5; putc(c,stdout); }
      else if ( c == '\\' ) { st = ST7; }
      else { putc(c,stdout); }
      break;
    case ST1:
      if ( c == '/' ) { st = ST2; }
      else if ( c == '*' ) { st = ST3; }
      else {
	st = ST0;
	putc('/',stdout);
	putc(c,stdout);
      }
      break;
    case ST2:
      if ( c == '\n' ) { st = ST0; putc(c,stdout); }
      break;
    case ST3:
      if ( c == '*' ) { st = ST4; }
      break;
    case ST4:
      if ( c == '/' ) { st = ST0; }
      else { st = ST3; }
      break;
    case ST5:
      if ( c == '\"' ) { st = ST0; }
      putc(c,stdout);
      break;
    case ST6:
      if ( c == '\'' ) { st = ST0; }
      else if ( c == '\\' ) { st = ST8; }
      putc(c,stdout);
      break;
    case ST7:
      st = ST0;
      putc(c,stdout);
      break;
    case ST8:
      st = ST6;
      putc(c,stdout);
      break;
    default:
      fclose(fp);
      fprintf(stderr, "status error\n");
      exit(1);
    }
  }

  fclose(fp);

  return EXIT_SUCCESS;
}
