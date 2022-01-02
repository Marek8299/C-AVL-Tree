/************************************************************************
 * (c) 2021 Marek Klanica, DSA zadanie 2 - AVL strom
 *
 * The above copyright notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
 ***********************************************************************/

#ifndef AVL_H
#define AVL_H

#include <stdbool.h>

typedef struct _NODE {
	unsigned long long key;
	char *value;
	struct _NODE *lchild;
	struct _NODE *rchild;
	unsigned long long height;
} NODE, *PNODE;

extern bool AVL_insert(PNODE *tree, unsigned long long key, char *value);

extern char * search(PNODE tree, unsigned long long key);

void tree_free(PNODE tree);

#endif // AVL_H
