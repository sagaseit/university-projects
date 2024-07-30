#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Phone book database. Each person has phone number, name and encrypted name in T9 coding system
 */
typedef struct Person {
    char *phone_number;
    char *name;
    char *encrypted;
} Person;

/**
 * Function that searches the phone book. Search for records that match the query,
 * display the records searched, and display the number of records found.
 * @param buffer user's input
 * @param inputLength input's length
 * @param count amount of people in database
 * @param people phone book database
 * @return true and shows people that match the query, false if the command is not formatted correctly.
 */
bool searchByPrefix(const char *buffer, size_t inputLength, int count, Person *people);

/**
 * Determines if a given search string is a prefix of another string
 * @param search prefix that will be searched
 * @param string phone number/coded name
 * @return true, if the prefix found in given string,
 * false if not found or the length of the string is less than the length of prefix
 */
bool isPrefix(const char *search, const char *string);

/**
 * Boolean function determinise if the person with the same name and phone number already exists in phone book
 * @param people phone book database
 * @param count phone book's size
 * @param name upcoming person's name
 * @param number upcoming person's phone number
 * @return true, if person already exists in database, false, if not
 */
bool samePerson(Person *people, int count, char *name, char *number);

/**
 * Increases memory for phone book
 * @param people phone book array
 * @param capacity of the phone book that will be increased
 */
void memoryIncrease(Person **people, int *capacity);

/**
 * Handles user's input
 * @param buffer array of chars where user's input will be stored and parsed
 * @param bufferSize input's size
 */
void input(char *buffer, size_t bufferSize);

/**
 * Function uses T9, which is a predictive text technology for mobile phones.
 * @param literal letter which will be converted to appropriate number
 * @return number coded in T9 or -1 if the letter is not in T9 format
 */
int codingSystem(char literal);

/**
 * Function frees dynamically allocated array of chars for the name and encrypted name
 */
void memoryClean(char *name, char *encrypted);

/**
 * Function that adds each person's name and phone number in database
 * @param people people in phone book
 * @param count amount of people in phone book
 * @param encrypted person's name in T9 coding system
 * @param name person's real name
 * @param nameLength person's name's length
 * @param numLength person's number's length
 * @param number person's phone number
 */
void addContact(Person **people, int *count, char *encrypted, char *name, size_t nameLength, size_t numLength, char number[]);

int main() {
    char *buffer = NULL;
    size_t bufferSize = 0;
    input(buffer, bufferSize);
    return EXIT_SUCCESS;
}

int codingSystem(char literal) {
    int T9[26] = {
            2, 2, 2,
            3, 3, 3,
            4, 4, 4,
            5, 5, 5,
            6, 6, 6,
            7, 7, 7, 7,
            8, 8, 8,
            9, 9, 9, 9
    };

    if(isalpha(literal)){
        literal = (char)toupper(literal);
        int index = literal - 'A';
        return T9[index];
    } else if(literal == ' '){
        return 1;
    } else {
        return -1;
    }
}

bool isPrefix(const char *search, const char *string){
    size_t searchLength = strlen(search), stringLength = strlen(string);
    if (stringLength < searchLength) {
        return false;
    } else {
        int cmpResult = memcmp(search, string, searchLength);
        if (cmpResult == 0) {
            return true;
        } else {
            return false;
        }
    }
}

void memoryIncrease(Person **people, int *capacity){
    *capacity *= 2;
    Person *temporary = (Person *) realloc(*people, *capacity * sizeof(Person));
    if(temporary == NULL){
        printf("Nespravny vstup.\n");
        exit(1);
    }
    *people = temporary;
}

bool searchByPrefix(const char *buffer, size_t inputLength, int count, Person *people){
    if(inputLength == 2 || buffer[2] == '\n'){
        return false;
    }
    char *search = NULL;
    size_t start = 2;
    size_t prefixLength = inputLength - start;
    search = (char *) malloc((prefixLength + 1) * sizeof(char));
    int prefixCount = 0;
    //Loop initializes search string and checks if the input is valid
    for(size_t i = start; i < inputLength; i++){
        char letter = buffer[i];
        if(!isdigit(letter) && letter != '\n'){
            free(search);
            return false;
        }
        search[prefixCount] = letter;
        prefixCount++;
    }
    if(search[prefixLength - 1] == '\n'){
        search[prefixLength - 1] = '\0';
    }
    int amount = 0;
    //Loop uses "isPrefix" function to find out if the person's name or phone number prefix matches the given prefix
    for(int i = 0; i < count; i++){
        if(isPrefix(search, people[i].phone_number) || isPrefix(search, people[i].encrypted)){
            amount++;
        }
    }
    if(amount > 10){
        printf("Celkem: %d\n", amount);
    } else {
        amount = 0;
        for(int i = 0; i < count; i++){
            if(isPrefix(search, people[i].phone_number) || isPrefix(search, people[i].encrypted)){
                printf("%s %s\n", people[i].phone_number, people[i].name);
                amount++;
            }
        }
        printf("Celkem: %d\n", amount);
    }
    free(search);
    return true;
}

