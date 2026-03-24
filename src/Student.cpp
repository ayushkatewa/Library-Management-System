#include "Student.h"
#include "Colors.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <iostream> // Added for std::cout

// Initialize static member
int Student::nextStudentId = 1001;

// Constructors
Student::Student() : name(""), studentId(""), password("") {}

Student::Student(const std::string& name, const std::string& password)
    : name(name), password(password) {
    studentId = generateStudentId();
}

// Getters
std::string Student::getName() const { return name; }
std::string Student::getStudentId() const { return studentId; }
std::string Student::getPassword() const { return password; }
const std::vector<BorrowedBook>& Student::getBorrowedBooks() const { return borrowedBooks; }

// Setters
void Student::setName(const std::string& name) { this->name = name; }
void Student::setPassword(const std::string& password) { this->password = password; }

// Book operations
bool Student::borrowBook(int bookId, const std::string& bookTitle) {
    // Check borrowing limit
    if (getActiveBorrowCount() >= MAX_BORROW_LIMIT) {
        std::cout << "Borrowing limit reached! You can borrow a maximum of " 
                  << MAX_BORROW_LIMIT << " books at a time." << std::endl;
        return false;
    }

    // Check if student already has this book
    for (const auto& book : borrowedBooks) {
        if (book.bookId == bookId && !book.returned) {
            return false; // Already borrowed
        }
    }
    
    BorrowedBook newBook;
    newBook.bookId = bookId;
    newBook.bookTitle = bookTitle;
    newBook.borrowDate = std::time(nullptr);
    newBook.dueDate = newBook.borrowDate + (14 * 24 * 60 * 60); // 14 days from now
    newBook.returned = false;
    newBook.fine = 0.0;
    
    borrowedBooks.push_back(newBook);
    return true;
}

bool Student::returnBook(int bookId) {
    for (auto& book : borrowedBooks) {
        if (book.bookId == bookId && !book.returned) {
            book.returned = true;
            // Calculate fine for this specific book
            std::time_t currentTime = std::time(nullptr);
            if (currentTime > book.dueDate) {
                double daysOverdue = std::difftime(currentTime, book.dueDate) / (24 * 60 * 60);
                book.fine = daysOverdue * 0.50;
            } else {
                book.fine = 0.0;
            }
            return true;
        }
    }
    return false;
}

bool Student::hasBook(int bookId) const {
    for (const auto& book : borrowedBooks) {
        if (book.bookId == bookId && !book.returned) {
            return true;
        }
    }
    return false;
}

int Student::getActiveBorrowCount() const {
    int count = 0;
    for (const auto& book : borrowedBooks) {
        if (!book.returned) count++;
    }
    return count;
}

int Student::getMaxBorrowLimit() const {
    return MAX_BORROW_LIMIT;
}

// Fine calculation
double Student::calculateFines() const {
    double totalFine = 0.0;
    std::time_t currentTime = std::time(nullptr);
    
    for (const auto& book : borrowedBooks) {
        if (!book.returned && currentTime > book.dueDate) {
            // Calculate days overdue
            double daysOverdue = std::difftime(currentTime, book.dueDate) / (24 * 60 * 60);
            totalFine += daysOverdue * 0.50; // $0.50 per day
        }
    }
    
    return totalFine;
}

void Student::updateFines() {
    for (auto& book : borrowedBooks) {
        if (!book.returned) {
            book.fine = 0.0;
            std::time_t currentTime = std::time(nullptr);
            if (currentTime > book.dueDate) {
                double daysOverdue = std::difftime(currentTime, book.dueDate) / (24 * 60 * 60);
                book.fine = daysOverdue * 0.50;
            }
        }
    }
}

// Display methods
void Student::display() const {
    std::cout << Colors::BOLD << Colors::MAGENTA << "\n=== Student Details ===" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "Name: " << Colors::WHITE << name << std::endl;
    std::cout << Colors::CYAN << "Student ID: " << Colors::WHITE << studentId << std::endl;
    std::cout << Colors::CYAN << "Books Borrowed: " << Colors::WHITE << getActiveBorrowCount() << "/" << MAX_BORROW_LIMIT << " (limit)" << std::endl;
    std::cout << Colors::CYAN << "Current Fine: $" << Colors::WHITE << std::fixed << std::setprecision(2) << calculateFines() << Colors::RESET << std::endl;
}

