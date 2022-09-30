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
    munit_t *s[SMAX];
    munit_t *p[PMAX];
 
    munit_t  *cp;
    munit_t  *mp=m;
    munit_t **sp=s;

    size_t np=0;
    ssize_t d;

    c=loadFile(argv[1]);

    cp=c;

    bool quit=false;

    while(!quit) {
        switch(*cp) {
        case '.': write(1,mp,sizeof(*mp)); break;
        case ',': read(0,mp,sizeof(*mp));  break;
        case '+': (*mp)++; break;
        case '-': (*mp)--; break;
        case '>': mp++; break;
        case '<': mp--; break;
        case '[':
            if(!(*mp)) {
                d=1;
                while(d && *cp) {
                    cp++;
                    d+=(*cp=='[')-(*cp==']');
                }
            }
            break;
        case ']':
            if(*mp) {
                d=1;
                while(d) {
                    cp--;
                    d+=(*cp==']')-(*cp=='[');
                }
            }
            break;
        case '(': 
            p[np++]=c;
            d=1;
            while(d && *cp) {
                cp++;
                d+=(*cp=='(')-(*cp==')');
            }
            break;            
        case ')': cp=*(--sp); break;            
        case ':':
            *(sp++)=cp; 
            cp=p[*mp]; 
            break;            
        case '#':
            while(*cp!='\n' && *cp) {
                cp++;
            }
            break;            
        case '@': 
        case '\0': 
            quit=true; 
            continue;            
        }
        if(*cp!='\0') cp++;
    }
    return *mp;
}
