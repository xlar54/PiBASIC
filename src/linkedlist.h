#ifndef _LINKEDLIST_
#define _LINKEDLIST_

extern "C"
{
	struct node {
		int linenum;
		unsigned char* data;
		struct node *next;
	};

	struct node* ll_gethead();
	void ll_insertFirst(int linenum, unsigned char* data);
	struct node* ll_deleteFirst();
	bool ll_isEmpty();
	int ll_length();
	struct node* ll_find(int linenum);
	struct node* ll_delete(int linenum);
	void ll_sort();
	void ll_reverse(struct node** head_ref);
}

#endif