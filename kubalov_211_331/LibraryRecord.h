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
};

/**
 * ����� ��� ������ � ������������� ��������
 */
class LibraryRecordManager {
private:
    std::vector<LibraryRecord> records;

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