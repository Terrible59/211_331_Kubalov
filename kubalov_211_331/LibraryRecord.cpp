#include "LibraryRecord.h"
#include <fstream>
#include <iostream>
#include <sstream>


extern "C" {
    #include "aes.h"
    #include "sha256.h"
    #include "base64.h"
}

std::string LibraryRecordManager::getCurrentDateTime() {
    time_t now = time(0);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

    return std::string(buffer);
}

std::string LibraryRecordManager::calculateHash(const LibraryRecord& record) {
    std::string data = record.bookTitle + record.dateTime + record.readerCardNum;

    uint8_t hash[32];
    sha256_easy_hash(data.c_str(), data.length(), hash);

    char* base64_hash = base64_encode_binary(hash, 32);
    std::string result(base64_hash);
    free(base64_hash);

    return result;
}

bool LibraryRecordManager::addRecord(const std::string& bookTitle, const std::string& readerCardNum) {
    LibraryRecord newRecord;
    newRecord.bookTitle = bookTitle;
    newRecord.readerCardNum = readerCardNum;
    newRecord.dateTime = getCurrentDateTime(); 
    newRecord.hash = calculateHash(newRecord); 
    newRecord.isValid = true;

    records.push_back(newRecord);

    std::cout << "\n✓ Запись успешно добавлена!" << std::endl;
    std::cout << "  Дата и время: " << newRecord.dateTime << std::endl;
    std::cout << "  Хеш SHA-256: " << newRecord.hash << std::endl;

    return true;
}

void LibraryRecordManager::verifyRecords() {
    for (size_t i = 0; i < records.size(); i++) {
        std::string calculatedHash = calculateHash(records[i]);

        if (calculatedHash != records[i].hash) {
            records[i].isValid = false;

            records[i].hash.erase(records[i].hash.find_last_not_of(" \n\r\t") + 1);
            records[i].hash.erase(0, records[i].hash.find_first_not_of(" \n\r\t"));

            if (calculatedHash != records[i].hash) {
                std::cout << "\n⚠ ВНИМАНИЕ: Запись #" << (i + 1) << " повреждена!" << std::endl;
                std::cout << "  Ожидаемый хеш: " << records[i].hash << std::endl;
                std::cout << "  Вычисленный хеш: " << calculatedHash << std::endl;
            }
        }
    }
}

bool LibraryRecordManager::encryptAndSaveFile(const std::string& filename) {
    std::ostringstream oss;
    for (const auto& record : records) {
        oss << record.hash << "\n";
        oss << record.bookTitle << "\n";
        oss << record.dateTime << "\n";
        oss << record.readerCardNum << "\n";
    }

    std::string data = oss.str();

    size_t padding_len = 16 - (data.length() % 16);
    for (size_t i = 0; i < padding_len; i++) {
        data += (char)padding_len;
    }

    std::vector<unsigned char> buffer(data.begin(), data.end());

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, aes_key, aes_iv);

    AES_CBC_encrypt_buffer(&ctx, buffer.data(), buffer.size());

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
        return false;
    }

    file.write((char*)buffer.data(), buffer.size());
    file.close();

    return true;
}

bool LibraryRecordManager::saveToFile(const std::string& filename) {
    if (encryptAndSaveFile(filename)) {
        std::cout << "✓ Данные успешно сохранены и зашифрованы" << std::endl;
        return true;
    }
    return false;
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

    verifyRecords();

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