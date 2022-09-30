#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define CMAX 65536 
#define MMAX 65536 
#define PMAX 256
#define SMAX 256

typedef unsigned char munit_t; 

unsigned char *loadFile(char *path) {
    unsigned char *source = NULL;

    FILE *fp = fopen(path, "r");

    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_END) == 0) {
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            source = malloc(sizeof(*source) * (bufsize + 1));

            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            size_t newLen = fread(source, sizeof(*source), bufsize, fp);
            if (newLen == 0) {
                fprintf(stderr,"Error: cannot read file %s", path);
                free(source);
                source = NULL;
            } else {
                source[newLen+1] = '\0';
            }
        }
        fclose(fp);
    } else {
        perror(path);
    }
    
    return source;
}

int main(int argc,char **argv) {

    munit_t *c=NULL;
    munit_t  m[MMAX]={0};

    size_t  s[SMAX];
    size_t  p[PMAX];
 
    size_t cp=0;
    size_t mp=0;
    size_t sp=SMAX;

    size_t np=0;
    ssize_t d;

    if(argc!=2) {
        return -1;
    }
    
    c=loadFile(argv[1]);
    
    bool quit=false;

    while(!quit) {
        switch(c[cp]) {
        case '.': write(1,&m[mp],sizeof(munit_t)); break;
        case ',': read(0,&m[mp],sizeof(munit_t));  break;
        case '+': m[mp]++; break;
        case '-': m[mp]--; break;
        case '>': mp++; break;
        case '<': mp--; break;
        case '[':
            if(m[mp]==0) {
                d=1;
                while(c[cp] && d) {
                    cp++;
                    d+=(c[cp]=='[')-(c[cp]==']');
                }
            }
            break;
        case ']':
            if(m[mp]!=0) {
                d=1;
                while(cp && d) {
                    cp--;
                    d+=(c[cp]==']')-(c[cp]=='[');
                }
            }
            break;
        case '(': 
            p[np++]=cp;
            d=1;
            while(c[cp] && d) {
                cp++;
                d+=(c[cp]=='(')-(c[cp]==')');
            }
            break;            
        case ')': cp=s[sp++]; break;            
        case ':':
            s[--sp]=cp; 
            cp=p[m[mp]]; 
            break;            
        case '#':
            cp++;
            while(c[cp] && c[cp]!='\n') {
                cp++;
            }
            break;            
        case '/':
            if(c[cp+1]=='/') {
                cp+=2;
                while(c[cp] && c[cp]!='\n') {
                    cp++;
                }     
            } else if(c[cp+1]=='*') {
                cp+=2;
                d=1;
                while(c[cp] && (c[cp]!='*' || c[cp+1]!='/')) {
                    cp++;
                }
                cp+=2;
            }
            break;
        case '@': 
        case '\0': 
            quit=true; 
            continue;            
        }
        cp+=cp!='\0';
    }
    return m[mp];
}


