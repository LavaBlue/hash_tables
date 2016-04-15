#include "hashTables.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define TBLSIZE 257
#define N 16
#define M N

/* vlad: I really do need such a function here (unlike for linked lists). 
 * Several variables must be initialized within the structure.
 */
int chtbl_init(	pCHTbl pRef, 
				int buckets, 
				int (*pHashFct)(char*), 
				int (*pMatchFct)(const char*, const char*), 
				void (*pDestroyList)(void*)
				)
{
 

	pRef->buckets = buckets;		/* vlad: number of buckets (entries in the table) */
	pRef->pHashFct = pHashFct;
	pRef->pMatchFct = pMatchFct;
	pRef->pDestroyList = pDestroyList;	/* vlad: usually this is free() */

	pRef->size = 0;		/* vlad: at the very beginning the table has zero elements */
	
	(*pRef->table) = (pWORD) malloc(buckets * sizeof(WORD));
	assert (pRef->table != NULL);
	
	return 0;
}

/* vlad: use a string example (as for the lists) */
int hashFct_string(char *inputKey)
{
 	char *pStr;
	int val;

	val = 0;
	pStr = inputKey;

	/* vlad: compute the hash for the input string */
	do{
	   int tmp;
	   val = (val << 4) + *pStr;
	   if( (tmp = (val & 0xf0000000)) )	/* vlad: isolate uppermost nibble */
	   {
	   	   val = val ^ (tmp >> 24);
		   val = val ^ tmp;
	   }
	   pStr++;
	} while(*pStr != '\0');

	return val % TBLSIZE;
}


int chtbl_insert(pCHTbl *pRef, char *toInsert)
{
	int bucket;

 	/* vlad: the string to insert should not be in the table (in the list at some bucket) */
	if( !chtbl_search(pRef, &toInsert) )
		return 1;	/* vlad: failure: the element already exists */
	else
	{
		bucket = (*pRef)->pHashFct(toInsert) % ((*pRef)->buckets);
		ins_node((&(*pRef)->table[bucket]), NULL, toInsert);	/* vlad: use ins_nod() from simple linked lists; insert at beginning */
		(*pRef)->size++;

		return 0;	/* vlad: success */
	}
}

int chtbl_search(pCHTbl *pRef, char **searchFor)
{
 	pWORD *pRefPers = NULL;
	int bucket;

	bucket = (*pRef)->pHashFct(*searchFor) % ((*pRef)->buckets);
	for(pRefPers = &((*pRef)->table[bucket]); pRefPers != NULL; (*pRefPers) = (*pRefPers)->pNext)
		if( (*pRef)->pMatchFct(*searchFor, (*pRefPers)->word) )
		{
		 	*searchFor = (*pRefPers)->word;
			return 0;	/* vlad: found it! */
		}
 	return -1;	/* vlad: didn't found it... */
}

void chtbl_delete(pCHTbl pRef)
{
 	int i;

 	for(i = 0; i<pRef->buckets; i++)
		  del_node(&(pRef->table[i]), NULL);	/* vlad: delete the first node */

	free(pRef->table);
	memset(pRef, 0, sizeof(CHTbl));		/* vlad: although un-necessary, clear the former used memory */

 	return;
}

/* vlad: two simple linked-lists functions I need: ins_nod() and del_nod() */
void ins_node(pWORD *pPrim, pWORD p, char *word)
{
	pWORD q = (pWORD)malloc(sizeof(WORD));
	assert(q != NULL);

	strcpy(q->word, word);	/* vlad: update the information fields, once for any insert */
	if(p == NULL)			/* vlad: insert as the 1st node (prim changes mandatory) */
	{
		q->pNext = *pPrim;
		*pPrim = q;
	} else {
		q->pNext = p->pNext;	/* vlad: save the current link */
		p->pNext = q;			/* vlad: update the new link */
	}
	return;
}

void del_node(pWORD *pPrim, pWORD p)
{
	pWORD q;
	if(p == NULL)
	{
		q = *pPrim;	/* vlad: retain the address of the first node */
		*pPrim = (**pPrim).pNext;	/* vlad: 'pass-by' the first node */
		free(q);
	} else {
		q = p->pNext;
		p->pNext = q->pNext;	/* vlad: 'pass-by' the node q */
		free(q);
	}
	return;
}

