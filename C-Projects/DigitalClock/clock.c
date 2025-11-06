#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void fill_time(char *, int);
void fill_date(char *);
int input_format();
void clear_screen();

int main()
{

    char time[50], date[100];

    int format_choice = input_format();

    while(1){
        fill_time(time, format_choice);
        fill_date(date);

        

        printf("\rCurrent Time: %s", time);
        printf(" | Current Date: %s", date);

        sleep(1);
    }

    return 0;
}

int input_format()
{
    int format_choice;
    printf("\n\nDigital Clock\n\n");
    printf("Choose the time format: ");
    printf("\n1. 12-hour format (HH:MM:SS AM/PM)");
    printf("\n2. 24-hour format (HH:MM:SS)");
    printf("\nEnter your choice (1 or 2): ");
    scanf("%d", &format_choice);
    return format_choice;
}

void fill_time(char *buffer, int format)
{

    time_t raw_time;
    struct tm *current_time;

    time(&raw_time);
    current_time = localtime(&raw_time);

    if (format == 2)
        strftime(buffer, 50, "%H:%M:%S", current_time);

    else
    {
        strftime(buffer, 50, "%I:%M:%S %p", current_time);
    }
}
void fill_date(char *buffer)
{
    time_t raw_time;
    struct tm *current_time;

    time(&raw_time);
    current_time = localtime(&raw_time);

    strftime(buffer, 100, "%A %B %d %Y", current_time);
}
void clear_screen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}