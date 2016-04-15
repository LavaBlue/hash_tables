#include "hashTables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TBLSIZE
	#define TBLSIZE 257	/* vlad: prime number, preferable */
#endif

#define N 16
#define M N

int main() 
{
	pCHTbl pRef = NULL;
	int buckets = TBLSIZE;
	char myWords[M][N] = {	"today", "tomorrow", "always"};

	int (*pHashFct)(char*) = hashFct_string;	/* vlad: the hash function */
	int (*pMatchFct)(const char*, const char*) = strcmp;	/* vlad: the comparison function (for strings) */
	void (*pDestroyList)(void *) = free;			/* vlad: the de-allocation function */

	/* vlad: init the table */
	chtbl_init(	pRef, 
				buckets, 
				pHashFct, 
				pMatchFct, 
				pDestroyList
				);

	chtbl_insert(&pRef, myWords[0]);
	
 	fflush(stdin);
	return 0;
}
