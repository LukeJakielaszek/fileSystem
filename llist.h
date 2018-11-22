#ifndef LLIST_H
#define LLIST_H

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

#endif /** LLIST_H */
