#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

// Функция для работы потока
DWORD WINAPI threadFunction(LPVOID lpParam) {
    // Получаем параметры потока
    int* params = (int*)lpParam;
    int threadNumber = params[0];
    int duration = params[1];
    
    // Сообщение о начале работы потока
    printf("Thread %d started\n", threadNumber);
    
    // Симуляция работы потока (приостановка)
    Sleep(duration * 1000); // Sleep принимает миллисекунды, поэтому умножаем на 1000
    
    // Сообщение о завершении работы потока
    printf("Thread %d completed task\n", threadNumber);
    return 0;
}

int main() {
    setlocale(LC_ALL, "English");
    
    // Цикл для перезапуска
    while (1) {
        // Запуск пяти потоков
        HANDLE threads[5];
        int params[5][2]; // Массив параметров для каждого потока (номер и продолжительность)
        
        for (int i = 0; i < 5; i++) {
            // Ввод пользователем продолжительности работы потока
            int duration;
            printf("Enter duration for thread %d (in seconds): ", i + 1);
            scanf_s("%d", &duration);
            
            // Устанавливаем параметры для потока
            params[i][0] = i + 1; // Номер потока
            params[i][1] = duration; // Продолжительность
        }
        
        // Создание и запуск потоков
        for (int i = 0; i < 5; i++) {
            threads[i] = CreateThread(NULL, 0, threadFunction, &params[i], 0, NULL);
            if (threads[i] == NULL) {
                printf("Error creating thread %d\n", i + 1);
                return 1;
            }
        }
        
        // Ожидание завершения всех потоков
        WaitForMultipleObjects(5, threads, TRUE, INFINITE);
        
        // Закрытие дескрипторов потоков
        for (int i = 0; i < 5; i++) {
            CloseHandle(threads[i]);
        }
        
        // Запрос на перезапуск или выход
        char choice;
        printf("Press x to exit, press r to restart: ");
        scanf_s(" %c", &choice); // Пробел перед %c нужен для корректной обработки ввода
        
        // Выход из программы
        if (choice == 'x') {
            break;
        }
    }
    return 0;
}
