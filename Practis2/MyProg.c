#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <locale.h>

#define THREAD_COUNT 5

typedef struct {
    int threadNumber;
    int seconds;
} ThreadData;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    
    Sleep(data->seconds * 1000);
    
    printf("Поток %d завершен (длительность: %d сек)\n", 
           data->threadNumber, data->seconds);
    
    return 0;
}

void RunThreads() {
    HANDLE threads[THREAD_COUNT];
    ThreadData threadData[THREAD_COUNT];
    int seconds;
    int result;
    
    printf("\nВведите длительность для 5 потоков:\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        do {
            printf("Поток %d (в секундах): ", i + 1);
            result = scanf("%d", &seconds);
            
            if (result != 1) {
                printf("Ошибка! Вводите только числа. Попробуйте снова.\n");
                while (getchar() != '\n'); // Очистка буфера от некорректного ввода
                continue;
            }
            
            if (seconds < 1) {
                printf("Ошибка! Число должно быть больше 0.\n");
            }
            
        } while (result != 1 || seconds < 1);
        
        // Подготовка данных для потока
        threadData[i].threadNumber = i + 1;
        threadData[i].seconds = seconds;
    }
    
    printf("\nЗапуск потоков...\n");
    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = CreateThread(NULL, 0, ThreadFunction, &threadData[i], 0, NULL);
        // Атрибут безопасности, размер стека, адрес функции потока, параметры потока, флаги создания, указатель на ID потока
    }
    
    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE);
    
    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(threads[i]);
    }
    
    printf("\nВсе потоки завершены!\n");
}

int main() {
    setlocale(LC_ALL, "Russian");
    char choice;
    int result;
    
    do {
        RunThreads();
        
        do {
            printf("\nВыберите действие:\n");
            printf("r - перезапустить\n");
            printf("x - выйти из программы\n");
            printf("Ваш выбор: ");
            result = scanf(" %c", &choice);
            
            if (result != 1) {
                printf("Ошибка ввода! Попробуйте снова.\n");
                while (getchar() != '\n'); // Очистка буфера
                continue;
            }
            
            choice = tolower(choice);
            
            if (choice != 'r' && choice != 'x') {
                printf("Ошибка! Введите 'r' или 'x'\n");
            }
            
        } while (result != 1 || (choice != 'r' && choice != 'x'));
        
    } while (choice == 'r');
    
    printf("Программа завершена.\n");
    return 0;
}
