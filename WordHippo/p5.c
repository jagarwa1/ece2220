// Word Hippo
// Jai Agarwal
// ECE 2220 Prog 5
// Purpose          - to gain experience with dynamic memory allocation and string functions
// Assumptions      - assumes the user enters advanced searches in sequential order
// Known Bugs       - can only do Contain Letters and one additional search, cannot continue to select multiple searches
//                  - formatting is off if word list is small

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// searches a 2D array for a given string
// takes a 2D array, a string, and length as arguments
// returns 1 if the string already exists in the list
int find(char** list, char* word, int size){
    int found = 0;
    for (int i = 0; i < size; i++){
        if (strcmp(list[i], word) == 0)
            found = 1;
    }
    return found;
}

// checks if chars of a string are present in another string
// takes two strings as arguments
// returns 1 if the string of the dictionary is a valid word
int lettersPresent(char* dictionaryIndex, char* search){
    int searchWord[26];
    int indexWord[26];
    int match = 0;

    for (int i = 0; i < 26; i++){
        searchWord[i] = 0;
        indexWord[i] = 0;
    }

    for (int k = 0; k < strlen(search); k++){
        searchWord[search[k] - 97]++;
    }
    for (int i = 0; i < strlen(dictionaryIndex); i++){
        indexWord[dictionaryIndex[i] - 97]++;
    }
    for (int j = 0; j < 26; j++){
        if (indexWord[j] >= searchWord[j])
            ++match;
    }

    if (match == 26)
        return 1;
    else return 0;
}

void insertElement(char*** mainList, char*** newList, int index, int* size, int* count){
    // reallocate every 100 words
    if (*count == 100){
        *newList = (char**)realloc(*newList, (*size+100)*sizeof(char*));
        *count = 0;
    }
    // make space at index
    (*newList)[*size] = (char*)malloc(strlen((*mainList)[index])+1); 
    strcpy((*newList)[*size], (*mainList)[index]);
    // increments counter variables
    (*size)++;
    (*count)++; 
}

// searches a 2D array for words of a specified word length and adds those to another list
// takes a char**, a char**, an int* counter, an int* newListNums, an int listNums, and length as arguments
// returns a char** list
char** wordLength(char** list, char** newList, int* count, int* newListNums, int listNums, int lengthOfWord){
    for (int x = 0; x < listNums; x++){
        if (strlen(list[x])-1 == lengthOfWord && !find(newList, list[x], *newListNums)){
            // reallocate every 100 words
            if (*count == 100){
                newList = (char**)realloc(newList, (*newListNums+100)*sizeof(char*));
                *count = 0;
            }
            // make space at index
            newList[*newListNums] = (char*)malloc(strlen(list[x])+1); 
            strcpy(newList[*newListNums], list[x]);
            // increments counter variables
            (*newListNums)++;
            (*count)++; 
        }
    }
    return newList;
}
// searches a 2D array for words excluding specified chars and adds those to another list
// takes a char**, a char**, an int* counter, an int* newListNums, an int listNums, and char* to exclude
// returns a char** list
char** excludingLett(char** list, char** newList, int* count, int* newListNums, int listNums, char* containLett){
    for (int x = 0; x < listNums; x++){
        if (!lettersPresent(list[x], containLett) && !find(newList, list[x], *newListNums)){
            // reallocate every 100 words
            if (*count == 100){
                newList = (char**)realloc(newList, (*newListNums+100)*sizeof(char*));
                *count = 0;
            }
            // make space at index
            newList[*newListNums] = (char*)malloc(strlen(list[x])+1); 
            strcpy(newList[*newListNums], list[x]);
            // increments counter variables
            (*newListNums)++;
            (*count)++; 
        }
    }

    return newList;
}

// searches a 2D array for words that start with a specific string
// takes a char**, a char**, an int* counter, an int* newListNums, an int listNums, and char* string to search for
// returns a char** list
char** startWith(char** list, char** newList, int* count, int* newListNums, int listNums, char* containLett){
    for (int x = 0; x < listNums; x++){
        if (strstr(list[x], containLett) != NULL && containLett[0] == list[x][0] && !find(newList, list[x], *newListNums)){ 
            // reallocate every 100 words
            if (*count == 100){
                newList = (char**)realloc(newList, (*newListNums+100)*sizeof(char*));
                *count = 0;
            }
            // make space at index
            newList[*newListNums] = (char*)malloc(strlen(list[x])+1); 
            strcpy(newList[*newListNums], list[x]);
            // increments counter variables
            (*newListNums)++;
            (*count)++;
        }
    }
    return newList;
}

