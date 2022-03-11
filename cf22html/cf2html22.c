/* Convert colorForth to html */
#include <stdio.h>
/*#define _CRT_SECURE_NO_WARNINGS */
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>

char ch[] = {' ', 'r', 't', 'o', 'e', 'a', 'n', 'i',
  's', 'm', 'c', 'y', 'l', 'g', 'f', 'w',
  'd', 'v', 'p', 'b', 'h', 'x', 'u', 'q',
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'j', '-', 'k', '.', 'z', '/',
  ';', ':', '!', '+', '@', '*', ',', '?'};

void print_text (unsigned int t) {
  while (t) {
    if (!(t & 0x80000000)) {
      putchar (ch[t >> 28]);
      t <<= 4;
    } else if ((t & 0xc0000000) == 0x80000000) {
      putchar (ch[8 + ((t >> 27) & 7)]);
      t <<= 5;
    } else {
      putchar (ch[((t >> 28) - 10) * 8 + ((t >> 25) & 7)]);
      t <<= 7;
    }
  }
}

char *function[] = {
	"extension", "execute", "execute", "define", 
	"compile", "compile", "compile", "compilemacro", 
	"execute", "text", "textcapitalized", "textallcaps",
  "variable", "compiler_feedback", "display_macro", "commented_number", 
	"", "", "executehex", "",  
	"", "compilehex", "compilehex", "", 
	"executehex", "", "", "", 
	"", "", "", "commented_number"

};

int print_tags (int p, int t, int lplast) { //lplast last blue string 
  if (p) {
    printf ("</code>");
  }
  if (lplast != 2) {
      if (lplast == 1)
          printf("<br>");
      else
          if (t == 3 && p)
              printf("<br>");
  }
  printf ("<code class=%s>", function[t]);
  if ((t != 3) && (lplast==0))
    putchar (' ');
 // if (lplast>0)
    return 0; 
}

char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void print_hex (unsigned int i) {
  int n = 8, f = 0;
  if (i == 0) {
    putchar ('0'); return;
  }
  while (n--) {
    if (!(i & 0xf0000000)) {
      if (f)
        putchar ('0');
    } else {
      f = 1;
      putchar (hex [i >> 28]);
    }
    i <<= 4;
  }
}

void print_dec (int i) {
  int j, k, f = 0;
  if (i == 0) {
    putchar ('0'); return;
  }
  if (i < 0) {
    putchar ('-'); i = -i;
  }
  for (j = 1000000000; j != 0; j /= 10) {
    k = i / j;
    if (k == 0) {
      if (f)
        putchar ('0');
    } else {
      i -= j * k;
      f = 1;
      putchar (hex[k]);
    }
  }
}

void append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

unsigned int invrt(unsigned int iinvrt){
   unsigned int lin= iinvrt;
   char b0=lin & 0xff;
   char b1=lin & 0xff00;
   char b2=lin & 0xff0000;
   char b3=lin & 0xff000000;
   lin=b0 & 0xff000000 + b1 & 0xff0000 + b2 & 0xff00 + b3;
   return lin;
}

#define BUFSZ 4

