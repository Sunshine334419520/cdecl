/**
 * This is a C program for statement of the C language,
 * it will the C language translation into the popular language.
 * @Author: sunshine
 * @Date:   2018-01-14T14:45:12+08:00
 * @Email:  guang334419520@126.com
 * @Filename: cdecl.c
 * @Last modified by:   sunshine
 * @Last modified time: 2018-01-14T14:55:16+08:00
 */




#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOKENS 100
#define MAXTOKENLEN 64

/*
 * Save the statement type
 */
enum Type_tag { Identifire,
                Qualifier,
                Type
};

/*
 * The data sturcture of tag.
 */
struct Token {
    char type;                  /* type of tag  */
    char string[MAXTOKENLEN];   /* string of tag*/
};

int top = -1;       /*  stack top   */
/* This is a stack for Save all logo of before the first logo */
struct Token stack[MAXTOKENS];

/* Save tag of the just read. */
struct Token this;

#define pop stack[top--]    /*  out stack   */
#define push(s) stack[++top] = s    /*  Into the stack  */

/*
 * class for the tag
 * @param   void
 * return   Type_tag
 */
enum Type_tag classify_string(void)
{
    char* s = this.string;

    if(!strcmp(s, "const")) {
        strcpy(s, "read-only");
        return Qualifier;
    }

    if(!strcmp(s, "volatile"))  return Qualifier;
    if(!strcmp(s, "void"))      return Type;
    if(!strcmp(s, "char"))      return Type;
    if(!strcmp(s, "int"))       return Type;
    if(!strcmp(s, "signed"))    return Type;
    if(!strcmp(s, "unsigned"))  return Type;
    if(!strcmp(s, "short"))     return Type;
    if(!strcmp(s, "long"))      return Type;
    if(!strcmp(s, "float"))     return Type;
    if(!strcmp(s, "double"))    return Type;
    if(!strcmp(s, "struct"))    return Type;
    if(!strcmp(s, "union"))     return Type;
    if(!strcmp(s, "enum"))      return Type;

    return Identifire;

}

/*
 * Read tag into the this
 * @param   void
 * return   void
 */
void GetToken(void)
{
    char* p = this.string;

    /* Jump the blank space */
    while( (*p = getchar()) == ' ')
        ;

    if(isalnum(*p)) {   /* identifier is A-Z or 0-9 */
        /* It's not a-z or 0-9. */
        while(isalnum(*++p = getchar()))
                ;
        ungetc(*p, stdin);
        *p = '\0';
        this.type = classify_string();
        return ;
    }

    if(*p == '*') { /*  identifier is '*'   */
        strcpy(this.string, "pointer to");
        this.type = '*';
        return ;
    }

    /* A single character */

    this.string[1] = '\0';
    this.type = *p;
    return;
}

/*
 * Get first the identifier
 * @param   void
 * return   void
 */
void read_to_first_identifier(void)
{
    GetToken();

    /*  Find the first Identifire */
    while(this.type != Identifire) {
        push(this);     /*  this push   */

        GetToken();     /* Get next the tag */
    }

    printf("%s is ", this.string);
    GetToken();

}

/*
 * To deal with arrays
 */
void deal_with_arrays(void)
{
    while(this.type == '[') {
        printf("array ");
        GetToken();      /* number or ']' */

        if(isdigit(this.string[0])) {   /* is number */
            printf("0..%d ", atoi(this.string) - 1);
            /* read the ']' */
            GetToken();
        }

        /*  chech for '[][]' */
        GetToken();
        printf("of ");
    }
}

/*
 * To deal with functions
 */
void deal_with_function_args(void)
{
    while(this.type != ')')
        GetToken();

    GetToken();

    printf("function returning ");
}

/*
 * To deal with pointers
 */
void deal_with_pointers()
{
    while(stack[top].type == '*')
        printf("%s ", pop.string);
}

/*
 * To deal with the all tag
 * @param void
 * return void
 */
void deal_with_declarator(void)
{
    /* An array or function that may exist afte processing an identifier */
    switch(this.type) {
        case '[':
            deal_with_arrays();
            break;
        case '(':
            deal_with_function_args();
    }

    /* An pointer that may exist after processing an identifier */
    deal_with_pointers();

    while(top >= 0) {
        if(stack[top].type == '(') {
         /* The identifier in parentheses */
            pop;
            GetToken();
            deal_with_declarator();
        }
        else {
            printf("%s ", pop.string);
        }
    }
}


int main(int argc, char* argv[])
{
    read_to_first_identifier();

    deal_with_declarator();

    printf("\n");
    return EXIT_SUCCESS;
}
