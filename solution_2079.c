// Solution for problem 2079
// Auto-generated from solutions/2079.mv

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MEMORYSIZE 65536
#define CODESIZE 65536
#define uint8 unsigned char
#define uint16 unsigned short

long c;                             
uint16 p = 0, q = 0;                       
uint8 reg[26];                      
uint8 mem[MEMORYSIZE];              
FILE * program, * input;                       

struct instruct {
    uint8 src_mode, dest_mode;     
    uint16 src, dest, src_offset, dest_offset;
    uint16 line;
} code[CODESIZE];

void err(char * s) {
    fprintf(stderr, "Error detected at line %hu: %s\n", p, s);
    if (input != stdin) fclose(input);
    exit(1);
}

void preprocess() {
    char * line = NULL;
    char src[16] = "", dest[16] = "";
    char buf1[2] = "", buf2[2] = "";
    int buf3 = 0;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, program)) != -1) {
        p++;
        if (line[0] == '#' || line[0] == '\r' || line[0] == '\n')
            continue;
        if (sscanf(line, "%*[ |\t]%[^<]<%[^ |\t|\r|\n]", dest, src) != 2)
            if (sscanf(line, "%[^<]<%[^ |\t|\r|\n]", dest, src) != 2)
                err("Syntax Error");

        if (src[0] == '[') {
            if (sscanf(src, "[%1[A-Z]+%1[A-Z]", buf1, buf2) == 2) {
                code[q].src_mode = 4;
                code[q].src = buf1[0] - 'A';
                code[q].src_offset = buf2[0] - 'A';
                if (code[q].src_offset == 8 || code[q].src_offset == 14)
                    err("I/O can't be used here");
            } else if (sscanf(src, "[%1[A-Z]+%d", buf1, &buf3) == 2) {
                code[q].src_mode = 3;
                code[q].src = buf1[0] - 'A';
                code[q].src_offset = (uint16)buf3;
            } else if (sscanf(src, "[%1[A-Z]", buf1) == 1) {
                code[q].src_mode = 2;
                code[q].src = buf1[0] - 'A';
                code[q].src_offset = 0;
            } else if (sscanf(src, "[%d", &buf3) == 1) {
                code[q].src_mode = 5;
                code[q].src = (uint16)buf3;
                code[q].src_offset = 0;
            } else {
                err("Syntax Error");
            }
            if ((code[q].src == 8 || code[q].src == 14) && code[q].src_mode != 5)
                err("I/O can't be used here");
        } else if (src[0] >= 'A' && src[0] <= 'Z') {
            code[q].src_mode = 1;
            code[q].src = src[0] - 'A';
            code[q].src_offset = 0;
            if (code[q].src == 14)
                err("O can't be used as src");
        } else if (sscanf(src, "%d", &buf3) == 1) {
            code[q].src_mode = 0;
            code[q].src = (uint16)buf3;
            code[q].src_offset = 0;
        } else {
            err("Syntax Error");
        }

        if (dest[0] == '[') {
            if (sscanf(dest, "[%1[A-Z]+%1[A-Z]", buf1, buf2) == 2) {
                code[q].dest_mode = 4;
                code[q].dest = buf1[0] - 'A';
                code[q].dest_offset = buf2[0] - 'A';
                if (code[q].dest_offset == 8 || code[q].dest_offset == 14)
                    err("I/O can't be used here");
            } else if (sscanf(dest, "[%1[A-Z]+%d", buf1, &buf3) == 2) {
                code[q].dest_mode = 3;
                code[q].dest = buf1[0] - 'A';
                code[q].dest_offset = (uint16)buf3;
            } else if (sscanf(dest, "[%1[A-Z]", buf1) == 1) {
                code[q].dest_mode = 2;
                code[q].dest = buf1[0] - 'A';
                code[q].dest_offset = 0;
            } else if (sscanf(dest, "[%d", &buf3) == 1) {
                code[q].dest_mode = 5;
                code[q].dest = (uint16)buf3;
                code[q].dest_offset = 0;
            } else {
                err("Syntax Error");
            }
            if ((code[q].dest == 8 || code[q].dest == 14) && code[q].dest_mode != 5)
                err("I/O can't be used here");
        } else if (dest[0] >= 'A' && dest[0] <= 'Z') {
            code[q].dest_mode = 1;
            code[q].dest = dest[0] - 'A';
            code[q].dest_offset = 0;
            if (code[q].dest == 8)
                err("I can't be used as dest");
        } else if (sscanf(dest, "%d", &buf3) == 1) {
            err("Dest can't be immediate");
        } else {
            err("Syntax Error");
        }
        code[q].line = p;
        q++;
    }
    free(line);
}

int main() {
    const char *mv_program = "# echo - Problem 2279\n"
        "# Read input and output the same content\n"
        "# When input ends, I returns 0\n"
        "A<I\n"
        "# Check if A is 0, if so halt\n"
        "[0]<1\n"
        "[1]<0\n"
        "B<[A]\n"
        "# If A is 0, B will be 1, else 0\n"
        "Z<B\n"
        "# Output A\n"
        "O<A\n"
        "";
    
    program = fmemopen((void*)mv_program, strlen(mv_program), "r");
    if (!program) {
        perror("fmemopen");
        return 1;
    }
    
    input = stdin;
    preprocess();
    fclose(program);
    
    while (true){
        for (uint16 i = 0; i < q; i++) {
            p = code[i].line;
            uint8 src = 0;
            if (code[i].src == 8 && code[i].src_mode == 1) {
                if ((c = fgetc(input)) != EOF) src = c;
            } else {
                switch (code[i].src_mode) {
                    case 0: src = code[i].src;                                          break;
                    case 1: src = reg[code[i].src];                                     break;
                    case 2: src = mem[reg[code[i].src]];                                break;
                    case 3: src = mem[reg[code[i].src] + code[i].src_offset];           break;
                    case 4: src = mem[reg[code[i].src] + reg[code[i].src_offset]];      break;
                    case 5: src = mem[code[i].src];                                     break;
                    default: err("Syntax Error");
                }
            }
            if (code[i].dest == 14 && code[i].dest_mode == 1) {
                putchar(src);
            } else if(code[i].dest == 25 && code[i].dest_mode == 1){
                if(src != 0) {
                    exit(0);
                }
            } else {
                switch (code[i].dest_mode) {
                    case 1: reg[code[i].dest] = src;                                    break;
                    case 2: mem[reg[code[i].dest]] = src;                               break;
                    case 3: mem[reg[code[i].dest] + code[i].dest_offset] = src;         break;
                    case 4: mem[reg[code[i].dest] + reg[code[i].dest_offset]] = src;    break;
                    case 5: mem[code[i].dest] = src;                                    break;
                    default: err("Syntax Error");
                }
            }
        }
    }
    exit(0);
}

