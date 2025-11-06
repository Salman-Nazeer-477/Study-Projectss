#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){

    int randomNum, guess;
    int numOfGuesses = 0;
    srand(time(NULL));

    printf("\n\n--Welcome to the World of Guessing Numbers!--\n\n");
    randomNum = rand() % 100 + 1; // Generate a random number between 1 and 100

    do{
        
        printf("Please enter your guess(1 - 100): ");
        scanf("%d", &guess);
        numOfGuesses++;

        if(guess < 1 || guess > 100){
            printf("Invalid input! Please try again.\n\n");
            continue;
        }else if(guess < randomNum){
            printf("Too low! Try again.\n\n");
        }else if(guess > randomNum){
            printf("Too high! Try again.\n\n");
        }else{
            printf("Congratulations! You've guessed the number in %d attempts!\n\n", numOfGuesses);  
        }

    }while(guess != randomNum);

    printf("Thank you for playing! Goodbye!\n\nDeveloped by ZiMu.\n\n");
    
    return 0;

}