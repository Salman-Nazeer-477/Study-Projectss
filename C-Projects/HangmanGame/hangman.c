/* hangman.c
   Simple Hangman game in C.
   Compile: gcc -o hangman hangman.c
   Run:     ./hangman
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_TRIES 6
#define WORD_COUNT 20

/* ===== Word list (you can expand or load from file later) ===== */
const char *words[WORD_COUNT] = {
    "computer", "programming", "hangman", "variable", "function",
    "pointer", "array", "string", "keyboard", "network",
    "compile", "debug", "algorithm", "structure", "binary",
    "process", "thread", "library", "database", "memory"
};

/* ===== ASCII art for hangman stages (0..MAX_TRIES) ===== */
void display_hangman(int wrong) {
    /* wrong: number of wrong guesses (0..MAX_TRIES) */
    /* We'll show a simple 7-stage hangman (0 = no body, 6 = full) */
    const char *stages[] = {
        /* 0 */
        "  +---+\n"
        "  |   |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "=========\n",
        /* 1 */
        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "=========\n",
        /* 2 */
        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        "  |   |\n"
        "      |\n"
        "      |\n"
        "=========\n",
        /* 3 */
        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|   |\n"
        "      |\n"
        "      |\n"
        "=========\n",
        /* 4 */
        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        "      |\n"
        "      |\n"
        "=========\n",
        /* 5 */
        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        " /    |\n"
        "      |\n"
        "=========\n",
        /* 6 = full */
        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        " / \\  |\n"
        "      |\n"
        "=========\n"
    };
    if (wrong < 0) wrong = 0;
    if (wrong > MAX_TRIES) wrong = MAX_TRIES;
    printf("%s", stages[wrong]);
}

/* ===== Pick a random word from the words array ===== */
void choose_random_word(char dest[], size_t dest_len) {
    int idx = rand() % WORD_COUNT;
    strncpy(dest, words[idx], dest_len - 1);
    dest[dest_len - 1] = '\0';
}

/* ===== Initialize masked word with '_' for letters and copy non-letters ===== */
void init_masked(const char original[], char masked[]) {
    size_t n = strlen(original);
    for (size_t i = 0; i < n; ++i) {
        if (isalpha((unsigned char)original[i]))
            masked[i] = '_';
        else
            masked[i] = original[i]; /* keep punctuation/spaces if any */
    }
    masked[n] = '\0';
}

/* ===== Check if a char has already been guessed ===== */
int already_guessed(int guessed[], char ch) {
    int idx = tolower(ch) - 'a';
    if (idx < 0 || idx >= 26) return 0;
    return guessed[idx];
}

/* ===== Mark a letter as guessed ===== */
void set_guessed(int guessed[], char ch) {
    int idx = tolower(ch) - 'a';
    if (idx < 0 || idx >= 26) return;
    guessed[idx] = 1;
}

/* ===== Update masked word if guess exists; return 1 if guess found, 0 otherwise ===== */
int apply_guess(const char original[], char masked[], char guess) {
    int found = 0;
    size_t n = strlen(original);
    for (size_t i = 0; i < n; ++i) {
        if (tolower((unsigned char)original[i]) == tolower((unsigned char)guess)) {
            masked[i] = original[i]; /* reveal; preserve original case */
            found = 1;
        }
    }
    return found;
}

/* ===== Check if the word is fully guessed ===== */
int is_solved(const char masked[]) {
    for (size_t i = 0; i < strlen(masked); ++i) {
        if (masked[i] == '_') return 0;
    }
    return 1;
}

/* ===== Print current state: masked word and list of guessed letters ===== */
void display_state(const char masked[], int guessed[]) {
    printf("\nWord: ");
    for (size_t i = 0; i < strlen(masked); ++i) {
        printf("%c ", masked[i]);
    }
    printf("\n\nGuessed letters: ");
    int empty = 1;
    for (int i = 0; i < 26; ++i) {
        if (guessed[i]) {
            printf("%c ", 'a' + i);
            empty = 0;
        }
    }
    if (empty) printf("none");
    printf("\n");
}

/* ===== Read a guess from the user: single letter or full word option =====
   Returns: 'L' for letter guess (stores char at *out_ch), 'W' for word guess (stores in out_word),
            or 0 for invalid input.
*/
char get_player_input(char *out_ch, char out_word[], size_t out_word_len) {
    char line[256];
    if (!fgets(line, sizeof(line), stdin)) return 0;
    /* remove trailing newline */
    size_t len = strlen(line);
    while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
        line[--len] = '\0';
    }
    if (len == 0) return 0;

    /* if user typed more than one character, we treat as a word guess */
    if (len > 1) {
        /* copy at most out_word_len-1 chars */
        strncpy(out_word, line, out_word_len - 1);
        out_word[out_word_len - 1] = '\0';
        return 'W';
    } else {
        /* single character. validate it's a letter */
        char c = line[0];
        if (!isalpha((unsigned char)c)) return 0;
        *out_ch = c;
        return 'L';
    }
}

/* ===== Main game loop for one round ===== */
void play_round() {
    char secret[MAX_WORD_LEN];
    char masked[MAX_WORD_LEN];
    int guessed[26] = {0}; /* track letters a..z */
    int wrong = 0;
    char input_char = 0;
    char input_word[MAX_WORD_LEN];

    choose_random_word(secret, sizeof(secret));
    init_masked(secret, masked);

    printf("\n=== New game started! ===\n");
    /* Optional: for debugging you can print the secret:
       printf("DEBUG secret: %s\n", secret);
    */

    while (wrong < MAX_TRIES && !is_solved(masked)) {
        display_hangman(wrong);
        display_state(masked, guessed);
        printf("\nEnter a letter to guess, or type a whole word to guess the solution.\nYour input: ");
        fflush(stdout);

        char type = get_player_input(&input_char, input_word, sizeof(input_word));
        if (type == 0) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (type == 'W') {
            /* Full-word guess */
            if (strcasecmp(input_word, secret) == 0) {
                /* Win */
                strcpy(masked, secret);
                break;
            } else {
                wrong++;
                printf("Wrong word guess! You lose one try. (%d tries left)\n", MAX_TRIES - wrong);
                continue;
            }
        } else if (type == 'L') {
            /* Single letter guess */
            if (already_guessed(guessed, input_char)) {
                printf("You already guessed '%c'. Try another letter.\n", input_char);
                continue;
            }
            set_guessed(guessed, input_char);
            int found = apply_guess(secret, masked, input_char);
            if (found) {
                printf("Good guess! '%c' is in the word.\n", input_char);
            } else {
                wrong++;
                printf("Sorry, '%c' is NOT in the word. (%d tries left)\n", input_char, MAX_TRIES - wrong);
            }
        }
    }

    /* Final drawing and result */
    display_hangman(wrong);
    if (is_solved(masked)) {
        printf("\nCongratulations — you guessed the word: %s\n", secret);
    } else {
        printf("\nGame over. The secret word was: %s\n", secret);
    }
}

/* ===== main ===== */
int main(void) {
    char line[8];
    srand((unsigned int)time(NULL));

    printf("Welcome to Hangman (C)\n");
    printf("You have %d wrong guesses before the man is hanged.\n", MAX_TRIES);
    printf("You may guess a single letter or type a whole word to try to solve it.\n");

    while (1) {
        play_round();
        printf("\nPlay again? (y/n): ");
        if (!fgets(line, sizeof(line), stdin)) break;
        if (line[0] == 'y' || line[0] == 'Y') {
            continue;
        } else {
            break;
        }
    }

    printf("Thanks for playing. Goodbye!\n");
    return 0;
}
