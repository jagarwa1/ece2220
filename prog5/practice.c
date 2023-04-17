#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *_strncat(char* dest, char* src, int n){
    int i = 0;
    int j = 0;
    while(dest[i] != 0){
        i++;
    }
    while (j < n){
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = 0;

    return dest;
}

char* _strstr(char* s1, char* s2){
    int len1 = 0;
    int len2 = 0;
    int match;

    while (s1[len1] != '\0'){ // find length of s2
        len1++;
    }

    while (s2[len2] != '\0'){
        len2++;
    }
    for (int i = 0; i < len1; i++){
        match = 1;
        for (int j = 0; j < len2; j++){
            if (s1[i+j] != s2[j])
                match = 0;
        }
        if (match == 1){
            return (s1+i);
        }
    }
    return NULL;
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

char** removeElement(char** list, int indexToRemove, int* size, int* count){

    char** tempArr = malloc((*size - 1) * sizeof(char*)); // allocate an array with a size 1 less than the current one
    int tempCount = 0, tempSize = 0;

    memmove(tempArr, list, (indexToRemove+1)*sizeof(char*)); // copy everything BEFORE the index
    memmove(tempArr+indexToRemove, list+(indexToRemove+1), (*size - indexToRemove)*sizeof(char*)); // copy everything AFTER the index

    --(*size);
    --(*count);
    return tempArr;
    
}

struct
{ unsigned i : 2;
  unsigned j : 3;
  unsigned k : 4;
} b;

int main(void)
{
  b.i = 3;
  b.j = 1;
  b.k = 2;

  printf("%d", b);
}
