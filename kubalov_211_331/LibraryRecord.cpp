#include "LibraryRecord.h"
#include <fstream>
#include <iostream>
#include <sstream>


extern "C" {
    #include "aes.h"
}


std::string LibraryRecordManager::decryptFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return "";
    }

    std::vector<unsigned char> encrypted_data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();

    if (encrypted_data.empty()) {
        std::cerr << "Ошибка: файл пуст" << std::endl;
        return "";
    }

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, aes_key, aes_iv);

    AES_CBC_decrypt_buffer(&ctx, encrypted_data.data(), encrypted_data.size());


    size_t data_len = encrypted_data.size();
    if (data_len > 0) {
        unsigned char padding = encrypted_data[data_len - 1];
        if (padding > 0 && padding <= 16) {
            data_len -= padding;
        }
    }

    return std::string(encrypted_data.begin(), encrypted_data.begin() + data_len);
}


bool LibraryRecordManager::loadFromFile(const std::string& filename) {
    std::string decrypted_data = decryptFile(filename);
    if (decrypted_data.empty()) {
        std::cerr << "Ошибка расшифровки файла" << std::endl;
        return false;
    }

    records.clear();
    std::istringstream iss(decrypted_data);

    std::string line;
    LibraryRecord currentRecord;
    int fieldIndex = 0;

    while (std::getline(iss, line)) {
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