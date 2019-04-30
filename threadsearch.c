// Multithreaded Text Pattern Search Algorithm
// Ryan Krawczyk (CSCI227103)

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define THRDS 4
#define MAX 1000000

char *pattern;
char text[MAX];
int count = 0;
int spot = -1;
pthread_t ids[THRDS];

// Scans user input by character and populates text array
void readInput() {
	int rs = scanf("%c", &text[count]);
	while (rs != -1) {
		count++;
		if (count >= MAX) break;
		rs = scanf("%c", &text[count]);
	}
}

// Determines matching character sequences
int match(char *s1, char *s2) {
	int i, flag = 1;
	for (i = 0; i < strlen(pattern); i++) {
		if (s1[i] != s2[i]) flag = 0;
	}
	return flag;
}

// Iterates through respective segment of text array for pattern
void *searchText(void *start) {
	int i, *st = (int *) start;
	for (i = *st; i < *st + (count/THRDS + 1); i++) {
		if (match(pattern, &text[i])) spot = i;
	}
}

// Partitions text array into segments and assigns each to a linear search thread
void genThreads() {
	int i, start[THRDS];
	for (i = 0; i < THRDS; i++) {
		start[i] = (count/THRDS) * i;
		pthread_create(&ids[i], NULL, searchText, &start[i]);
	}
}

// Terminates threads when entire text array has been visited
void joinThreads() {
	int i;
	for (i = 0; i < THRDS; i++) {
		pthread_join(ids[i], NULL);
	}
}

int main(int argc, char *argv[]) {
	pattern = argv[1];
	readInput();
	genThreads();
	joinThreads();
	if (spot == -1)		printf("\nPattern not found\n\n");
	else 				printf("\nPattern found at character %d\n\n", spot);
	return 0;
}

