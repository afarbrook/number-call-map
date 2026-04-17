#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct call call;
typedef struct number number;
typedef struct queue queue;
typedef struct node node;

struct call{
    number *number;
    int count;
    struct call *next;
};

struct number{
    char *number;
    call *calls;
    int inQ;
    int level;
    struct number *next;
};

struct node{
    number *num;
    struct node *next;
};

struct queue{
    node *head;
    node *tail;
};
queue *q;
number *allNumbers;
int errOut;

/**
 * enque(nd) -- enqueues a number into the queue
 * 
 * @param nd is a number struct that gets added
 */
void enque(number *nd){
    node *new = malloc(sizeof(node));
    if(new == NULL){
        fprintf(stderr, "Out of memory.\n");
        errOut =1;
        return;
    }
    new->next = NULL;
    if(new == NULL){
        errOut =1;
        return;
    }new->num = nd;
    if(q->head == NULL){
        q->head = new;
        q->tail = new;
        return;
    }
    q->tail->next = new;
    q->tail = new;
}
/**
 * dequeue() -- dequeues the first item in the queue
 * 
 * @returns the number dequeued or NULL if the queue is empty
 */
number *dequeue(){
    if(q->head == NULL){
        return NULL;
    }
    node *curr = q->head;
    q->head = q->head->next;
    if(q->head == NULL){
        q->tail = NULL;
    }number *tmp = curr->num;
    free(curr);
    return tmp;
}

/**
 * freeQ() -- frees the queue from memory
 */
void freeQ(){
    node *curr = q->head;
    node *nxt;
    while(curr != NULL){
        nxt = curr->next;
        free(curr);
        curr = nxt;
    }
    free(q);
}

/**
 * isEmpty() -- checks if the queue is empty
 * @returns 1 if the queue is empty and 0 if it is not
 */
int isEmpty(){
    return q->head==NULL;
}

/**
 * addNumber(new) -- adds a number to the list of all numbers
 * @param new is the number struct to be added
 */
void addNumber(number *new){
    if(allNumbers == NULL){
        allNumbers = new;
        return;
    }
    number *curr = allNumbers;
    while(curr->next != NULL) curr = curr->next; //iterates to end of list
    curr->next = new;
}

/**
 * createNum(num) -- creates a number struct with the given number and adds it to the list of all numbers
 * @param num is the number to be added
 * @returns the number struct created
 */
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
    newNum->inQ = 0;
    if(newNum->number == NULL){
        fprintf(stderr, "Out of memory.\n");
        errOut = 1;
        return NULL;
    }addNumber(newNum);
    return newNum;
}

/**
 * get(num) -- gets the number struct with the given number
 * @param num is the number to be found
 * @returns the number struct with the given number or NULL if it is not found
 */
number *get(char *num){
    if(allNumbers == NULL){ //checks if list is empty
        return NULL;
    }
    number *curr = allNumbers;
    while(curr->next!= NULL){ //iterates through all but 1 num
        if(strcmp(num, curr->number)==0){ //checks num to see if found
            return curr;
        }
        curr = curr->next;
    }
    if(strcmp(num, curr->number)==0){ //checks last num
            return curr;
        }
    return NULL;
    
}

/**
 * addCall(source, new) -- adds a call from source to new
 * @param source is the number struct that made the call
 * @param new is the number struct that received the call
 */
