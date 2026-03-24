#ifndef FILESTORE_H
#define FILESTORE_H

#include "IDataStore.h"
#include "Book.h"
#include "Student.h"
#include <vector>
#include <map>
#include <string>
#include <memory>

class FileStore : public IDataStore {
private:
    std::string booksFile;
    std::string studentsFile;
    std::string adminConfigFile;

public:
    FileStore(const std::string& booksFile = "data/books.txt",
              const std::string& studentsFile = "data/students.txt",
              const std::string& adminConfigFile = "data/admin_config.txt");

    // IDataStore implementation
    bool loadBooks(std::vector<Book>& books) override;
    bool saveBooks(const std::vector<Book>& books) override;
    
    bool loadStudents(std::map<std::string, Student>& students) override;
    bool saveStudents(const std::map<std::string, Student>& students) override;
    
    bool loadAdminPassword(std::string& passwordHash) override;
    bool saveAdminPassword(const std::string& passwordHash) override;
};

#endif
