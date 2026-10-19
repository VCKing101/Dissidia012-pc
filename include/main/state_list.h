#ifndef MAIN_STATE_LIST_H
#define MAIN_STATE_LIST_H

/* First two words are used by the observed singly linked state list. */
typedef struct MainStateListNode {
    unsigned int field_0000;
    struct MainStateListNode *next;
} MainStateListNode;
typedef struct MainStateListPrefix {
    MainStateListNode *head;
    MainStateListNode *tail;
} MainStateListPrefix;
extern MainStateListPrefix *D_089D5EB0;

#endif
