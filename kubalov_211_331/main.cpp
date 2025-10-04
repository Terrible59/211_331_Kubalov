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
    std::cout << "3. Добавить новую запись" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Выберите действие: ";
}

void addRecordDialog(LibraryRecordManager& manager, const std::string& filename) {
    std::cin.ignore(); // Очищаем буфер после предыдущего ввода числа

    std::string bookTitle, readerCardNum;

    std::cout << "\n=== ДОБАВЛЕНИЕ НОВОЙ ЗАПИСИ ===" << std::endl;

    std::cout << "Введите название книги: ";
    std::getline(std::cin, bookTitle);

    if (bookTitle.empty()) {
        std::cout << "Ошибка: название книги не может быть пустым!" << std::endl;
        return;
    }

    std::cout << "Введите номер читательского билета: ";
    std::getline(std::cin, readerCardNum);

    if (readerCardNum.empty()) {
        std::cout << "Ошибка: номер билета не может быть пустым!" << std::endl;
        return;
    }

    if (manager.addRecord(bookTitle, readerCardNum)) {
        if (manager.saveToFile(filename)) {
            std::cout << "✓ Файл успешно обновлен" << std::endl;
        }
        else {
            std::cout << "⚠ Ошибка сохранения файла" << std::endl;
        }
    }
}

/**
 * Основная функция приложения
 */
int main() {
    // Настройка кодировки консоли для корректного отображения русского текста
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    LibraryRecordManager manager;
    //std::string filename = "C:\\Users\\power\\Desktop\\secure_dev\\kubalov_211_331\\Debug\\library_records_enc.txt";
    std::string filename = "C:\\Users\\power\\Desktop\\secure_dev\\kubalov_211_331\\Debug\\library_records_enc_corrupted.txt";

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

        case 3:
            addRecordDialog(manager, filename);
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