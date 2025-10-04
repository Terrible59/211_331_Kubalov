#include "LibraryRecord.h"
#include <iostream>
#include <string>
#include <windows.h>

/**
 * Отображает главное меню приложения
 */
void displayMenu() {
    std::cout << "\n=== СИСТЕМА УЧЕТА БИБЛИОТЕЧНЫХ ЗАПИСЕЙ ===" << std::endl;
    std::cout << "1. Данные - показать все записи" << std::endl;
    std::cout << "2. Показать количество записей" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Выберите действие: ";
}

/**
 * Основная функция приложения
 */
int main() {
    // Настройка кодировки консоли для корректного отображения русского текста
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    LibraryRecordManager manager;
    std::string filename = "C:\\Users\\power\\Desktop\\secure_dev\\kubalov_211_331\\Debug\\library_records.txt";

    std::cout << "Загрузка данных из файла: " << filename << std::endl;
    if (!manager.loadFromFile(filename)) {
        std::cout << "Не удалось загрузить данные. Проверьте наличие файла." << std::endl;
        return 1;
    }

    int choice = -1;
    while (choice != 0) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            manager.displayRecords();
            break;

        case 2:
            std::cout << "\nВсего записей: " << manager.getRecordCount() << std::endl;
            break;

        case 0:
            std::cout << "Выход из программы..." << std::endl;
            break;

        default:
            std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
            break;
        }
    }

    return 0;
}