#ifndef IDATASTORE_H
#define IDATASTORE_H

#include "Book.h"
#include "Student.h"
#include <vector>
#include <map>
#include <string>

class IDataStore {
public:
    virtual ~IDataStore() = default;

    // Load/Save Life cycle
    virtual bool loadBooks(std::vector<Book>& books) = 0;
    virtual bool saveBooks(const std::vector<Book>& books) = 0;
    
    virtual bool loadStudents(std::map<std::string, Student>& students) = 0;
    virtual bool saveStudents(const std::map<std::string, Student>& students) = 0;
    
    virtual bool loadAdminPassword(std::string& passwordHash) = 0;
    virtual bool saveAdminPassword(const std::string& passwordHash) = 0;
};

#endif
