#ifndef LLIST_H
#define LLIST_H

struct node {
    char * wpt;
    int wsize;
    struct node *next;
};

struct llist{
    struct node *head;
    int size;
};

struct llist *allocMBList;
struct llist *freedMBList;

//Delete node at index
int deleteNode(struct llist *llist, int nodeNum, bool freed);
//Add node to back of list
void pushBack(struct llist *llist, struct node *node);
//Outputs list to file
void printList(FILE *file, struct llist *llist, bool alloc);
//Combines node that are next to each other in memory in freedMBList
void combineNodes(struct llist* llist);

#endif