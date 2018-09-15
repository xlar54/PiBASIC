#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"

extern "C"
{

static struct node *ll_head = NULL;
static struct node *ll_current = NULL;

struct node* ll_gethead()
{
	return ll_head;
}

//insert link at the first location
void ll_insertFirst(int linenum, unsigned char* data)
{
	//create a link
	struct node *link = (struct node*) malloc(sizeof(struct node));

	link->linenum = linenum;
	link->data = data;

	//point it to old first node
	link->next = ll_head;

	//point first to new first node
	ll_head = link;
}

//delete first item
struct node* ll_deleteFirst()
{
	//save reference to first link
	struct node *tempLink = ll_head;

	//mark next to first link as first 
	free(ll_head->data);
	ll_head = ll_head->next;

	//return the deleted link
	return tempLink;
}

//is list empty
bool ll_isEmpty()
{
	return ll_head == NULL;
}

int ll_length()
{
	int length = 0;
	struct node *ll_current;

	for (ll_current = ll_head; ll_current != NULL; ll_current = ll_current->next) {
		length++;
	}

	return length;
}

//find a link with given linenum
struct node* ll_find(int linenum)
{

	//start from the first link
	struct node* ll_current = ll_head;

	//if list is empty
	if (ll_head == NULL) {
		return NULL;
	}

	//navigate through list
	while (ll_current->linenum != linenum) {

		//if it is last node
		if (ll_current->next == NULL) {
			return NULL;
		}
		else {
			//go to next link
			ll_current = ll_current->next;
		}
	}

	//if data found, return the current Link
	return ll_current;
}

//delete a link with given linenum
struct node* ll_delete(int linenum)
{

	//start from the first link
	struct node* ll_current = ll_head;
	struct node* ll_previous = NULL;

	//if list is empty
	if (ll_head == NULL) {
		return NULL;
	}

	//navigate through list
	while (ll_current->linenum != linenum) {

		//if it is last node
		if (ll_current->next == NULL) {
			return NULL;
		}
		else {
			//store reference to current link
			ll_previous = ll_current;
			//move to next link
			ll_current = ll_current->next;
		}
	}

	//found a match, update the link
	if (ll_current == ll_head) {
		//change first to point to next link
		free(ll_head->data);
		ll_head = ll_head->next;
	}
	else {
		//bypass the current link
		ll_previous->next = ll_current->next;
	}

	return ll_current;
}

void ll_sort()
{

	int i, j, k, templinenum;
	unsigned char *tempData;
	struct node *ll_current;
	struct node *ll_next;

	int size = ll_length();
	k = size;

	for (i = 0; i < size - 1; i++, k--) {
		ll_current = ll_head;
		ll_next = ll_head->next;

		for (j = 1; j < k; j++) {

			if (ll_current->linenum > ll_next->linenum)
			{
				tempData = ll_current->data;
				ll_current->data = ll_next->data;
				ll_next->data = tempData;

				templinenum = ll_current->linenum;
				ll_current->linenum = ll_next->linenum;
				ll_next->linenum = templinenum;
			}

			ll_current = ll_current->next;
			ll_next = ll_next->next;
		}
	}
}

void ll_reverse(struct node** ll_head_ref)
{
	struct node* ll_prev = NULL;
	struct node* ll_current = *ll_head_ref;
	struct node* ll_next;

	while (ll_current != NULL) {
		ll_next = ll_current->next;
		ll_current->next = ll_prev;
		ll_prev = ll_current;
		ll_current = ll_next;
	}

	*ll_head_ref = ll_prev;
}
}