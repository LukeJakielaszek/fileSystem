#include<stdio.h>
#include<stdlib.h>

typedef struct node{
  struct node * next;
  struct node * prev;
  char * command;
}node;

typedef struct llist1{
  struct node * head;
  struct node * tail;
  int size;
  struct llist1 *next;
  struct llist1 *prev;
}llist1;

typedef struct llist2{
  struct llist1 * head;
  struct llist1 * tail;
  int size;
}llist2;

// two dimensional list functions
llist2 * makeList2();
int isEmpty2(llist2 * list);
void printList2(llist2 * list2);
void append2(llist2 * list2, llist1 * list1);
llist1 * pop2(llist2 * list2);
llist1 * peek(llist2 * list2);

// one dimensional list functions 
llist1 * makeList1();
void append1(llist1 * list, char * command);
node * createNode(char * command);
void printList1(llist1 * list);
char * get1(llist1 * list, int index);
char * remove1(llist1 * list, int index);
int isEmpty1(llist1 * list);

// removes first 1D list
llist1 * pop2(llist2 * list2){
  llist1 * retList;
  // attempts to remove first 1D list
  if(list2->head == NULL){
    // prints error if 2D list is empty
    printf("ERROR: 2D list is empty, cannot pop\n");
    exit(-1);
  }else if(list2->size == 1){
    // if 2D list is of size one
    retList = list2->head;
    list2->head = NULL;
    list2->tail = NULL;
  }else{
    // if 2D list has multiple things in it
    // grabs 1D list
    retList = list2->head;

    // detaches 1D list from 2D list
    list2->head = list2->head->next;
    list2->head->prev = NULL;
  }

  // decrements 2Dlist size
  list2->size--;

  // cleanup for security
  retList->next = NULL;
  retList->prev = NULL;

  // returns 1Dlist
  return retList;
}

// returns pointer to first 1D list
llist1 * peek(llist2 * list2){
  // attempts to view first 1D list
  if(list2->head == NULL){
    // returns null if 2d list is empty
    return NULL;
  }else{
    // returns 1D list pointer if not empty
    return list2->head;
  }
}

// appends a 1D list to the end of a 2D list
void append2(llist2 * list2, llist1 * list1){
    // attaches 1Dlist to 2D list to list
    if(list2->head == NULL){
      // if 2Dlist is empty sets head & tail to 1Dlist
      list2->head = list1;
      list2->tail = list2->head;
      list2->size++;
    }else{
      // if not empty, sets tail to temp
      list1->prev = list2->tail;
      list2->tail->next = list1;
      list2->tail = list1;
      list2->size++;
    }
}

// prints a 2D list to std_out
void printList2(llist2 * list2){
  printf("\nPrinting 2D list:\n");

  // prints list
  if(isEmpty2(list2)){
    // if empty, prints empty
    printf("2D llist is Empty\n");
  }else{
    // initialize temp to first list index
    llist1 * temp = list2->head;

    // loop through 2D list, printing each 1D list
    int i = 0;
    for(i = 0; i < list2->size; i++){
      printf("list %d:\n", i);
      printList1(temp);
      printf("\n");
      temp=temp->next;
    }

    // print size of 2D list
    printf("list2D size : [%d]\n", list2->size);
  }
}

// returns 1 if 2D list is empty, 0 otherwise
int isEmpty2(llist2 * list){
  return list->head == NULL;
}

// makes a two dimensional linked list
llist2 * makeList2(){
  llist2 * list;

  // allocate memory for linked list
  list = (llist2*)malloc(sizeof(llist2));

  // checks for success
  if(list == NULL){
    printf("ERROR: Linked list 2 failed to malloc.\n");
    exit(-1);
  }

  // initializes to NULL/0
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;

  // returns 2D list pointer
  return list;
}

// returns 1 if 1D list is empty, 0 otherwise
int isEmpty1(llist1 * list){
  return list->head == NULL;
}

char * remove1(llist1 * list, int index){
  // check if index is out of bounds
  if(index < 0 || index >= list->size){
    printf("ERROR: Index out of single dimensional list range.\n");
    exit(-1);
  }

  // stores return node 
  node * retnode;

  // remove node
  if(list->size == 1){
    //list of size 1
    // grab retnode
    retnode = list->head;
    
    // detach node from list of size one and set everything to 0/NULL
    list->head = NULL;
    list->tail = NULL;
  }else if(index == 0){
    // if removing the first thing from list of size > 1
    // grab retnode
    retnode = list->head;

    // detach node from list
    list->head = list->head->next;
    list->head->prev = NULL;
  }else if (index == list->size-1){
    // if removing the last node from list of size > 1
    retnode = list->tail;

    // detach node from list
    list->tail = list->tail->prev;
    list->tail->next = NULL;    
  }else{    
    // remove node anywhere else within list of size > 1
    // grab the head node
    retnode = list->head;

    int i = 0;

    // find the desired node
    for(i = 0; i < index; i++){
      retnode = retnode->next;
    }

    // detach node
    retnode->prev->next = retnode->next;
    retnode->next->prev = retnode->prev;
  }

  
  // decrement size
  list->size--;

  // grab desired command
  char * retchar = retnode->command;

  // free removed node
  free(retnode);

  // return command
  return retchar;
}

// returns the value at the specified index for a 1 dimensional list
char * get1(llist1 * list, int index){
  if(index < 0 || index >= list->size){
    printf("ERROR: Index out of single dimensional list range.\n");
    exit(-1);
  }

  node * retval = list->head;

  int i = 0;
  for(i = 0; i < index; i++){
    retval = retval->next;
  }

  return retval->command;
}

// prints a single dimensional linked list
void printList1(llist1 * list){

  int i = 0;
  node * temp = list->head;

  for(i = 0; i < list->size; i++){
    printf("\t%s\n", temp->command);
    temp = temp->next;
  }

  printf("\tlist1D size : [%d]\n", list->size);
}

// makes a single dimensional linked list
llist1 * makeList1(){
  llist1 * list;

  // allocate memory for linked list
  list = (llist1*)malloc(sizeof(llist1));

  if(list == NULL){
    printf("ERROR: Linked list 1 failed to malloc.\n");
    exit(-1);
  }

  list->head = NULL;
  list->tail = NULL;
  list->next = NULL;
  list->prev = NULL;
  list->size = 0;
}

// appends to end of a single dimensional linked list
void append1(llist1 * list, char * command){
  node * temp;

  // creates temporary node
  temp = createNode(command);
  
  // attaches node to list
  if(list->head == NULL){
    // if list is empty sets head & tail to temp
    list->head = temp;
    list->tail = list->head;
    list->size++;
  }else{
    // if not empty, sets tail to temp
    temp->prev = list->tail;
    list->tail->next = temp;
    list->tail = temp;
    list->size++;
  }
}

// creates a node with next set to null and command set to a string
node * createNode(char * command){
  node * temp;

  // malloc a new node
  temp = (node*)malloc(sizeof(node));

  // check if failed
  if(temp == NULL){
    printf("ERROR: Node failed to malloc.\n");
    exit(-1);
  }

  // initialize node values
  temp->next = NULL;
  temp->prev = NULL;
  temp->command = command;

  // return node
  return temp;
}