// searches a 2D array for words that end with a specific string
// takes a char**, a char**, an int* counter, an int* newListNums, an int listNums, and char* string to search for
// returns a char** list
char** endsWith(char** list, char** newList, int* count, int* newListNums, int listNums, char* containLett){
    for (int x = 0; x < listNums; x++){
        if (strstr(list[x], containLett) != NULL && containLett[strlen(containLett)-1] == list[x][strlen(list[x])-1] && !find(newList, list[x], *newListNums)){ 
            // reallocate every 100 words
            if (*count == 100){
                newList = (char**)realloc(newList, (*newListNums+100)*sizeof(char*));
                *count = 0;
            }
            // make space at index
            newList[*newListNums] = (char*)malloc(strlen(list[x])+1); 
            strcpy(newList[*newListNums], list[x]);
            // increments counter variables
            (*newListNums)++;
            (*count)++;
        }
    }
    return newList;
}
// searches a 2D array for words that has a specific string occuring not at the beggining and not at the end
// takes a char**, a char**, an int* counter, an int* newListNums, an int listNums, and char* string to search for
// returns a char** list
char** inMiddle(char** list, char** newList, int* count, int* newListNums, int listNums, char* containLett){
    for (int x = 0; x < listNums; x++){
        if (strstr(list[x], containLett) != NULL && containLett[0] != list[x][0] && containLett[strlen(containLett)-1] != list[x][strlen(list[x])-2] && !find(newList, list[x], *newListNums)){
            // reallocate every 100 words
            if (*count == 100){
                newList = (char**)realloc(newList, (*newListNums+100)*sizeof(char*));
                *count = 0;
            }
            // make space at index
            newList[*newListNums] = (char*)malloc(strlen(list[x])+1); 
            strcpy(newList[*newListNums], list[x]);
            // increments counter variables
            (*newListNums)++;
            (*count)++;
        }
    }
    return newList;
}
// searches a 2D array for words have a specific string anywhere
// takes a char**, a char**, an int* counter, an int* newListNums, an int listNums, and char* string to search for
// returns a char** list
char** anywhere(char** list, char** newList, int* count, int* newListNums, int listNums, char* containLett){
    for (int x = 0; x < listNums; x++){
        if (strstr(list[x], containLett) != NULL && !find(newList, list[x], *newListNums)){
            // reallocate every 100 words
            if (*count == 100){
                newList = (char**)realloc(newList, (*newListNums+100)*sizeof(char*));
                *count = 0;
            }
            // make space at index
            newList[*newListNums] = (char*)malloc(strlen(list[x])+1); 
            strcpy(newList[*newListNums], list[x]);
            // increments counter variables
            (*newListNums)++;
            (*count)++;
        }
    }
    return newList;
}

