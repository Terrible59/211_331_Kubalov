#include "LibraryRecord.h"
#include <iostream>
#include <string>
#include <windows.h>

/**
 * ���������� ������� ���� ����������
 */
void displayMenu() {
    std::cout << "\n=== ������� ����� ������������ ������� ===" << std::endl;
    std::cout << "1. ������ - �������� ��� ������" << std::endl;
    std::cout << "2. �������� ���������� �������" << std::endl;
    std::cout << "0. �����" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "�������� ��������: ";
}

/**
 * �������� ������� ����������
 */
int main() {
    // ��������� ��������� ������� ��� ����������� ����������� �������� ������
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    LibraryRecordManager manager;
    std::string filename = "C:\\Users\\power\\Desktop\\secure_dev\\kubalov_211_331\\Debug\\library_records.txt";

    std::cout << "�������� ������ �� �����: " << filename << std::endl;
    if (!manager.loadFromFile(filename)) {
        std::cout << "�� ������� ��������� ������. ��������� ������� �����." << std::endl;
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
            std::cout << "\n����� �������: " << manager.getRecordCount() << std::endl;
            break;

        case 0:
            std::cout << "����� �� ���������..." << std::endl;
            break;

        default:
            std::cout << "�������� �����. ���������� �����." << std::endl;
            break;
        }
    }

    return 0;
}