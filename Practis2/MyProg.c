#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_COUNT 5

typedef struct {
    int threadNumber;
    int seconds;
} ThreadData;

// Мьютекс для синхронизации вывода в консоль
HANDLE consoleMutex;

// Функция потока
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    
    Sleep(data->seconds * 1000);
    
    // Синхронизированный вывод о завершении потока
    WaitForSingleObject(consoleMutex, INFINITE);
    printf("Thread %d completed (duration: %d sec)\n", 
           data->threadNumber, data->seconds);
    ReleaseMutex(consoleMutex);
    
    return 0;
}

// Функция для запуска потоков
void RunThreads() {
    HANDLE threads[THREAD_COUNT];
    ThreadData threadData[THREAD_COUNT];
    int seconds[THREAD_COUNT];
    
    // Запрос длительности для каждого потока
    printf("\nEnter duration for 5 threads:\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        printf("Thread %d (in seconds): ", i + 1);
        scanf("%d", &seconds[i]);
        
        // Подготовка данных для потока
        threadData[i].threadNumber = i + 1;
        threadData[i].seconds = seconds[i];
    }
    
    printf("\nStarting threads...\n");
    for (int i = 0; i < 5; i++) {
            threads[i] = CreateThread(NULL, 0, ThreadFunction, &threadData[i], 0, NULL);
            // Атрибут безопасности, размер стека, адрес функции потока, параметры потока, флаги создания, указатель на ID потока
    }
    
    // Ожидание завершения всех потоков
    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);
    
    // Закрытие хэндлов потоков
    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(threads[i]);
    }
    
    printf("\nAll threads completed!\n");
}

int main() {
    
    // Создание мьютекса для синхронизации вывода
    consoleMutex = CreateMutex(NULL, FALSE, NULL);
    
    char choice;
    
    do {
        RunThreads();
        
        printf("\nChoose an action:\n");
        printf("r - restart\n");
        printf("x - exit program\n");
        printf("Your choice: ");
        
        scanf(" %c", &choice);
        choice = tolower(choice);
        
        if (choice != 'r' && choice != 'x') {
            printf("Invalid choice. Use 'r' or 'x'\n");
        }
        
    } while (choice == 'r');
    
    // Закрытие мьютекса
    CloseHandle(consoleMutex);
    
    printf("Program terminated.\n");
    return 0;
}
