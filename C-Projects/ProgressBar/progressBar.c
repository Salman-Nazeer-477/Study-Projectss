#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

const int BAR_LENGTH = 50;
const int MAX_TASKS = 5;

typedef struct {
    int id;
    int progress;
    int step;
} Task;

void print_bar(Task task);
void clear_screen();

int main()
{
    int num_tasks;
    printf("This is a progress bar demonstration program.\n\n");
    printf("Enter the number of tasks you need to track (max %d): ", MAX_TASKS);
    if(scanf("%d", &num_tasks) != 1 || num_tasks < 1 || num_tasks > MAX_TASKS) {
        printf("Invalid number of tasks. Please enter a number between 1 and %d.\n", MAX_TASKS);
        return 1;
    }
    
    srand(time(NULL)); // Seed for random step sizes

    Task tasks[num_tasks];

    for(int i = 0; i < num_tasks; i++)
    {
        tasks[i].id = i + 1;
        tasks[i].progress = 0;
        tasks[i].step = rand() % 10 +1; // Different step sizes for each task
    }

    int tasks_incomplete = 1;
    while(tasks_incomplete){
        tasks_incomplete = 0;
        clear_screen();
        for(int i = 0; i < num_tasks; i++)
    {
        tasks[i].progress += tasks[i].step;
        if(tasks[i].progress > 100){
            tasks[i].progress = 100;
        }else if(tasks[i].progress < 100){
            tasks_incomplete = 1;
        }
        
        print_bar(tasks[i]);
    }
        sleep(1); //Sleep for 1 second.
    }
    printf("All tasks completed!\nThankyou for using the progress bar demo program.\n");

    return 0;
}

void print_bar(Task task)
{
    int bars_to_show = (task.progress * BAR_LENGTH) / 100;

    printf("Task %d: [", task.id);

    for (int i = 0; i < BAR_LENGTH; i++)
    {
        if (i < bars_to_show)
        {
            printf("#");
        }
        else
        {
            printf(" ");
        }
    }

    printf("] %d%%\n", task.progress);
}
void clear_screen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}