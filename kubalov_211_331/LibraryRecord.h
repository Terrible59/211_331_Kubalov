#pragma once
#include <string>
#include <vector>

/**
 * ��������� ��� �������� ������������ ������
 */
struct LibraryRecord {
    std::string hash;           
    std::string bookTitle;     
    std::string dateTime;
    std::string readerCardNum;
    bool isValid = true;
};

/**
 * ����� ��� ������ � ������������� ��������
 */
class LibraryRecordManager {
private:
    std::vector<LibraryRecord> records;

    unsigned char aes_key[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };

    unsigned char aes_iv[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };

    /**
     * ��������� ����
     * @param filename ��� ����� � �������
     * @return ������ �����
     */
    std::string decryptFile(const std::string& filename);

    /**
     * ��������� ��� ������
     * @param record ������
     * @return ��� � base64
     */
    std::string calculateHash(const LibraryRecord& record);

    void verifyRecords();

public:
    /**
     * ��������� ������ �� �����
     * @param filename ��� ����� � �������
     * @return true ���� �������, false ��� ������
     */
    bool loadFromFile(const std::string& filename);

    /**
     * ���������� ��� ������ � �������
     */
    void displayRecords() const;

    /**
     * ���������� ���������� ����������� �������
     */
    size_t getRecordCount() const { return records.size(); }
};