void memoryClean(char *name, char *encrypted){
    free(name);
    free(encrypted);
}

bool samePerson(Person *people, int count, char *name, char *number){
    for(int i = 0; i < count; i++){
        if(strcmp(people[i].phone_number, number) == 0 &&
           strcmp(people[i].name, name) == 0){
            return true;
        }
    }
    return false;
}

void addContact(Person **people, int *count, char *encrypted, char *name, size_t nameLength, size_t numLength, char number[]){
    (*people)[*count].phone_number = (char *)malloc((numLength + 1) * sizeof(char));
    (*people)[*count].encrypted = (char *)malloc((nameLength + 1) * sizeof(char));
    (*people)[*count].name = (char *)malloc((nameLength + 1) * sizeof(char));
    strcpy((*people)[*count].phone_number, number);
    strcpy((*people)[*count].encrypted, encrypted);
    strcpy((*people)[*count].name, name);
    (*count)++;
    memoryClean(name, encrypted);
}

void input(char *buffer, size_t bufferSize){
    int capacity = 5, count = 0, name_count = 0;
    Person *people = (Person *) malloc(capacity * sizeof(Person));
    while (getline(&buffer, &bufferSize, stdin) != EOF){
        size_t inputLength = strlen(buffer);
        char number[100], *name = NULL, *encrypted = NULL;
        if(inputLength == 2 && (buffer[0] == '?' || buffer[0] == '+')){
            memoryClean(name, encrypted);
            printf("Nespravny vstup.\n");
            continue;
        }
        if(buffer[0] == '+'){
            bool isDigit = false;
            //ssanf parses input and assigns phone number into a number variable
            if((sscanf(buffer, " + %s", number)) != 1){
                memoryClean(name, encrypted);
                printf("Nespravny vstup.\n");
                continue;
            }
            size_t numLength = strlen(number);
            if(numLength > 20 || numLength < 1 || buffer[1] != ' '){
                memoryClean(name, encrypted);
                printf("Nespravny vstup.\n");
                continue;
            }
            //Loop checks if phone number was entered correctly(no symbols except for numbers)
            for(size_t i = 0; i < numLength; i++){
                if(!isdigit(number[i])){
                    isDigit = true;
                    break;
                }
            }
            if(isDigit){
                memoryClean(name, encrypted);
                printf("Nespravny vstup.\n");
                continue;
            }
            if(capacity == count){
                //If maximum capacity was reached, function doubles its capacity and reallocated memory
                memoryIncrease(&people, &capacity);
            }
            size_t st = 3 + numLength;
            if(inputLength == st || buffer[st] == ' ' || buffer[inputLength - 2] == ' '){
                memoryClean(name, encrypted);
                printf("Nespravny vstup.\n");
                continue;
            }
            size_t nameLength = (inputLength - st) + 1;
            name = (char *) malloc(nameLength * sizeof(char));
            encrypted = (char *) malloc(nameLength * sizeof(char));
            bool isLetter = false;
            //For loop initializes person's name and encrypted name by parsing input
            for(size_t i = st; i < inputLength; i++){
                char letter = buffer[i];
                if(isdigit(letter) || (codingSystem(letter) == -1 && letter != '\n')){
                    isLetter = true;
                    break;
                }
                encrypted[name_count] = (char)(codingSystem(letter) + '0');
                name[name_count] = letter;
                name_count++;
            }
            if(isLetter){
                memoryClean(name, encrypted);
                name_count = 0;
                printf("Nespravny vstup.\n");
                continue;
            }
            if (name[name_count - 1] == '\n') {
                name[name_count - 1] = '\0';
            }
            encrypted[name_count] = '\0';
            //Function checks if the person with the same name and phone number
            // was already added into a phone book
            if(samePerson(people, count, name, number)){
                printf("Kontakt jiz existuje.\n");
                memoryClean(name, encrypted);
                name_count = 0;
                continue;
            }
            //Function adds each person's info into a phone book
            addContact(&people, &count, encrypted, name, nameLength, numLength, number);
            printf("OK\n");
            name_count = 0;
        } else if(buffer[0] == '?'){
            if(!searchByPrefix(buffer, inputLength, count, people)){
                printf("Nespravny vstup.\n");
                continue;
            }
        } else {
            memoryClean(name, encrypted);
            printf("Nespravny vstup.\n");
            continue;
        }
        name_count = 0;
    }
    free(buffer);
    //We cannot just free the memory for array of people, because in each structure person's
    //name, encrypted name and phone number was allocated dynamically too, so we need to free each variable
    for(int i = 0; i < count; i++){
        free(people[i].name);
        free(people[i].phone_number);
        free(people[i].encrypted);
    }
    free(people);
}