void Student::displayBorrowedBooks() const {
    if (borrowedBooks.empty()) {
        std::cout << Colors::YELLOW << "\nNo books currently borrowed." << Colors::RESET << std::endl;
        return;
    }
    
    std::cout << Colors::BOLD << Colors::BLUE << "\n=== Borrowed Books ===" << Colors::RESET << std::endl;
    std::cout << Colors::BLUE << std::left << std::setw(5) << "ID" 
              << std::setw(30) << "Title" 
              << std::setw(25) << "Borrow Date" 
              << std::setw(25) << "Due Date" 
              << std::setw(10) << "Status" 
              << std::setw(10) << "Fine" << Colors::RESET << std::endl;
    std::cout << Colors::BLUE << std::string(105, '-') << Colors::RESET << std::endl;
    
    for (const auto& book : borrowedBooks) {
        std::cout << Colors::CYAN << std::left << std::setw(5) << book.bookId 
                  << Colors::WHITE << std::setw(30) << book.bookTitle;
        
        // Format dates
        char borrowBuf[26], dueBuf[26];
        std::time_t borrow = book.borrowDate;
        std::time_t due = book.dueDate;
        
        #ifdef _WIN32
            ctime_s(borrowBuf, sizeof(borrowBuf), &borrow);
            ctime_s(dueBuf, sizeof(dueBuf), &due);
        #else
            ctime_r(&borrow, borrowBuf);
            ctime_r(&due, dueBuf);
        #endif

        std::string borrowDate(borrowBuf);
        borrowDate.pop_back(); // Remove trailing newline
        std::string dueDate(dueBuf);
        dueDate.pop_back();
        
        std::cout << std::setw(25) << borrowDate;
        std::cout << std::setw(25) << dueDate;
        
        std::cout << std::setw(10) << (book.returned ? "Returned" : "Borrowed");
        std::cout << "$" << std::fixed << std::setprecision(2) << book.fine << std::endl;
    }
}

// File operations
void Student::saveToFile(std::ofstream& file) const {
    file << studentId << "|" << name << "|" << password << "|" << borrowedBooks.size() << std::endl;
    
    for (const auto& book : borrowedBooks) {
        file << book.bookId << "|" << book.bookTitle << "|" 
             << book.borrowDate << "|" << book.dueDate << "|" 
             << book.returned << "|" << book.fine << std::endl;
    }
}

void Student::loadFromFile(std::ifstream& file) {
    std::string line;
    borrowedBooks.clear();
    studentId = "";

    if (std::getline(file, line)) {
        if (line.empty()) return;

        try {
            size_t pos = 0;
            
            // Parse studentId
            pos = line.find('|');
            if (pos == std::string::npos) return;
            studentId = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Update nextStudentId to avoid collisions
            if (studentId.length() > 3) {
                try {
                    int idNum = std::stoi(studentId.substr(3));
                    if (idNum >= nextStudentId) nextStudentId = idNum + 1;
                } catch (...) {}
            }
            
            // Parse name
            pos = line.find('|');
            if (pos == std::string::npos) { studentId = ""; return; }
            name = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Parse password
            pos = line.find('|');
            if (pos == std::string::npos) { studentId = ""; return; }
            password = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Parse number of borrowed books
            int numBooks = std::stoi(line);
            
            // Load borrowed books
            for (int i = 0; i < numBooks; i++) {
                if (std::getline(file, line)) {
                    BorrowedBook book;
                    
                    // Parse bookId
                    pos = line.find('|');
                    if (pos == std::string::npos) continue;
                    book.bookId = std::stoi(line.substr(0, pos));
                    line.erase(0, pos + 1);
                    
                    // Parse bookTitle
                    pos = line.find('|');
                    if (pos == std::string::npos) continue;
                    book.bookTitle = line.substr(0, pos);
                    line.erase(0, pos + 1);
                    
                    // Parse borrowDate
                    pos = line.find('|');
                    if (pos == std::string::npos) continue;
                    book.borrowDate = std::stol(line.substr(0, pos));
                    line.erase(0, pos + 1);
                    
                    // Parse dueDate
                    pos = line.find('|');
                    if (pos == std::string::npos) continue;
                    book.dueDate = std::stol(line.substr(0, pos));
                    line.erase(0, pos + 1);
                    
                    // Parse returned
                    pos = line.find('|');
                    if (pos == std::string::npos) continue;
                    book.returned = (line.substr(0, pos) == "1");
                    line.erase(0, pos + 1);
                    
                    // Parse fine
                    book.fine = std::stod(line);
                    
                    borrowedBooks.push_back(book);
                }
            }
        } catch (const std::exception&) {
            studentId = ""; // Mark as invalid on parse error
        }
    }
}

// Utility methods
std::string Student::generateStudentId() {
    std::ostringstream oss;
    oss << "STU" << std::setfill('0') << std::setw(4) << nextStudentId++;
    return oss.str();
}
