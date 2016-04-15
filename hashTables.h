#ifndef __HASHTABLES_H_
#define __HASHTABLES_H_

typedef struct Cuvant
	{	char word[16];
		struct Cuvant *pNext;
	} WORD;
typedef WORD *pWORD;

typedef struct chained_table
{
		int buckets;
		int (*pHashFct)(char*);			/* vlad: hash function, for strings  (char*) */
		int (*pMatchFct)(const char*, const char*);
		void (*pDestroyList)(void*);	/* vlad: generic delete function */
		int size;		/* vlad: this size changes for every insert/delete */
		pWORD *table;	/* vlad: simple linked list, containing simple words */
} CHTbl;
typedef CHTbl *pCHTbl;	/* vlad: reference type for a hash-table structure */

/* vlad: functions' declarations */
int chtbl_init(	pCHTbl, int, 
				int (*pHashFct)(char*), 
				int (*pMatchFct)(const char*, const char*), 
				void (pDestroyList)(void*)
				);
int hashFct_string(char *);
int chtbl_insert(pCHTbl*, char*);	/* vlad: information to be added into the table */
void chtbl_delete(pCHTbl);			/* vlad: reference to the preceding node - see linked lists */
int chtbl_search(pCHTbl*, char**);	/* vlad: search for a string into a linked list */

/* vlad: two functions related to simple linked-lists */
void ins_node(pWORD*, pWORD, char *);
void del_node(pWORD*, pWORD);

#endif
