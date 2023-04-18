/*
Jai Agarwal
ECE 2220
Project 3: Huffman Encoding and Decoding
Purpose: Encode an inputted string using Huffman encoding. Decode an inputted hex string using Huffman encoding
Status of Code: Created different functions to help convert and print in binary. Code does not function properly. Could not figure out how to do many of the required tasks.

Assumptions - assumes that capital letters are entered for decoding
            
Known Bugs  - each element of codeword[] holds only one letter
            - does not decode 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define UCHAR unsigned char
// function (taken from class examples) that prints out binary value
void print_bits(char *text, void *mem, UCHAR len){
    UCHAR *addr; int bit;
    // *** Print Binary Value of Memory *** //
    for (addr=(UCHAR *)mem + len - 1; addr>=(UCHAR *)mem; addr--){
        for (bit = 7; bit>=0; bit--) 
            printf((*addr >> bit) & 0x01 ? "1" : "0");
    }
    printf("\n");
}

//function that returns the number of bits of each huffman code character
int get_size(int j);

// functoin that takes in a hex value 0 to F and returns its binary value
int toBinary(char input);
 

int main(){
    int inputChoice = 0;
    char inputString[64];
    int encodeArray[64];
    // A = 0, B = 1, . . . Z = 25, [ = 26                             70                                               75                                           80                                           85                                                     90
    int huffmanCode[] = {0b0001, 0b001111, 0b10110, 0b00000, 0b011, 0b11011, 0b001100, 0b1001, 0b0100, 0b11001011, 0b1100100, 0b00001, 0b11000, 0b0101, 0b0010, 0b001110, 0b1100101000, 0b1010, 0b1000, 0b111, 0b1011, 0b110011, 0b11010, 0b1100101001, 0b001101, 0b1100101010, 0b1100101011};
    short int codeword[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0};
    int buffer[64];
    short int decode[30];

    printf("Enter 1 to encode data, enter 2 to decode data, enter 3 to quit: ");
    scanf("%d", &inputChoice);
    getchar();

    while(!(inputChoice <= 3 && inputChoice >= 1)){
        printf("Invalid, try again: ");
        scanf("%d", &inputChoice);
        getchar();  
    }
    
    while(1){

        for(int i = 0; i<64; i++){ // clear the inputString
            inputString[i] = 0;
            buffer[i] = 0;
        }

        // encoding operation
        if (inputChoice == 1){
            printf("Please enter the desired string with a '[' at the end: ");
            fgets(inputString, 64, stdin);
            inputString[strcspn(inputString, "[")+1] = '\0';  // this line sets the null character the where the EOT symbol was entered 

            int i = 0;
            for (char *itr = inputString; *itr; ++itr) {   // this loop will fill in the encodeArray and will convert letters to uppercase
                if ((isalpha(*itr) && !isspace(*itr)) || (*itr == '[')){
                    encodeArray[i] = toupper(*itr);
                    i++;
                }
            }
            encodeArray[i] = '\0'; //this is setting the null character to the proper place (symmetric to inputString)
            printf("\nString to encode: ");
            for (int i = 0; encodeArray[i] != '['; i++)
                printf("%c", encodeArray[i]);

            short int code = 0;
            for (i = 0; encodeArray[i] != '\0'; i++){
                for(int j = 65; j <= 91; j++){
                    code = 0;
                    if (encodeArray[i] == j){
                        buffer[i] = get_size(j);

                        short int reversed = 0;
                        code |= huffmanCode[26-(91-j)];
                        for(int k = 0; k <= buffer[i]; k++){ //this code reverses the bits
                            if ((code & (1 << k)))
                                reversed |= 1 << ((buffer[i])-k);
                        }
                        codeword[i+1] |= reversed;
                    }
                }
            }
            codeword[0] = i;
        
            printf("\ncodeword[0]: %d\n\n", codeword[0]);
            for(int i = 1; codeword[i] != '\0'; i++){
                printf("codeword[%d]: ", i);
                for (int bit = buffer[i-1]; bit>=0; bit--){ 
                    printf((codeword[i] >> bit) & 0x01 ? "1" : "0");
                }
                printf("\t0x%X \t%o", codeword[i], codeword[i]);
                puts("\n");

            }
        }
        //decoding operation
        else if (inputChoice == 2){
            printf("Please enter the hex (capital letters) you want to decode: ");
            fgets(inputString, 64, stdin);

            int i = 0;
            for (char *itr = inputString; *itr; ++itr) {   // this loop will fill in the decode array 
                decode[i] = toBinary(*itr);
                i++;
            }
            decode[i] = '\0'; // my way of setting the null
            
            printf("String to decode: ");
            short int reversed = 0;
            for(int i = 0; decode[i] != '\0'; i++){
                for(int k = buffer[i]; k <= buffer[i]; k++){ //this code reverses the bits
                    if ((decode[i] & (1 << k)))
                    reversed |= 1 << ((buffer[i])-k);
                    printf("%X ", decode[i]);

                }
            }
            printf("\nSorry, I don't work.\n");
        }
        // quitting the program
        else if (inputChoice == 3){
            printf("\nQuitting program...\n");
            exit(1);
        }
        printf("Enter 1 to encode data, enter 2 to decode data, enter 3 to quit: ");
        scanf("%d", &inputChoice);
        getchar();

        while(!(inputChoice <= 3 && inputChoice >= 1)){
            printf("Invalid, try again: ");
            scanf("%d", &inputChoice);
            getchar();  
        }
    }
} //end main

//function that returns the number of bits of each huffman code character
int get_size(int j){
    int buffer = 0;
    if (j == 65) buffer = 3;
    else if (j == 66) buffer = 5;
    else if (j == 67) buffer = 4;
    else if (j == 68) buffer = 4;
    else if (j == 69) buffer = 2;
    else if (j == 70) buffer = 4;
    else if (j == 71) buffer = 5; 
    else if (j == 72) buffer = 3;
    else if (j == 73) buffer = 3;
    else if (j == 74) buffer = 7;
    else if (j == 75) buffer = 6;
    else if (j == 76) buffer = 4;
    else if (j == 77) buffer = 4;
    else if (j == 78) buffer = 3;
    else if (j == 79) buffer = 3;
    else if (j == 80) buffer = 5;
    else if (j == 81) buffer = 9;
    else if (j == 82) buffer = 3;
    else if (j == 83) buffer = 3;
    else if (j == 84) buffer = 2;
    else if (j == 85) buffer = 3;
    else if (j == 86) buffer = 5;
    else if (j == 87) buffer = 4;
    else if (j == 88) buffer = 9;
    else if (j == 89) buffer = 5;
    else if (j == 90) buffer = 9;
    else if (j == 91) buffer = 9;
    return buffer;
}

// functoin that takes in a hex value 0 to F and returns its binary value
int toBinary(char input){
    if (input == 1){return 0b0001;}
    else if (input == '2'){return 0b0010;}
    else if (input == '3'){return 0b0011;}
    else if (input == '4'){return 0b0100;}
    else if (input == '5'){return 0b0101;}
    else if (input == '6'){return 0b0110;}
    else if (input == '7'){return 0b0111;}
    else if (input == '8'){return 0b1000;}
    else if (input == '9'){return 0b1001;}
    else if (input == 'A'){return 0b1010;}
    else if (input == 'B'){return 0b1011;}
    else if (input == 'C'){return 0b1100;}
    else if (input == 'D'){return 0b1101;}
    else if (input == 'E'){return 0b1110;}
    else if (input == 'F'){return 0b1111;}
    else return 0;
}