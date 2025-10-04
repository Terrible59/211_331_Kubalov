#include "LibraryRecord.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool LibraryRecordManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return false;
    }

    records.clear();
    std::string line;
    LibraryRecord currentRecord;
    int fieldIndex = 0;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        switch (fieldIndex) {
        case 0:
            currentRecord.hash = line;
            break;
        case 1:
            currentRecord.bookTitle = line;
            break;
        case 2:
            currentRecord.dateTime = line;
            break;
        case 3:
            currentRecord.readerCardNum = line;
            records.push_back(currentRecord);
            currentRecord = LibraryRecord();
            fieldIndex = -1;
            break;
        }
        fieldIndex++;
    }

    file.close();
    std::cout << "Загружено записей: " << records.size() << std::endl;
    return true;
}

void LibraryRecordManager::displayRecords() const {
    if (records.empty()) {
        std::cout << "Нет записей для отображения." << std::endl;
        return;
    }

    std::cout << "\n========== БИБЛИОТЕЧНЫЕ ЗАПИСИ ==========" << std::endl;
    for (size_t i = 0; i < records.size(); i++) {
        std::cout << "\nЗапись #" << (i + 1) << ":" << std::endl;
        std::cout << "  Хеш SHA-256 (base64): " << records[i].hash << std::endl;
        std::cout << "  Название книги: " << records[i].bookTitle << std::endl;
        std::cout << "  Дата и время: " << records[i].dateTime << std::endl;
        std::cout << "  Номер билета: " << records[i].readerCardNum << std::endl;
    }
    std::cout << "\n=========================================" << std::endl;
}