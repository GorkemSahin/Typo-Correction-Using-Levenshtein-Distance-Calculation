#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LENGTH 30
#define MAX_DISTANCE 5

int countWords (FILE*);
int calculateArrayLength (int);
void fixCapitalLetters (char*);
int hash (char**, char*, int);
void readDictionary (FILE*, char**, int);
void printHashedDictionary (FILE*, char**, int);
void readHashedDictionary (FILE*, char**, int);
void displayHashedDictionary (char**, int);
void readTest (FILE*, char**);
void checkTestWords (FILE*, char**, char**, int, int, int);
int calculateDistance (char*, int, char*, int);

int main(int argc, char *argv[]) {
	FILE* dictionaryFP;
	FILE* hashedDictionaryFP;
	FILE* testFP;
	FILE* testOutFP;
	int i, noOfWords, arrayLength, choice, noOfTestWords;
	char** dictionary;
	char** testWords;
	
	printf("Please enter 0 if you have already generated the hashedDicionary file.\n");
	printf("If not, enter any other number.\n");
	scanf("%d", &choice);
	if (choice != 0){
		dictionaryFP = fopen("dictionary.txt", "r");
		hashedDictionaryFP = fopen("hashedDictionary.txt", "w");
		if (dictionaryFP == NULL || hashedDictionaryFP == NULL){
			printf("ERROR: dictionary.txt could not be found.\n");
			return 0;
		}
		printf("\n");
		noOfWords = countWords(dictionaryFP);
		arrayLength = calculateArrayLength(noOfWords);
		dictionary = (char**) malloc (arrayLength * (sizeof(char*)));
		for (i=0;i<arrayLength;i++){
			dictionary[i] = NULL;
		}
		printf("Hashing the words from dictionary.txt...\n\n");
		readDictionary(dictionaryFP, dictionary, arrayLength);
		printf("Hashed words are being saved to the hashedDicionary.txt file...\n");
		printHashedDictionary(hashedDictionaryFP, dictionary, arrayLength);
		
		fclose(hashedDictionaryFP);
		fclose(dictionaryFP);
		for (i=0;i<arrayLength;i++){
			free(dictionary[i]);
		}
		free(dictionary);
		printf("hashedDictionary.txt successfully generated.\n");
	}
	
	hashedDictionaryFP = fopen("hashedDictionary.txt", "r");
	if (hashedDictionaryFP == NULL){
		printf("ERROR: hashedDictionary.txt not found.\n");
		return 0;
	}
	printf("\n Reading words and indexes from hashedDictionary.txt...\n");
	fscanf(hashedDictionaryFP, "%d\n", &arrayLength);
	printf("Array length: %d.\n", arrayLength);
	dictionary = (char**) malloc (arrayLength * (sizeof(char*)));
	readHashedDictionary(hashedDictionaryFP, dictionary, arrayLength);
	printf("hashedDictionary.txt successfully read.\n");
	fclose(hashedDictionaryFP);
	printf("\nReading words to be tested from test.txt...\n");
	testFP = fopen("test.txt", "r");
	testOutFP = fopen("testOut.txt", "w");
	if (testFP == NULL || testOutFP == NULL){
		printf("ERROR: test.txt not found.\n");
	}
	noOfTestWords = countWords(testFP);
	testWords = (char**) malloc (noOfTestWords * sizeof(char*));
	for (i=0;i<noOfTestWords;i++){
		testWords[i] = (char*) malloc (WORD_LENGTH * sizeof(char));
	}
	readTest(testFP, testWords);
	printf("test.txt successfully read.\n\n");
	checkTestWords(testOutFP, dictionary, testWords, noOfWords, arrayLength, noOfTestWords);
	
	return 0;
}



int countWords (FILE* fp){
	char word[WORD_LENGTH];
	int i = 0;
	while (fscanf(fp, "%s\n", word) == 1){
		i++;
	}
	printf("Word count: %d.\n", i);
	rewind(fp);
	return i;
}

int calculateArrayLength (int noOfWords){
	int i;
	int ok = 0;
	int arrayLength = 2 * noOfWords;
	printf("Number of words multiplied by two: %d.\n", arrayLength);
	while (ok == 0){
		i = 2;
		while (i < arrayLength && (arrayLength % i != 0)){
			i++;
		}
		if (i == arrayLength){
			ok = 1;
		} else {
			arrayLength++;
		}
	}
	printf("Array length: %d.\n\n", arrayLength);
	return arrayLength;
}

void readDictionary (FILE* dictionaryFP, char** dictionary, int arrayLength){
	char word[WORD_LENGTH];
	long index;
	while (fscanf(dictionaryFP, "%s", word) == 1){
		fixCapitalLetters(word);
		index = hash (dictionary, word, arrayLength);
		dictionary[index] = (char*) malloc (WORD_LENGTH * sizeof(char));
		strcpy(dictionary[index], word);
	}
}

