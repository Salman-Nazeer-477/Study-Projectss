#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
//#include <termios.h>

#define MAX_USERS 10
#define CREDENTIALS_LENGTH 50

typedef struct
{
    char username[CREDENTIALS_LENGTH];
    char password[CREDENTIALS_LENGTH];
} User;

User users[MAX_USERS];
int user_count = 0;

void register_user();
int login_user();
void fix_fgets_input(char*);
void input_credentials(char* username, char* password);

int main()
{

    int choice;
    int user_index;
    while (1)
    {
        printf("\nHello, Welcome to User Management System\n");
        printf("\n1. Register User");
        printf("\n2. Login User");
        printf("\n3. Exit\n");
        printf("Total registered users: %d\n", user_count);
        printf("\nPlease enter your choice: ");

        scanf("%d", &choice);
        getchar(); // Consume newline character left by scanf

        switch (choice)
        {
        case 1:
            register_user();
            break;
        case 2:
            user_index = login_user();
            if(user_index >= 0){
                printf("\nLogin successful. \nWelcome, %s!\n", users[user_index].username);
            }else{
                printf("\nLogin failed. \nIncorrect username or password.\n");
            }
            break;
        case 3:
            printf("\nExiting the program.\n");
            return 0;
            break;

        default:
            printf("\nInvalid choice. Please try again.\n");
            break;
        }
    }

    return 0;
}

void register_user(){
    if(user_count >= MAX_USERS){
        printf("User limit reached(%d). Cannot register more users.\n", MAX_USERS);
        return;
    }

    int new_index = user_count;
    printf("\nRegistering a new user.\n");

    input_credentials(users[new_index].username, users[new_index].password);

    printf("User %s registered successfully!.\n", users[new_index].username);
    printf("Welcome, %s!\n", users[new_index].username);

    user_count++;
}
int login_user(){
    char username[CREDENTIALS_LENGTH]; 
    char password[CREDENTIALS_LENGTH];
    
    input_credentials(username, password);

    for(int i=0; i<user_count; i++){
        if(strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0){
            return i;
        }
    }

    return -1;
}
void fix_fgets_input(char* string){
    int index = strcspn(string, "\n");
    string[index] = '\0';
}
void input_credentials(char* username, char* password) {
    printf("Enter username: ");
    fgets(username, CREDENTIALS_LENGTH, stdin);
    fix_fgets_input(username);

    printf("Enter password: ");
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < CREDENTIALS_LENGTH - 1) { // '\r' is Enter key
        if (ch == '\b' && i > 0) { // handle backspace
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}