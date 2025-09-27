#include <windows.h>
#include <stdio.h>
#include <ctype.h>

#define THREAD_COUNT 5

typedef struct {
    int threadNumber;
    int seconds;
} ThreadData;


DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    
    Sleep(data->seconds * 1000);
    

    printf("Thread %d completed (duration: %d sec)\n", 
           data->threadNumber, data->seconds);
    
    return 0;
}

void RunThreads() {
    HANDLE threads[THREAD_COUNT];
    ThreadData threadData[THREAD_COUNT];
    int seconds;
    
    printf("\nEnter duration for 5 threads:\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        do {
            printf("Thread %d (in seconds): ", i + 1);
            while (scanf("%d", &seconds) != 1 || seconds < 1) {
                printf("Error! Enter a number greater than 0: ");
                while (getchar() != '\n'); // Очистка буфера
            }
        } while (seconds < 1);
        
        threadData[i].threadNumber = i + 1;
        threadData[i].seconds = seconds;
    }
    
    printf("\nStarting threads...\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = CreateThread(NULL, 0, ThreadFunction, &threadData[i], 0, NULL);
        // Атрибут безопасности, размер стека, адрес функции потока, параметры потока, флаги создания, указатель на ID потока
    }
    
    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);
    
    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(threads[i]);
    }
    
    printf("\nAll threads completed!\n");
}

int main() {
    
    
    char choice;
    
    do {
        RunThreads();
        do {
            printf("\nChoose an action:\n");
            printf("r - restart\n");
            printf("x - exit program\n");
            printf("Your choice: ");
            scanf(" %c", &choice);
            choice = tolower(choice);
            
            if (choice != 'r' && choice != 'x') {
                printf("Error! Enter 'r' or 'x'\n");
            }
        } while (choice != 'r' && choice != 'x');
        
    } while (choice == 'r');
    
    
    printf("Program terminated.\n");
    return 0;
}
