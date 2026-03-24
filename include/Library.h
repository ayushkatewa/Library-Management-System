#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Student.h"
#include "Colors.h"
#include "IDataStore.h"
#include <vector>
#include <map>
#include <string>
#include <memory>

class Library {
private:
    std::vector<Book> books;
    std::map<std::string, Student> students;
    std::string adminPasswordHash;
    std::string activityLogFile;
    
    std::unique_ptr<IDataStore> dataStore;

    // Utility
    size_t hashPassword(const std::string& password) const;
    void logActivity(const std::string& action) const;
    void displayHeader(const std::string& title) const;

public:
    // Constructor
    Library(std::unique_ptr<IDataStore> store = nullptr,
            const std::string& activityLogFile = "data/activity_log.txt");
    
    // Book management
    bool addBook(const std::string& title, const std::string& author, 
                 const std::string& isbn, int quantity, const std::string& genre = "General");
    bool deleteBook(int bookId);
    bool updateBook(int bookId, const std::string& title, const std::string& author, 
                   const std::string& isbn, int quantity, const std::string& genre = "General");
    Book* findBook(int bookId);
    Book* findBookByIsbn(const std::string& isbn);
    std::vector<Book> searchBooks(const std::string& query);
    void displayAllBooks() const;
    
    // Student management
    bool addStudent(const std::string& name, const std::string& password);
    bool deleteStudent(const std::string& studentId);
    Student* findStudent(const std::string& studentId);
    Student* findStudentByName(const std::string& name);
    Student* authenticateStudent(const std::string& studentId, const std::string& password);
    bool authenticateAdmin(const std::string& password);
    void displayAllStudents() const;
    
    // Password management
    bool changeAdminPassword(const std::string& oldPassword, const std::string& newPassword);
    bool changeStudentPassword(const std::string& studentId, const std::string& oldPassword, const std::string& newPassword);
    
    // Book operations
    bool issueBook(const std::string& studentId, int bookId);
    bool returnBook(const std::string& studentId, int bookId);
    void displayIssuedBooks() const;
    
    // File operations
    void loadData();
    void saveData();
    
    // Input helpers
    static int getValidInt(const std::string& prompt);
    static std::string getValidString(const std::string& prompt, bool allowSpaces = true);
    
    // Utility methods
    void displayMenu() const;
    void displayAdminMenu() const;
    void displayStudentMenu() const;
    void clearScreen() const;
    void pause() const;
};

#endif
