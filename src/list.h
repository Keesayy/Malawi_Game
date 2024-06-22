#ifndef _LIST_H_
#define _LIST_H_

typedef struct list {
	int value;
	struct list * next;
} list;

list *listCreate();
int listIsEmpty(list * s);
list *listAdd(list * s, int v);
int listTop(list * s);
list *listRemove(list * s);
int listSize(list * s);
void listDisplay(list * s);
void listFree(list * s);
list *listCopy(list *head);

list *listCopy(list *s);
int factorial(int x);
int comb(int n, int k);
void generate_combinations(list *indices, int n, list *current, list **result, int *index);
list **listPartition(list *indices, int n);

#endif