int main(int argc, char* argv[]){

    FILE* inFile = fopen(argv[1], "r");
    FILE* outFile = fopen(argv[2], "w");
    

    char** dictionary = (char**)malloc(100*sizeof(char*)); // allocates an array of char* pointers
    char** midArr = (char**)malloc(100*sizeof(char*));
    char** outputArr = (char**)malloc(100*sizeof(char*));


    char wordIn[30];
    int index = 0; // this will be the total number of entries for the dictionary
    int totalNums = 0, count = 0; // these will keep track of things while filling both arrays
    int input, tempCount, col;

    // error messages for file not being supplied or not opening
    if (argc == 1){
        printf("please supply a dictionary file and an output file!\n");
        exit(1);
    }
    else if (argc == 2){
        printf("please supply an output file!\n");
        exit(1);
    }

    if (inFile == NULL){
        printf("the input file can't be opened!\n");
        exit(1);
    }

    // read in the dictionary
    while(fgets(wordIn, 30, inFile)){
        // reallocate every 100 words
        if (count == 100){
            dictionary = (char**)realloc(dictionary, (totalNums+100)*sizeof(char*));
            count = 0;
        }
        dictionary[index] = (char*)malloc(strlen(wordIn)+1);
        // convert to lower case
        for (int i = 0; i < strlen(wordIn); i++) {
            wordIn[i] = tolower(wordIn[i]);
        }
        wordIn[strlen(wordIn)-2] = 0;
        strcpy(dictionary[index], wordIn);
        index = ++totalNums;
        count++;
    }

    char containLett[32];
    int lengthOfWord;
    int midTotalNums = 0;
    totalNums = 0;
    count = 0;

    // does the first narrowing down of the dictionary
    printf("Default - Containing the letters\n");
    printf("Enter initial letters to narrow your search: ");
    scanf("%s", containLett);
    for (int x = 0; x < index; x++){
        if (lettersPresent(dictionary[x], containLett)){    
            insertElement(&dictionary, &midArr, x, &midTotalNums, &count);
        }
    }

    printf("Enter 1 to see advanced search options or 0 to exit and see results now: ");
    scanf("%d", &input);
    while (!(input == 1 || input == 0)){printf("Please enter a valid choice: "); scanf("%d", &input);}

    // branching choice for if user wants to do advanced search or not
    if (input == 1){

        count = 0;

        printf("\nAdvanced Search\n1 - Word length...\n2 - Exclude...\n3 - Starts with...");
        printf("\n4 - Ends with...\n5 - In the middle...\n6 - Anywhere...\n0 - stop\n");

        printf("\nEnter a number from the menu above in sequential order: ");
        scanf("%d", &input);
        while (!(input >= 0 && input <=6)){printf("Please enter a valid choice: "); scanf("%d", &input);}

        while (1){
            switch(input){
                case 1: // word length
                    printf("Word length: ");
                    scanf("%d", &lengthOfWord);
                    outputArr = wordLength(midArr, outputArr, &count, &totalNums, midTotalNums, lengthOfWord);
                    break;
                case 2: // exclude 
                    printf("Excluding: ");
                    scanf("%s", containLett);
                    outputArr = excludingLett(midArr, outputArr, &count, &totalNums, midTotalNums, containLett);
                    break;
                case 3: // starts with
                    printf("Starts with: ");
                    scanf("%s", containLett);
                    outputArr = startWith(midArr, outputArr, &count, &totalNums, midTotalNums, containLett);
                    break;
                case 4: // ends with 
                    printf("Ends with: ");
                    scanf("%s", containLett);
                    outputArr = endsWith(midArr, outputArr, &count, &totalNums, midTotalNums, containLett);
                    break;
                case 5: // in the middle
                    printf("In the middle: ");
                    scanf("%s", containLett);
                    outputArr = inMiddle(midArr, outputArr, &count, &totalNums, midTotalNums, containLett);
                    break;
                case 6: // anywhere
                    printf("Anywhere: ");
                    scanf("%s", containLett);
                    outputArr = anywhere(midArr, outputArr, &count, &totalNums, midTotalNums, containLett);
                    break;
                        
                case 0: // quit
                    tempCount = 1;
                    col = 3;

                    // find the number of columns that the words can evenly fit in
                    for (int y = 3; y < 6; y++){
                        if (totalNums % y == 0)
                            col = y;
                    }
                    // print out the words, go to newline everytime counter equals 
                    for(int i = 0; i < totalNums; i++){
                        fprintf(outFile, "%-25s \t", outputArr[i]);
                        if (tempCount == col){
                            fprintf(outFile, "\n");
                            tempCount = 0;
                        }
                        ++tempCount;
                    }
                    fclose(outFile);

                    for(int i = 0; i < index; i++) free(dictionary[i]);
                    free(dictionary);

                    for(int i = 0; i < midTotalNums; i++) free(midArr[i]);
                    free(midArr);
                    
                    for(int i = 0; i < totalNums; i++) free(outputArr[i]);
                    free(outputArr);

                    printf("Finished. Check your output file!\n");
                    exit(1);
                    break;
            }
            printf("\nEnter a number from the menu above (enter 0 to stop selection/quit): ");
            scanf("%d", &input);
            while (!(input >= 0 && input <=6)){
               printf("Please enter a valid choice: ");
                scanf("%d", &input);
            }
        }
    }
    else if (input == 0){
        tempCount = 1;
        col = 3;

        // find the number of columns that the words can evenly fit in
        for (int y = 3; y < 6; y++){
            if (totalNums % y == 0)
                col = y;
        }
        // print out the words, go to newline everytime counter equals 
        for(int i = 0; i < midTotalNums; i++){
            fprintf(outFile, "%-25s \t", midArr[i]);
            if (tempCount == col){
                fprintf(outFile, "\n");
                tempCount = 0;
            }
            ++tempCount;
        }
        fclose(outFile);

        for(int i = 0; i < index; i++) free(dictionary[i]);
        free(dictionary);

        for(int i = 0; i < midTotalNums; i++) free(midArr[i]);
        free(midArr);
        
        for(int i = 0; i < totalNums; i++) free(outputArr[i]);
        free(outputArr);

        printf("Finished. Check your output file!\n");
        exit(1);
    }

}