void fixCapitalLetters (char* word){
	int i = 0;
	while (word[i] != '\0'){
		if (word[i] >= 'A' && word[i] <= 'Z'){
			word[i] += 32;
		}
		i++;
	}
}

int hash (char** dictionary, char* word, int arrayLength){
	int index, i;
	for (i=index=0; i < strlen(word); i++){
		index += (word[i])*26*i ;
	}
	index = index % arrayLength;
	while (dictionary[index] != NULL){
		index = (++index) % arrayLength;
	}
	return index;
}

void printHashedDictionary (FILE* hashedDictionaryFP, char** dictionary, int arrayLength){
	int i;
	fprintf(hashedDictionaryFP, "%d\n", arrayLength);
	for (i=0;i<arrayLength;i++){
		if (dictionary[i] != NULL){
			fprintf(hashedDictionaryFP, "%d: %s\n", i, dictionary[i]);
		}
	}
}
	
void readHashedDictionary (FILE* hashedDictionaryFP, char** dictionary, int arrayLength){
	char word[WORD_LENGTH];
	int index;
	while (fscanf(hashedDictionaryFP, "%d: %s\n", &index, word) > 0){
		dictionary[index] = (char*) malloc (WORD_LENGTH * sizeof(char));
		strcpy(dictionary[index], word);
	}
}

void displayHashedDictionary (char** dictionary, int arrayLength){
	int i;
	printf("\nSozluk:\n");
	for (i=0;i<arrayLength;i++){
		if (dictionary[i] != NULL){
			printf("%d: %s\n",i, dictionary[i]);
		}
	}
}

void readTest (FILE* testFP, char** testWords){
	char word[WORD_LENGTH];
	int i = 0;
	while (fscanf(testFP, "%s", word) == 1){
		fixCapitalLetters(word);
		strcpy(testWords[i], word);
		i++;
	}
}

void checkTestWords (FILE* testOutFP, char** dictionary, char** testWords, int noOfWords, int arrayLength, int noOfTestWords){
	int i, j, index, distance, minDistance;
	int noOfTries = 0;
	for (j=0;j<noOfTestWords;j++){
		for (i=index=0; i < strlen(testWords[j]); i++){
			index += (testWords[j][i])*26*i ;
		}
		index = index % arrayLength;
		while (dictionary[index] != NULL && (strcmp(testWords[j], dictionary[index]) != 0) && noOfTries < noOfWords){
			index = (++index) % arrayLength;
		}
		if (dictionary[index] != NULL && noOfTries < noOfWords){
			printf("The word %s matched the word %s at index: %d.\n\n", testWords[j], dictionary[index], index);
			fprintf(testOutFP, "%s -> OK\n", testWords[j]);
		} else {
			printf("Word %s  could not be found.\n", testWords[j]);
			minDistance = 999;
			for (i=0;i<arrayLength;i++){
				if (dictionary[i] != NULL){
					distance = calculateDistance (testWords[j], strlen(testWords[j]), dictionary[i], strlen(dictionary[i]));
					if (distance <= minDistance){
						minDistance = distance;
						index = i;
					}
				}
			}
			if (minDistance < MAX_DISTANCE){
				printf("Closest word to %s was the word: %s. Distance: %d.\n\n", testWords[j], dictionary[index], minDistance);
				fprintf(testOutFP, "%s -> %d -> %s\n", testWords[j], minDistance, dictionary[index]);
			} else {
				printf("No words close to %s has been found.\n\n", testWords[j]);
				fprintf(testOutFP, "%s -> NONE\n", testWords[j]);
			}
		}
	}
}

int calculateDistance (char* word1, int length1, char* word2, int length2){
	int matrix[length1 + 1][length2 + 1];
	int i;
	for (i = 0; i <= length1; i++) {
		matrix[i][0] = i;
	}
	for (i = 0; i <= length2; i++) {
		matrix[0][i] = i;
	}
	for (i = 1; i <= length1; i++) {
		int j;
		char c1;

		c1 = word1[i-1];
		for (j = 1; j <= length2; j++) {
			char c2;

			c2 = word2[j-1];
			if (c1 == c2) {
				matrix[i][j] = matrix[i-1][j-1];
			}
			else {
				int del;
				int insert;
				int substitute;
				int minimum;

				del = matrix[i-1][j] + 1;
				insert = matrix[i][j-1] + 1;
				substitute = matrix[i-1][j-1] + 2;
				minimum = del;
				if (insert < minimum) {
					minimum = insert;
				}
				if (substitute < minimum) {
					minimum = substitute;
				}
				matrix[i][j] = minimum;
			}
		}
	}
	return matrix[length1][length2];
}


