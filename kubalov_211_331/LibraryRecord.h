#pragma once
#include <string>
#include <vector>

/**
 * Структура для хранения библиотечной записи
 */
struct LibraryRecord {
    std::string hash;           
    std::string bookTitle;     
    std::string dateTime;
    std::string readerCardNum;
};

/**
 * Класс для работы с библиотечными записями
 */
class LibraryRecordManager {
private:
    std::vector<LibraryRecord> records;

public:
    /**
     * Загружает записи из файла
     * @param filename Имя файла с данными
     * @return true если успешно, false при ошибке
     */
    bool loadFromFile(const std::string& filename);

    /**
     * Отображает все записи в консоли
     */
    void displayRecords() const;

    /**
     * Возвращает количество загруженных записей
     */
    size_t getRecordCount() const { return records.size(); }
};