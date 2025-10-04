#include "LibraryRecord.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool LibraryRecordManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "������: �� ������� ������� ���� " << filename << std::endl;
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
    std::cout << "��������� �������: " << records.size() << std::endl;
    return true;
}

void LibraryRecordManager::displayRecords() const {
    if (records.empty()) {
        std::cout << "��� ������� ��� �����������." << std::endl;
        return;
    }

    std::cout << "\n========== ������������ ������ ==========" << std::endl;
    for (size_t i = 0; i < records.size(); i++) {
        std::cout << "\n������ #" << (i + 1) << ":" << std::endl;
        std::cout << "  ��� SHA-256 (base64): " << records[i].hash << std::endl;
        std::cout << "  �������� �����: " << records[i].bookTitle << std::endl;
        std::cout << "  ���� � �����: " << records[i].dateTime << std::endl;
        std::cout << "  ����� ������: " << records[i].readerCardNum << std::endl;
    }
    std::cout << "\n=========================================" << std::endl;
}