void addCall(number *source, number *new){
    if(source->calls == NULL){ //number has no calls
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
        if(strcmp(curr->number->number, new->number)==0){ //checks if they have called before
            curr->count++;
            return;
        }curr=curr->next;
    }
    if(strcmp(curr->number->number, new->number)==0){ //checks last item
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
    curr->next = newCall; //adds newCall to list of calls

}

/**
 * countCalls(source, target) -- counts the calls between source and target
 * @param source is the source of the calls
 * @param target is the target of the calls
 * @retuns the amount of times they called
 */
int countCalls(number *source, number *target){
    call *curr = source->calls;
    while(curr != NULL){
        if(strcmp(curr->number->number, target->number)==0){ //checks if the numbers are the same
            return curr->count;
        }curr = curr->next;
    }return 0;
}

/**
 * addCalls(source, target) -- adds calls between both source and the target
 * 
 * Makes sure to add them to both sides since they call each other.
 * @param source is the source of the calls
 * @param target is the target of the calls
 */
void addCalls(char *source, char *target){
    number *num1 = get(source);
    number *num2 = get(target);
    if(num1 == NULL) num1 = createNum(source); //creates numbers if they are new
    if(num2 == NULL) num2 = createNum(target);
    addCall(num1, num2);
    addCall(num2, num1);
}

/**
 * resetQ() -- resets the nums in the queue to the unqueued state
 */
void resetQ(){
    number *curr = allNumbers;
    while(curr!=NULL){
        curr->inQ = 0;
        curr->level = 0;
        curr = curr->next;
    }
}

/**
 * isConnected(start, target) -- checks if two numbers are connected using BFS
 * 
 * Psuedo code taken from spec.
 * @param start is the number to start at
 * @param target is the number to find
 * @returns the level of the target if found or -1 if not found
 */
int isConnected(number *start, number *target){
    resetQ();
    while(!isEmpty()) dequeue();
    start->inQ = 1;
    start->level = 0;
    enque(start);

    while(!isEmpty()){
        number *tmp = dequeue();
        if(tmp == target){
            return tmp->level;
        }
        call *curr = tmp->calls;
        while(curr != NULL){
            number *child = curr->number;
            if(child->inQ == 0){
                child->inQ = 1;
                child->level = tmp->level+1;
                enque(child);
            }curr = curr->next;
        }


    }
    return -1;
}

/**
 * isValid(num) -- checks if the given number is in the correct format
 * @param num is the number to be checked
 * @returns 1 if the number is valid or 0 if not
 */
int isValid(char *num){
    if(strlen(num) != 12) return 0;
    for(int i = 0; i<12;i++){
        if(i ==3 || i == 7){
            if(num[i] != '-') return 0;
        }else{
            if(!isdigit(num[i])) return 0;
        }
    }return 1;
}

/**
 * proccessFile(fp) -- takes in a file and iterates through it adding all numbers and calls to
 * the data struct.
 * 
 * sets errOut to 1 for bad formatted data
 * @param fp is the file to go through
 */
void proccessFile(FILE *fp){
    char *line = NULL;
    size_t len = 0;
    while(getline(&line, &len, fp)!=-1){
        char tmp[2];
        if(sscanf(line, "%1s", tmp) == EOF)continue; //checks for blank line
        char *numOne = malloc(13);
        char *numTwo = malloc(13);
        int check = sscanf(line, "%12s %12s", numOne, numTwo);
        if(check !=2 || isValid(numOne) == 0 || isValid(numTwo) == 0){ //checks for bad format
            fprintf(stderr, "invalid format\n");
            errOut = 1;
            free(numOne);
            free(numTwo);
            continue;
        }
        if(strcmp(numOne, numTwo)==0){ //checks for same number
            fprintf(stderr, "Same number.\n");
            errOut = 1;
            free(numOne);
            free(numTwo);
            continue;
        }
        addCalls(numOne, numTwo);
        free(numOne);
        free(numTwo);
    }free(line);
}

/**
 * freeCalls(ptr) -- frees all of ptrs calls
 * 
 * @param ptr is a number that needs its calls freed
 */
void freeCalls(number *ptr){
    call *calls = ptr->calls;
    call *nxt;
    while(calls != NULL){
        nxt = calls->next;
        free(calls);
        calls = nxt;
    }
}
/**
 * freeNums() -- frees all the numbers from all numbers, and all of their calls
 * 
 * Calls on freeCalls to clear out the calls.
 */
void freeNums(){
    if(allNumbers == NULL) return;
    number *ptr1, *ptr2;
    ptr1 = allNumbers;
    while(ptr1 != NULL){
        ptr2 = ptr1->next;
        freeCalls(ptr1);
        free(ptr1->number);
        free(ptr1);
        ptr1=ptr2;
    }
}

int main(int argc, char *argv[]){ 
    FILE *fp;
    allNumbers=NULL;
    errOut = 0;
    q=malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    if(argc<2){ //makes sure there are files to go through
        fprintf(stderr, "Not enough arguments.\n");
        return 1;
    }argv++; //skips argv[0]
    while(*argv){
        fp = fopen(*argv, "r");
        if(fp == NULL){
            fprintf(stderr, "file not found: %s\n", *argv);
            errOut = 1;
            argv++;
            continue;
        }proccessFile(fp);
        fclose(fp);
        argv++;
    }
    char *line = NULL;
    size_t len = 0;
    while(getline(&line, &len, stdin) != EOF){ //gors through stdin
        char tmp[2];
        if(sscanf(line, "%1s", tmp) == EOF)continue; //checks for blank line
        char *numOne = malloc(13);
        char *numTwo = malloc(13);
        int check = sscanf(line, "%12s %12s", numOne, numTwo);
        if(check !=2 || isValid(numOne) == 0 || isValid(numTwo) == 0){ //checks for bad format
            fprintf(stderr, "invalid format in stderr\n");
            errOut = 1;
            free(numOne);
            free(numTwo);
            continue;
        }
        if(strcmp(numOne, numTwo)==0){ //checks for same number
            fprintf(stderr, "Same number: %s\n", numOne);
            errOut = 1;
            free(numOne);
            free(numTwo);
            continue;
        }
        number *num1 = get(numOne);
        number *num2 = get(numTwo);
        if((num1 == NULL) || (num2 == NULL)){
            fprintf(stderr, "One or both numbers not in graph: %s %s\n", numOne, numTwo);
            errOut = 1;
            free(numOne);
            free(numTwo);
            continue;
        }
        free(numOne);
        free(numTwo);
        int calls = countCalls(num1, num2);
        if(calls >0){
            printf("Talked %d times\n", calls);
        }else{
            int connected = isConnected(num1, num2);
            if(connected==-1){
                printf("Not connected\n");
            }else{
                printf("Connected through %d numbers\n", connected-1);
            }
        }
    }free(line);
    freeNums();
    freeQ();
    return errOut;
}
