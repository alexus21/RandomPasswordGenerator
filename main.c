#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE 50
#define MIN_SIZE 8

char array[] = {
        '0', '1', '2', '3', '4','5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E','F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O','P', 'Q', 'R', 'S', 'T',
        'U','V', 'W', 'X', 'Y','Z',
        'a', 'b', 'c', 'd','e', 'f', 'g', 'h', 'i','j',
        'k', 'l', 'm', 'n','o', 'p', 'q', 'r', 's','t',
        'u', 'v', 'w', 'x','y', 'z',
        '+', '=', '{','}', '<', '>', '-', '@',' ', '%', '#', '&', '*'
};

int arrayLength = sizeof(array) / sizeof(char);

void savePassword(char* str){
    char* fileName = malloc(sizeof(char)*MAX_SIZE);

    printf("Enter file name: ");
    scanf("%s", fileName);

    FILE* generatePasswordFile = fopen(fileName, "w");

    if(generatePasswordFile == NULL){
        printf("Can\'t create file. Try it again later");
        return;
    }

    fputs(str, generatePasswordFile);
    fclose(generatePasswordFile);
    printf("Password saved successfully to %s\n", fileName);

    free(fileName);
}

void askForExportPasswordToFile(char* str){
    char response;
    printf("Do you want to save the generate password to a file? [Y/N]: ");
    scanf("%s", &response);

    if(response == 'y' || response == 'Y'){
        savePassword(str);
    }

}

void showGeneratedPassword(char* str){
    printf("Your randomized password is: %s\n", str);
    askForExportPasswordToFile(str);
}

int checkIfPasswordHasBeenLeaked(char* str){
    FILE* myFile = fopen("../common-passwords.txt", "r");

    if(myFile == NULL){
        printf("File not found");
        return 1;
    }

    char c[MAX_SIZE];
    while(fgets(c, sizeof(c), myFile)){
        if(strcmp(c, str) == 0){
            fclose(myFile);
            return 1;
        }
    }

    showGeneratedPassword(str);
    fclose(myFile);
    return 0;
}

void generateRandomPassword(char* str, int passwordLength){
    for (int i = 0; i < passwordLength; ++i) {
        str[i] = array[rand() % arrayLength];
    }

    str[passwordLength] = '\0';

    int operationResult = checkIfPasswordHasBeenLeaked(str);

    while(operationResult != 0){
        generateRandomPassword(str, passwordLength);
        operationResult = checkIfPasswordHasBeenLeaked(str);
    }
}

int main() {
    srand(time(NULL) * getpid());

    int passwordLength = 0;
    printf("Please, enter your password length: ");
    scanf("%i", &passwordLength);

    if(passwordLength > MAX_SIZE){
        printf("Error: only %i characters available. \n", MAX_SIZE);
        return EXIT_FAILURE;
    }

    if(passwordLength < MIN_SIZE){
        printf("Error: at least %i characters. \n", MIN_SIZE);
        return EXIT_FAILURE;
    }

    char* str = malloc(sizeof(char) * passwordLength);
    generateRandomPassword(str, passwordLength);

    free(str);
    return EXIT_SUCCESS;
}
