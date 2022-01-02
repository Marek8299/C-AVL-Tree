/************************************************************************
 * (c) 2021 Marek Klanica, DSA zadanie 2 - tester pre stromy / hash tabulku
 *
 * The above copyright notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "avl.h"

#define MAX_STR_LEN 1000

#define TEST_SIZE_DEFAULT 100000

#define TEST_REPEAT 5

typedef struct _TESTBLK {
	long long key;
	char value[MAX_STR_LEN];
	struct _TESTBLK *next;
} TESTBLK, *PTESTBLK;

unsigned int getRandom( unsigned int value_min, unsigned int value_max ) {
	return ( ( rand() % ( value_max + 1 - value_min ) ) + value_min );
}

long long getLongRand() {
	int len = getRandom(1, 18), part;
	long long rn = 0;
	for ( int i = 0; i < len; i++ ) {
		rn *= 10;
		rn += getRandom(0, 9);
	}
	return rn;
}

void rmnewline(char *line) {
	if( ! strtok(line, "\r\n")) strtok(line, "\n");
}

PTESTBLK newBlk(char *txt) {
	PTESTBLK blk = malloc(sizeof(TESTBLK));
	if( blk ) {
		blk->key = getLongRand();
		if(txt && strlen(txt) < MAX_STR_LEN) strcpy(blk->value, txt);
		else memset(blk->value, 0, MAX_STR_LEN);
		blk->next = NULL;
	}
	return blk;
}

PTESTBLK createTestSet(char *filename, unsigned long long *testcount) {
	bool file = false;
	if ( *filename ) {
		printf("Loading testset from file %s\n", filename);
		file = true;
	}
	else printf("Generating %d keys\n", TEST_SIZE_DEFAULT);
	
	char buffer[MAX_STR_LEN];
	*testcount = 0;
	FILE *fr;
	if (file) {
		fr = fopen(filename, "r");
		if (!fr) {
			printf("Could not open file %s\n", filename);
			return NULL;
		}
	}
	
	PTESTBLK testset = NULL, current = NULL, last = NULL;
	if ( file ) {
		while(fgets(buffer, MAX_STR_LEN, fr) != NULL) {
			rmnewline(buffer);
			last = current;
			current = newBlk(buffer);
			if( ! current ) break;
			if( ! testset ) testset = current;
			else last->next = current;
			
			(*testcount)++;
		}
	
		printf("Done, %lld item(s) found.\n", *testcount);
	
		fclose(fr);
	}
	else {
		for(unsigned long long i = 0; i < TEST_SIZE_DEFAULT; i++) {
			last = current;
			current = newBlk(NULL);
			if( ! current ) break;
			if( ! testset ) testset = current;
			else last->next = current;
			
			(*testcount)++;
		}
	
		printf("Done, %lld item(s) created.\n", *testcount);
	}
	
	return testset;
}

void destroyTestSet(PTESTBLK *first) {
	PTESTBLK last = NULL;
	while(*first != NULL) {
		last = *first;
		*first = last->next;
		free(last);
	}
}

void testReport(double *times, unsigned long long count) {
	if( ! count ) {
		printf("No data to report.");
		return;
	}
	
	double count_times = 0, min = DBL_MAX, max = 0;
	
	for(unsigned long long i = 0; i < count; i++) {
		count_times += times[i];
		if(times[i] < min) min = times[i];
		if(times[i] > max) max = times[i];
	}
	
	printf("min: %lf, max: %lf, avg: %lf, total: %lf\n", min, max, count_times / count, count_times);
	
}

void testReportExt(double *times, unsigned long long count, int percentage) {
	if( ! count ) {
		printf("No data to report.");
		return;
	}
	
	printf("1 item: \n");
	testReport(times, 1);
	printf("%d%% of items: \n", percentage);
	testReport(times, (count * percentage) / 100);
	printf("All items: \n");
	testReport(times, count);
	
}

void testTree(PTESTBLK testset, unsigned long long count) {
	if ( ! testset ) {
		printf("Testset is missing. Test failed.\n");
		return;
	}
	
	printf("TEST: AVL tree\n");
	
	bool nulltest = (*(testset->value)) ? false : true;
	
	PNODE tree = NULL;
	double *times = NULL;
	PTESTBLK currenttest = testset;
	unsigned long long num = 0;
	
	times = malloc(sizeof(double) * count);
	printf("Test size: %llu items\n", count);
	
	clock_t start, end;
	printf("Testing insert...\n");
	
	for( ; currenttest && num < count; currenttest = currenttest->next) {
		start = clock();
		
		AVL_insert(&tree, currenttest->key, currenttest->value);
		
		end = clock();
		
		if( times ) times[num] = (double)(end - start) / CLOCKS_PER_SEC;
		
		num++;
	}
	
	testReportExt(times, count, 25);
	
	printf("Testing search...\n");
	num = 0;
	
	for( currenttest = testset; currenttest && num < count; currenttest = currenttest->next) {
		start = clock();
		
		search(tree, currenttest->key);
		
		end = clock();
		
		if( times ) times[num] = (double)(end - start) / CLOCKS_PER_SEC;
		
		num++;
	}
	
	testReportExt(times, count, 5);
	
	
	
	free(times);
	tree_free(tree);
}

int main(int argc, char **argv) {
	char filename[255] = {0};
	
	if( argc > 1 && strlen(argv[1]) < 255) strcpy(filename, argv[1]);
	
	srand(time(0));
	
	unsigned long long count;
	
	PTESTBLK testset = createTestSet(filename, &count);
	
	
	for (int i = 0; i < TEST_REPEAT; i++) {
		printf("\n\nProgress: %d of %d\n\n", i+1, TEST_REPEAT);
		testTree(testset, count);
		/*if(count >= 1000) {
			//testTree(testset, &insert, &TESTCOMPAT_search, "Non-balanced tree", 1000);
			testTree(testset, 1000);
		}
		if(count >= 25000) {
			testTree(testset, 25000);
		}
		if(count >= 100000) {
			testTree(testset, 100000);
		}*/
	}
	
	destroyTestSet(&testset);
	
	
	return 0;
}
