#include "FileStore.h"
#include "Book.h"
#include "Student.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

FileStore::FileStore(const std::string& b, const std::string& s, const std::string& a)
    : booksFile(b), studentsFile(s), adminConfigFile(a) {}

bool FileStore::loadBooks(std::vector<Book>& books) {
    std::ifstream file(booksFile);
    if (!file.is_open()) return false;
    
    books.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            books.push_back(Book::loadFromFile(line));
        } catch (...) {
            continue;
        }
    }
    file.close();
    return true;
}

bool FileStore::saveBooks(const std::vector<Book>& books) {
    std::ofstream file(booksFile);
    if (!file.is_open()) return false;
    
    for (const auto& book : books) {
        file << book.saveToFile() << std::endl;
    }
    file.close();
    return true;
}

bool FileStore::loadStudents(std::map<std::string, Student>& students) {
    std::ifstream file(studentsFile);
    if (!file.is_open()) return false;
    
    students.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            Student s = Student::loadFromFile(line);
            students[s.getStudentId()] = s;
        } catch (...) {
            continue;
        }
    }
    file.close();
    return true;
}

bool FileStore::saveStudents(const std::map<std::string, Student>& students) {
    std::ofstream file(studentsFile);
    if (!file.is_open()) return false;
    
    for (const auto& pair : students) {
        file << pair.second.saveToFile() << std::endl;
    }
    file.close();
    return true;
}

bool FileStore::loadAdminPassword(std::string& passwordHash) {
    std::ifstream file(adminConfigFile);
    if (!file.is_open()) return false;
    std::getline(file, passwordHash);
    file.close();
    return true;
}

bool FileStore::saveAdminPassword(const std::string& passwordHash) {
    std::ofstream file(adminConfigFile);
    if (!file.is_open()) return false;
    file << passwordHash;
    file.close();
    return true;
}