int main (int argc, char **argv) {

 //   unsigned char t[BUFSZ] = {0};
//	unsigned char n[BUFSZ] = {0};
    //size_t bytes = 0, i, readsz = sizeof buf;
    FILE *fp = argc > 1 ? fopen (argv[1], "rb") : stdin;

    if (!fp) {
        fprintf (stderr, "error: file open failed '%s'.\n", argv[1]);
        return 1;
    }
   char bstr[50]=""; 
   int lastp=0;
   int b=0, bref=0; /* block */
  /* read/output BUFSZ bytes at a time */
  /*  while ((bytes = fread (buf, sizeof *buf, readsz, fp)) == readsz) {
        for (i = 0; i < readsz; i++)
            printf (" 0x%02x", buf[i]);
        putchar ('\n');
    }  
    for (i = 0; i < bytes; i++) /* output final partial buf */
  /*      printf (" 0x%02x", buf[i]);
    putchar ('\n');

    if (fp != stdin)
        fclose (fp);

    return 0;
}*/



/*int main () {*/
   if (argc == 3) {
       char* a = argv[2];
       b = atoi(a);
   }
   if (argc == 4) {
       char* a = argv[3];
       bref = atoi(a);
   }
   /*int b = 18,*/
   int w=256, p = 0, /*t, n,*/ pos = 0;
   unsigned int t, n;
  //pos = 0;
  printf ("<html>\n");
  printf ("<link rel=stylesheet type=\"text/css\" href=\"colorforth.css\">\n");
  printf("  <style type=\"text/css\">\n");
  printf("  body { margin-right:10%;}\n");
  printf("  div.code {\n");
  /*printf("    width:100%;\n");*/
  printf("    padding:0.5em;\n");
  printf("    background-color:black;\n");
  printf("    font-size:xx-large;\n");
  printf("    font-weight:bold;\n");
  printf("    text-transform:lowercase;\n");
  printf("  }\n");
  printf("  code.define { color:red; }\n");
  printf("  code.compile { color:#00ff00; }\n");
  printf("  code.compilehex { color:green; }\n");
  printf("  code.execute { color:yellow; }\n");
  printf("  code.executehex { color:#c0c000; }\n");
  printf("  code.compilemacro { color:#00ffff; }\n");
  printf("  code.variable {color:#ff00ff; }\n");
  printf("  code.text { color:white; }\n");
  printf("  code.textcapitalized { color:white; text-transform:capitalize; }\n");
  printf("  code.textallcaps { color:white; text-transform:uppercase; }\n");
  printf("  code.display_macro { color:#0000FF; }\n");
  printf("  code.compiler_feedback { color:grey; }\n");
  printf("  code.commented_number { color:white; }\n");
  printf("  </style>\n");
  w = 256;
  if (b!=0)
      fseek(fp, (b-0)*w*4, SEEK_SET);
  if (fread (&t, 4, 1, fp) == 0) return 0;
  /*t = invrt(t);*/
  if ((t & 0xF ) == 0)
      t = t | 0x1;
  pos = 4;
  lastp = 0;
  while (1) {
    printf ("{block %d}\n", b+bref); /* b block*/
    printf ("<div class=code>\n"); /* div class code */
    w = 256;
    while (w--) {
      printf("<!-- pos: %d -->", pos);
      // 
      //if ((t & 0xF) == 0)
      //    t = t | 0x1;
      switch (t & 0xf) {
		 case 14: //blue text
             char* s = bstr;
             s[0] = '\0';
             lastp = print_tags(p, t & 0xf, lastp);
             print_text(t & 0xfffffff0);
             t = t & 0xfffffff0;
		      while (t) {
                 if (!(t & 0x80000000)) {
                     append(bstr,ch[t >> 28]);
                     t <<= 4;
                  } else if ((t & 0xc0000000) == 0x80000000) {
                     append(bstr,ch[8 + ((t >> 27) & 7)]);
                     t <<= 5;
                  } else {
                      append(bstr,ch[((t >> 28) - 10) * 8 + ((t >> 25) & 7)]);
                     t <<= 7;
                  }
               }
          char str[60];
          sprintf_s(str, sizeof(str), "blue value |%s| \0", bstr);
          OutputDebugStringA(str);
		    if ((strcmp(bstr, "*") == 0) || 
 			    (strcmp(bstr, "-cr") == 0) ||
				(strcmp(bstr, ".") == 0))
			{
			  lastp=2 ;
			} 
			else if ((strcmp(bstr, "cr") == 0) ||
					(strcmp(bstr, "br") == 0) ||
					(strcmp(bstr, ",") == 0))
			{
			  lastp=1 ; // put <cr>
			}
			/* more else if clauses */
			/*else/*/ /* default: */
		/*	{
			} */
			break;
        case 0:
          print_text (t & 0xfffffff0);
          break;
        case 2: case 5:
          lastp=print_tags (p, t & 0x1f,lastp);
          if (w == 0)
            break;
          fread (&n, 4, 1, fp);
          /*n = invrt(n);*/
          pos += 4;
          w--;
          if (t & 0x10)
            print_hex (n);
          else
            print_dec (n);
          break;
        case 6: case 8: case 0xf:
          lastp=print_tags (p, t & 0x1f,lastp);
          if (t & 0x10)
            print_hex (t >> 5);
          else
            print_dec (t >> 5);
          break;
        case 0xc: // variable
          lastp=print_tags (p, t & 0xf,lastp);
          print_text (t & 0xfffffff0);
          if (w == 0)
            break;
          fread (&t, 4, 1, fp); 
          /*t = invrt(t);*/
          pos += 4;
          w--;
          lastp=print_tags (1, 4, lastp);
          print_dec (t);
          break;
        default:
          lastp=print_tags (p, t & 0xf,lastp);
          print_text (t & 0xfffffff0);
          break;
      }
      p = 1;
      if (fread (&t, 4, 1, fp) == 0) {
        printf ("</code>\n</div>\n");
        goto end;
      }
      pos += 4;
    }
    if (p) {
      printf ("</code>\n");
    }
    p = 0;
    printf ("</div>\n<hr>\n");
    b++;
  }
end:
  printf ("</html>\n");
  if (fp != stdin)
        fclose (fp);
  return 0;
}
