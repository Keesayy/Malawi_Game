#include <stdlib.h>
#include <stdio.h>
#include "list.h"

list *listCreate() {
	list *s = NULL;
	return s;
}

int listIsEmpty(list *s) {
	return (s == NULL);
}

list *listAdd(list *s, int v) {
	list *tete = (list*)malloc(sizeof(list));
	tete->value = v;
	tete->next = s;
	return tete;
}

int listTop(list *s) {
	return s->value;
}

list *listRemove(list *s) {
	list *copie = s;
	copie = s->next;
	free(s);
	return copie;
}

int listSize(list *s) {
	int count = 0;
	while(s != NULL){
		count++;
		s = s->next;
	}
	return count;
}

void listDisplay(list *s) {
	if(s == NULL) printf("NULL\n");
	while(s != NULL){
		printf("%d ", s->value);
		s = s->next;
	}
	printf("\n");
}

void listFree(list *s) {
	list *copie = NULL;
	while(s != NULL){
		copie = s;
		s = s->next;
		free(copie);
	}
}

list *listCopy(list *s) {
    if (!s) return NULL;
    list *new_list = (list *)malloc(sizeof(list));
    new_list->value = s->value;
    new_list->next = listCopy(s->next);
    return new_list;
}

int factorial(int x) {
    if (x == 0) return 1;
    int result = 1;
    for (int i = 1; i <= x; ++i) 
        result *= i;

    return result;
}

int comb(int n, int k) {
    if (k > n) return 0;
    return factorial(n) / (factorial(k) * factorial(n - k));
}

void generate_combinations(list *indices, int n, list *current, list **result, int *index) {
    if (n == 0) {
        result[*index] = listCopy(current);
        (*index)++;
        return;
    }
    if (!indices) return;
    list *included = listAdd(current, indices->value);

    generate_combinations(indices->next, n - 1, included, result, index);
    generate_combinations(indices->next, n, current, result, index);
    free(included);
}

list **listPartition(list *indices, int n) {
    int len = listSize(indices);
    int comb_count = comb(len, n);
    int index = 0;

    list **result = (list**)malloc(comb_count * sizeof(list*));
    generate_combinations(indices, n, NULL, result, &index);

    return result;
}

