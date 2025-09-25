#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>

// Структура для передачи параметров в поток
typedef struct {
    int threadNumber;
    int duration;
} ThreadParameters;

// Функция выполнения потока
DWORD WINAPI threadFunction(LPVOID lpParam) {
    ThreadParameters* params = (ThreadParameters*)lpParam;
    
    printf("Thread %d started work\n", params->threadNumber);
    
    // Симуляция работы потока
    Sleep(params->duration * 1000);
    
    printf("Thread %d completed task\n", params->threadNumber);
    
    return 0;
}

int main() {
    
    char userChoice = 'r';
    
    // Основной цикл программы
    while (userChoice != 'x') {
        
        // Массивы для дескрипторов потоков и их параметров
        HANDLE threadHandles[5];
        ThreadParameters threadParams[5];
        
        // Ввод длительности работы каждого потока
        printf("Enter duration for each thread:\n");
        for (int i = 0; i < 5; i++) {
            printf("Thread %d duration (seconds): ", i + 1);
            scanf("%d", &threadParams[i].duration);
            threadParams[i].threadNumber = i + 1;
        }
        
        printf("\nCreating and launching threads\n");
        
        // Создание и запуск потоков
        for (int i = 0; i < 5; i++) {
            threadHandles[i] = CreateThread(NULL, 0, threadFunction, &threadParams[i], 0, NULL);
            // Атрибут безопасности, размер стека, адрес функции потока, параметры потока, флаги создания, указатель на ID потока

            if (threadHandles[i] == NULL) {
                printf("Error creating thread %d. Error code: %d\n", 
                    i + 1, (int)GetLastError());

                for (int j = 0; j < i; j++) {
                    CloseHandle(threadHandles[j]);
                }
                return 1;
            }
        }
        
        
        // Ожидание завершения всех потоков одновременно
        WaitForMultipleObjects(5, threadHandles, TRUE, INFINITE);
        
        printf("\nAll threads have finished execution\n");
        
        // Освобождение ресурсов - закрытие дескрипторов потоков
        for (int i = 0; i < 5; i++) {
            CloseHandle(threadHandles[i]);
        }

        do {
            printf("\nPress x to exit, press r to restart: ");
            scanf(" %c", &userChoice);
            userChoice = tolower(userChoice);
            
            // Проверка корректности ввода
            if (userChoice != 'x' && userChoice != 'r') {
                printf("Invalid input! Please enter only 'x' to exit or 'r' to restart.\n");
            }
            
        } while (userChoice != 'x' && userChoice != 'r');
        
        printf("\n");

    }
    
    printf("Program terminated successfully.\n");
    return 0;
}
