#include<stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct call call;
typedef struct number number;
typedef struct node node;

struct call{
    number *number;
    int count;
    struct call *next;
};

struct number{
    char *number;
    call *calls;
    struct number *next;
};

struct node{
    number *name;
    struct node *next;
    struct node *prev;

};
node *head;

void enque(node *nd){
    if(head == NULL){
        head = nd;
        return;
    }node *curr = head;
    while(curr->next!=NULL){
        curr=curr->next;
    }curr->next=nd;
    nd->prev=curr;
}

node *dequeue(){
    if(head == NULL){
        return NULL;
    }node *curr = head;
    while(curr->next!=NULL){
        curr=curr->next;
    }curr->prev->next = NULL;
    return curr;
}

number *allNumbers;
int errOut;

void addNumber(number *new){
    if(allNumbers == NULL){
        allNumbers = new;
        return;
    }
    number *curr = allNumbers;
    while(curr->next != NULL) curr = curr->next;
    curr->next = new;
}

number *createNum(char *num){
    number *newNum = malloc(sizeof(number));
    if(newNum == NULL){
        fprintf(stderr, "Out of memory.\n");
        errOut = 1;
        return NULL;
    }
    newNum->calls = NULL;
    newNum->next = NULL;
    newNum->number = strdup(num);
    if(newNum->number == NULL){
        fprintf(stderr, "Out of memory.\n");
        errOut = 1;
        return NULL;
    }return newNum;
}

number *get(char *num){
    if(allNumbers == NULL){ //checks if list is empty
        number *new = createNum(num);
        addNumber(new);
        return new;
    }
    number *curr = allNumbers;
    while(curr->next!= NULL){ //iterates through all but 1 num
        if(strcmp(num, curr->number)==0){
            return curr;
        }
        curr = curr->next;
    }
    if(strcmp(num, curr->number)==0){ //checks last num
            return curr;
        }
    number *new = createNum(num);
    addNumber(new);
    return new;
    
}

void addCall(number *source, number *new){
    if(source->calls == NULL){
        call *newCall = malloc(sizeof(call));
        if(newCall == NULL){
            fprintf(stderr, "Out of memory.\n");
            errOut = 1;
            return;
        }
        newCall->number = new;
        newCall->next = NULL;
        newCall->count = 1;
        source->calls = newCall;
        return;
    }
    call *curr = source->calls;
    while(curr->next != NULL){
        if(strcmp(curr->number->number, new->number)==0){
            curr->count++;
            return;
        }curr=curr->next;
    }
    if(strcmp(curr->number->number, new->number)==0){
            curr->count++;
            return;
    }
    call *newCall = malloc(sizeof(call));
    if(newCall == NULL){
        fprintf(stderr, "Out of memory.\n");
        errOut = 1;
        return;
    }
    newCall->number = new;
    newCall->next = NULL;
    newCall->count = 1;
    curr->next = newCall;

}

void addCalls(char *source, char *end){
    number *num1 = get(source);
    number *num2 = get(end);
    if((num1 == NULL) || (num2 == NULL)){
        fprintf(stderr, "Out of memory.\n");
        errOut = 1;
        return;
    }
    addCall(num1, num2);
    addCall(num2, num1);
}

int isConnected(char *num1, char *num2){
    //To DO
}

void proccessFile(FILE *fp){
    char *line = NULL;
    size_t len = 0;
    while(getline(&line, &len, fp)!=-1){
        char *numOne = malloc(13);
        char *numTwo = malloc(13);
        int check = sscanf(line, "%12s %12s", numOne, numTwo);
        if(check !=2){
            errOut = 1;
            return;
        }
        addCalls(numOne, numTwo);
    }free(line);
}

int main(int argc, char *argv[]){ 
    FILE *fp;
    allNumbers=NULL;
    errOut = 0;
    if(argc<2){
        fprintf(stderr, "Not enough arguments.\n");
        return 1;
    }argv++;
    while(*argv){
        fp = fopen(*argv, "r");
        if(fp == NULL){
            fprintf(stderr, "file not found: %s\n", *argv);
            errOut = 1;
            argv++;
            continue;
        }proccessFile(fp);
        argv++;
    }
    
    char *line = NULL;
    size_t len = 0;
    while(getline(&line, &len, stdin) != EOF){
        char *numOne = malloc(13);
        char *numTwo = malloc(13);
        int check = sscanf(line, "%12s %12s", numOne, numTwo);
        if(check !=2){
            fprintf(stderr, "invalid format\n");
            return 1;
        }
        


    }
    return errOut;
}