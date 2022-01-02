/************************************************************************
 * (c) 2021 Marek Klanica, DSA zadanie 2 - AVL
 *
 * The above copyright notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
 ***********************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"


void rotate_l(PNODE *tree);

PNODE getNode(unsigned long long key, char *data) {
	PNODE node = malloc(sizeof(NODE));
	if ( node ) {
		node->key = key;
		if( data ) {
			node->value = strdup(data);
		}
		else node->value = NULL;
		node->lchild = NULL;
		node->rchild = NULL;
		node->height = 1;
	}
	return node;
}

void update(char **tgt, char *new_data) {
	unsigned long long new_len = strlen(new_data);
	if( strlen(*tgt) < new_len ) {
		*tgt = realloc(*tgt, (sizeof(char) * new_len) + 1);
	}
	if ( *tgt ) stpcpy(*tgt, new_data);
}

unsigned long long getMaxHeight(PNODE a, PNODE b) {
	if ( !a && !b) return 0;
	if ( !a && b ) return b->height;
	if ( a && !b ) return a->height;
	return (a->height > b->height) ? a->height : b->height;
}

void rotate_r(PNODE *tree) {
	PNODE root_old = *tree, root_new, backup_node;
	if( ! root_old->lchild->lchild && root_old->lchild->rchild) {
		rotate_l(&((*tree)->lchild));
	}
	root_new = root_old->lchild;
	backup_node = root_new->rchild;
	*tree = root_new;
	root_new->rchild = root_old;
	root_old->lchild = backup_node;
	root_old->height = getMaxHeight(root_old->lchild, root_old->rchild) + 1;
	root_new->height = getMaxHeight(root_new->lchild, root_new->rchild) + 1;
}

void rotate_l(PNODE *tree) {
	PNODE root_old = *tree, root_new = root_old->rchild, backup_node = root_new->lchild;
	if( ! root_old->rchild->rchild && root_old->rchild->lchild ) {
		rotate_r(&((*tree)->rchild));
	}
	root_new = root_old->rchild;
	backup_node = root_new->lchild;
	*tree = root_new;
	root_new->lchild = root_old;
	root_old->rchild = backup_node;
	root_old->height = getMaxHeight(root_old->lchild, root_old->rchild) + 1;
	root_new->height = getMaxHeight(root_new->lchild, root_new->rchild) + 1;
}

long long getBalanceFactor(PNODE a, PNODE b) {
	long long st = (a) ? a->height : 0;
	long long nd = (b) ? b->height : 0;
	return st - nd;
}

void balance(PNODE *tree) {
	if ( *tree == NULL ) return;
	long long diff = getBalanceFactor((*tree)->lchild, (*tree)->rchild);
	if ( llabs(diff) > 1 ) {
		if (diff > 0) {
			rotate_r(tree);
		}
		else {
			rotate_l(tree);
		}
	}
}

bool AVL_insert(PNODE *tree, unsigned long long key, char *value) {
	bool result = false;
	if ( ! *tree ) {
		*tree = getNode(key, value); // height sa nastavi vo funkcii
		result = true;
	}
	else {
		long long diff = key - (*tree)->key;
		if(diff) {
			if(diff < 0) {
				result = AVL_insert(&((*tree)->lchild), key, value);
			}
			else {
				result = AVL_insert(&((*tree)->rchild), key, value);
			}
			if(result) (*tree)->height = getMaxHeight((*tree)->lchild, (*tree)->rchild) + 1;
		}
		else update(&((*tree)->value), value);
	}
	balance(tree);
	//printf("%lld\n", (*tree)->height);
	return result;
}

char * search(PNODE tree, unsigned long long key) {
	PNODE match = NULL;
	if( tree && key ) {
		long long diff = key - tree->key;
		if( diff ) {
			if(diff < 0) return search(tree->lchild, key);
			else return search(tree->rchild, key);
		}
		else return tree->value;
	}
	return NULL;
}

void tree_free(PNODE tree) {
	if( tree ) {
		tree_free(tree->lchild);
		tree_free(tree->rchild);
		free(tree->value);
		free(tree);
	}
}
