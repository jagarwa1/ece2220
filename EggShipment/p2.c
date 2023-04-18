/*
Jai Agarwal
ECE 2220 
Project 2: Egg Shipment
Purpose: Menu based program that has the user enter information about their company, shipment information, and egg information. Can display all info and delete all info.
Assumptions: Program is in the same folder as StateAbbreviations.txt. User enters in a date with valid months/days.
Known bugs: When data is displayed with undeclared and declared values (specifically shipmentDate undeclared and companyName declared) the contents of companyName is outputted with the shipmentDate output.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int main(){
    
    // variables that will be used
    int input = 0;
    char companyName[33] = "X";
    char companyState[] = "XX";
    char numInput[10], shipDate[] = "XX-XX-XXXX";
    int shipNum = 0;
    float shipCost = 0.00;
    char eggGrade[] = "XX";
    char sizeInput[] = "unknown";
    int eggSize = 0;
    float eggCost = 0.00;
    int flag;


    do { // outer most infinite loop
        if (input == 1){ //company information
            
            // declare variables specific to this scope
            FILE *stateAbbreviations;
            char stateInput[50], stateOutput[50]; // stateInput is for reading from the file, stateOutput is for recording the names of the states
            flag = 1; // flag variable that will be used in the input validation

            printf("\nCompany Information\n");
            
            //company name
            printf("Enter Company Name: ");
            while (flag == 1){
                fgets(companyName, 33, stdin);
                
                while (strcspn(companyName, "\n") > 32){
                    printf("Invalid input. Enter a name with 32 characters or less: ");
                    fgets(companyName, 33, stdin);

                }
                for(int i = 0; companyName[i] != '\0'; i++){
                    if ((isalpha(companyName[i]) || (companyName[i] == 45 || companyName[i] == 46))){ // condition is met if the character of the string is in the alphabet, a '-', or '.'     
                        flag = 0;
                        companyName[strcspn(companyName, "\n")] = 0; //remove the newline character

                    }
                    else flag = 1;
                }

                if (flag == 1)
                    printf("Sorry, that is an invalid input: ");
            }

            //company state
            flag = 1;
            printf("Enter Company State (first letter): ");
            
            while (flag == 1){
                stateAbbreviations = fopen("StateAbbreviations.txt", "r"); // opens the file to read from, this is in the loop
                scanf("%s", companyState);                                // in case the user wants to change states for their reinput
                getchar();
                while (*companyState == 'B' || *companyState == 'E' || *companyState == 'J' || *companyState == 'Q' || *companyState == 'X' || *companyState == 'Y' || *companyState == 'Z'){
                    printf("Invalid input. Enter a valid state letter: ");
                    scanf("%s", companyState);                                // in case the user wants to change states for their reinput
                    getchar();
                }
                printf("\n");
                while(fgets(stateInput, 50, stateAbbreviations)){
                    if(stateInput[0] == companyState[0]){
                        printf("%s", stateInput);
                        strcat(stateOutput, stateInput);
                    }
                }
                
                printf("\n");
                printf("From above, enter the state's complete two letter abbreviation: ");
                scanf("%2s", companyState);
                while(strlen(companyState) != 2){
                    printf("Invalid, enter the complete two letter abbreviation: ");
                    scanf("%2s", companyState);
                }
                if (strstr(stateOutput, companyState) != NULL){
                    flag = 0;
                }
                else printf("Sorry, invalid input. Enter the first letter again: ");
                
                fclose(stateAbbreviations); // closes the file
            }
                    
        } // end case 1
        else if (input == 2){ // shipment information

            printf("\nShipment Information\n");

            // shipment number
            printf("Enter shipment number: ");
            scanf("%s", numInput);
            getchar();
            shipNum = atoi(numInput); // converts string to int
            while (!(shipNum > 0 && shipNum < 1000000000)){ //input validation
                printf("Invlaid, enter shipment number (1 to 999,999,999): ");
                scanf("%s", numInput);
                getchar();
                shipNum = atoi(numInput);
            }
            // shipment date
            printf("Enter shipment date (mm-dd-yyyy): ");
            scanf("%s", shipDate);
            getchar();
            while(!(shipDate[2] == 45 && shipDate[5] == 45 && strlen(shipDate) == 10)){ //input validation
                printf("Sorry, invalid input. Enter with format (mm-dd-yyyy): ");
                scanf("%s", shipDate);
            }
            // shipment cost
            printf("Enter shipment cost (USD): ");
            scanf("%f", &shipCost);
            getchar();
            

        }// end case 2
        else if (input == 3){// egg information
            flag = 1;
            
            printf("\nEgg Information\n");

            //egg grade
            printf("Enter the egg grade: ");
            scanf("%s", eggGrade);
            getchar();

            //egg size
            printf("Enter the egg size (Small, Medium, Large, Extra-Large, or Jumbo): ");
            while (flag == 1){ //input validation for size input, string entered must be case sensitive
                scanf("%s", sizeInput);
                getchar();
                if (strcmp(sizeInput, "Small") == 0 || strcmp(sizeInput, "Medium") == 0 || strcmp(sizeInput, "Large") == 0 || strcmp(sizeInput, "Extra-Large") == 0 || strcmp(sizeInput, "Jumbo") == 0)       
                        flag = 0;
                else flag = 1;

                if (flag == 1)
                    printf("Sorry, that is an invalid input: ");
            }
            eggSize = atoi(sizeInput); //converts string to int
            
            //individual egg cost
            printf("Enter the individual egg cost: ");
            scanf("%f", &eggCost);
            while (!(eggCost >= 0.01 && eggCost < 1000000.00)){ //input validation
                printf("Invalid input of %f, enter a number within $0.01 and $999,999.99: ", eggCost);
                scanf("%f", &eggCost);
            }
        
        }// end case 3
        else if (input == 4){//display all values

            if (eggSize != 0){ // if eggsize has not been entered, this makes it so the default string of "unknown" is printed instead of a 0
                sprintf(sizeInput, "%d", eggSize);
            }


            printf("\nDisplaying all values: \n");
            //display all details
            printf("Company Name: %s\n", companyName);
            printf("Company State: %s\n", companyState);
            printf("Shipment Number: %d\n", shipNum);
            printf("Shipment Date: %s\n", shipDate);
            printf("Shipment Cost: $%0.2f\n", shipCost);
            printf("Egg Grade: %s\n", eggGrade);
            printf("Egg Size: %s\n", sizeInput);
            printf("Individual Egg Cost: $%0.2f\n", eggCost);

        }// end case 4
        else if (input == 5){// clear all data

            // resets the companyName string
            *companyName = 'X', companyName[1] = 0;
            //resets the companyState string
            *companyState = 'X', companyState[1] = 'X', companyState[2] = 0;
            
            // resets the shipment date in the proper format
            for (int i = 0; i<=10; i++){
                if(i == 2 || i == 5)
                    shipDate[i] = '-';
                else 
                    shipDate[i] = 'X';
            }
            shipNum = 0;
            shipCost = 0.00;

            //resets the eggGrade string
            *eggGrade = 'X', eggGrade[1] = 0;
            //resets the eggSize string
            *sizeInput = 'X', sizeInput[1] = 0;
            eggSize = 0;
            eggCost = 0.00;

            printf("\nAll values are reset.\n");

        }// end case 5
        else if (input == 6){// quits the program
        
            printf("\nQuitting the program...\n");
            exit(1);
        
        }// end case 6

        printf("\nWhat do you want to do?\n");
        printf("1. Enter Company Information\n2. Enter Shipment Information\n3. Enter Egg Information\n4. Display all Data\n5. Clear all Data\n6. Quit\n");
        printf("Enter a number from the menu: ");
        scanf("%d", &input);
        while(input < 1 && input > 6){
            printf("Invalid choice. Select 1-6: ");
            scanf("%d", &input);
        }
        getchar();
    } while (1);
    
    return 0;
}