#include "FileStore.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <iomanip>
#include <ctime>
#include <sstream>

// Constructor
Library::Library(std::unique_ptr<IDataStore> store, const std::string& logFile)
    : activityLogFile(logFile), dataStore(std::move(store)) {
    
    // If no store provided, use FileStore by default
    if (!dataStore) {
        dataStore = std::make_unique<FileStore>();
    }
    
    loadData();
}

// Password hashing
size_t Library::hashPassword(const std::string& password) const {
    return std::hash<std::string>{}(password);
}

// Activity logging
void Library::logActivity(const std::string& action) const {
    std::ofstream logFile(activityLogFile, std::ios::app);
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        char buf[26];
        #ifdef _WIN32
            ctime_s(buf, sizeof(buf), &now);
        #else
            ctime_r(&now, buf);
        #endif
        std::string timestamp(buf);
        if (!timestamp.empty() && timestamp.back() == '\n') timestamp.pop_back();
        logFile << "[" << timestamp << "] " << action << std::endl;
        logFile.close();
    }
}

void Library::displayHeader(const std::string& title) const {
    clearScreen();
    std::cout << Colors::BOLD << Colors::BG_BLUE << Colors::WHITE << "  " << title << "  " << Colors::RESET << std::endl;
    std::cout << Colors::BLUE << std::string(title.length() + 4, '=') << Colors::RESET << "\n" << std::endl;
}

// Book management
bool Library::addBook(const std::string& title, const std::string& author, 
                      const std::string& isbn, int quantity, const std::string& genre) {
    // Check if book with same ISBN already exists
    if (findBookByIsbn(isbn) != nullptr) {
        std::cout << RED << "Error: Book with ISBN " << isbn << " already exists!" << RESET << std::endl;
        return false;
    }
    
    Book newBook(title, author, isbn, quantity, genre);
    books.push_back(newBook);
    std::cout << GREEN << "Success: Book added successfully! Book ID: " << BOLD << newBook.getBookId() << RESET << std::endl;
    logActivity("BOOK_ADDED: \"" + title + "\" by " + author + " (ID: " + std::to_string(newBook.getBookId()) + ")");
    saveData(); // Auto-save
    return true;
}

bool Library::deleteBook(int bookId) {
    auto it = std::find_if(books.begin(), books.end(), 
                          [bookId](const Book& book) { return book.getBookId() == bookId; });
    
    if (it != books.end()) {
        std::string title = it->getTitle();
        books.erase(it);
        std::cout << "Book deleted successfully!" << std::endl;
        logActivity("BOOK_DELETED: \"" + title + "\" (ID: " + std::to_string(bookId) + ")");
        saveData(); // Auto-save
        return true;
    }
    
    std::cout << "Book not found!" << std::endl;
    return false;
}

bool Library::updateBook(int bookId, const std::string& title, const std::string& author, 
                        const std::string& isbn, int quantity, const std::string& genre) {
    Book* book = findBook(bookId);
    if (book != nullptr) {
        book->setTitle(title);
        book->setAuthor(author);
        book->setIsbn(isbn);
        book->setQuantity(quantity);
        book->setGenre(genre);
        std::cout << "Book updated successfully!" << std::endl;
        logActivity("BOOK_UPDATED: \"" + title + "\" (ID: " + std::to_string(bookId) + ")");
        saveData(); // Auto-save
        return true;
    }
    
    std::cout << "Book not found!" << std::endl;
    return false;
}

Book* Library::findBook(int bookId) {
    for (auto& book : books) {
        if (book.getBookId() == bookId) {
            return &book;
        }
    }
    return nullptr;
}

Book* Library::findBookByIsbn(const std::string& isbn) {
    for (auto& book : books) {
        if (book.getIsbn() == isbn) {
            return &book;
        }
    }
    return nullptr;
}

std::vector<Book> Library::searchBooks(const std::string& query) {
    std::vector<Book> results;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    
    for (const auto& book : books) {
        std::string title = book.getTitle();
        std::string author = book.getAuthor();
        std::string isbn = book.getIsbn();
        std::string genre = book.getGenre();
        
        std::transform(title.begin(), title.end(), title.begin(), ::tolower);
        std::transform(author.begin(), author.end(), author.begin(), ::tolower);
        std::transform(isbn.begin(), isbn.end(), isbn.begin(), ::tolower);
        std::transform(genre.begin(), genre.end(), genre.begin(), ::tolower);
        
        if (title.find(lowerQuery) != std::string::npos ||
            author.find(lowerQuery) != std::string::npos ||
            isbn.find(lowerQuery) != std::string::npos ||
            genre.find(lowerQuery) != std::string::npos) {
            results.push_back(book);
        }
    }
    
    return results;
}

void Library::displayAllBooks() const {
    if (books.empty()) {
        std::cout << YELLOW << "\nNo books in the library." << RESET << std::endl;
        return;
    }
    
    displayHeader("LIBRARY COLLECTION");
    std::cout << BOLD << BLUE << std::left << std::setw(5) << "ID" 
              << std::setw(25) << "Title" 
              << std::setw(20) << "Author" 
              << std::setw(15) << "ISBN" 
              << std::setw(12) << "Genre"
              << std::setw(10) << "Available" << RESET << std::endl;
    std::cout << BLUE << std::string(87, '-') << RESET << std::endl;
    
    for (const auto& book : books) {
        book.displayShort();
    }
}

// Student management
bool Library::addStudent(const std::string& name, const std::string& password) {
    // Hash the password before storing
    size_t passHash = hashPassword(password);
    Student newStudent(name, std::to_string(passHash));
    students[newStudent.getStudentId()] = newStudent;
    std::cout << GREEN << "Success: Student registered successfully!" << RESET << std::endl;
    std::cout << CYAN << "Your Student ID: " << BOLD << newStudent.getStudentId() << RESET << std::endl;
    std::cout << CYAN << "Name: " << name << RESET << std::endl;
    std::cout << YELLOW << "Please save your Student ID for login!" << RESET << std::endl;
    logActivity("STUDENT_ADDED: " + name + " (ID: " + newStudent.getStudentId() + ")");
    saveData(); // Auto-save
    return true;
}

bool Library::deleteStudent(const std::string& studentId) {
    auto it = students.find(studentId);
    if (it != students.end()) {
        std::string name = it->second.getName();
        students.erase(it);
        std::cout << "Student deleted successfully!" << std::endl;
        logActivity("STUDENT_DELETED: " + name + " (ID: " + studentId + ")");
        saveData(); // Auto-save
        return true;
    }
    
    std::cout << "Student not found!" << std::endl;
    return false;
}

Student* Library::findStudent(const std::string& studentId) {
    auto it = students.find(studentId);
    if (it != students.end()) {
        return &(it->second);
    }
    return nullptr;
}

Student* Library::findStudentByName(const std::string& name) {
    for (auto& pair : students) {
        if (pair.second.getName() == name) {
            return &(pair.second);
        }
    }
    return nullptr;
}

Student* Library::authenticateStudent(const std::string& studentId, const std::string& password) {
    Student* student = findStudent(studentId);
    if (student != nullptr) {
        // Compare hashed password
        size_t inputHash = hashPassword(password);
        if (student->getPassword() == std::to_string(inputHash)) {
            return student;
        }
    }
    return nullptr;
}

bool Library::authenticateAdmin(const std::string& password) {
    size_t inputHash = hashPassword(password);
    return std::to_string(inputHash) == adminPasswordHash;
}

void Library::displayAllStudents() const {
    if (students.empty()) {
        std::cout << "\nNo students registered." << std::endl;
        return;
    }
    
    std::cout << "\n=== All Students ===" << std::endl;
    for (const auto& pair : students) {
        pair.second.display();
        std::cout << std::endl;
    }
}

// Password management
bool Library::changeAdminPassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!authenticateAdmin(oldPassword)) {
        std::cout << "Current password is incorrect!" << std::endl;
        return false;
    }
    
    if (newPassword.length() < 4) {
        std::cout << "New password must be at least 4 characters!" << std::endl;
        return false;
    }

    adminPasswordHash = std::to_string(hashPassword(newPassword));
    saveAdminConfig();
    std::cout << "Admin password changed successfully!" << std::endl;
    logActivity("ADMIN_PASSWORD_CHANGED");
    return true;
}

bool Library::changeStudentPassword(const std::string& studentId, const std::string& oldPassword, const std::string& newPassword) {
    Student* student = authenticateStudent(studentId, oldPassword);
    if (student == nullptr) {
        std::cout << "Current password is incorrect!" << std::endl;
        return false;
    }

    if (newPassword.length() < 4) {
        std::cout << "New password must be at least 4 characters!" << std::endl;
        return false;
    }

    student->setPassword(std::to_string(hashPassword(newPassword)));
    saveData();
    std::cout << "Password changed successfully!" << std::endl;
    logActivity("STUDENT_PASSWORD_CHANGED: " + student->getName() + " (ID: " + studentId + ")");
    return true;
}

// Book operations
bool Library::issueBook(const std::string& studentId, int bookId) {
    Student* student = findStudent(studentId);
    Book* book = findBook(bookId);
    
    if (student == nullptr) {
        std::cout << "Student not found!" << std::endl;
        return false;
    }
    
    if (book == nullptr) {
        std::cout << "Book not found!" << std::endl;
        return false;
    }
    
    if (!book->isAvailable()) {
        std::cout << "Book is not available!" << std::endl;
        return false;
    }
    
    if (student->hasBook(bookId)) {
        std::cout << "Student already has this book!" << std::endl;
        return false;
    }
    
    if (student->borrowBook(bookId, book->getTitle()) && book->issueBook()) {
        std::cout << "Book issued successfully!" << std::endl;
        std::cout << "Books borrowed: " << student->getActiveBorrowCount() 
                  << "/" << student->getMaxBorrowLimit() << std::endl;
        logActivity("BOOK_ISSUED: \"" + book->getTitle() + "\" to " + student->getName() + " (Student: " + studentId + ", Book: " + std::to_string(bookId) + ")");
        saveData(); // Auto-save
        return true;
    }
    
    std::cout << "Failed to issue book!" << std::endl;
    return false;
}

bool Library::returnBook(const std::string& studentId, int bookId) {
    Student* student = findStudent(studentId);
    Book* book = findBook(bookId);
    
    if (student == nullptr) {
        std::cout << "Student not found!" << std::endl;
        return false;
    }
    
    if (book == nullptr) {
        std::cout << "Book not found!" << std::endl;
        return false;
    }
    
    if (student->returnBook(bookId) && book->returnBook()) {
        std::cout << "Book returned successfully!" << std::endl;
        logActivity("BOOK_RETURNED: \"" + book->getTitle() + "\" by " + student->getName() + " (Student: " + studentId + ", Book: " + std::to_string(bookId) + ")");
        saveData(); // Auto-save
        return true;
    }
    
    std::cout << "Failed to return book!" << std::endl;
    return false;
}

void Library::displayIssuedBooks() const {
    std::cout << "\n=== Issued Books ===" << std::endl;
    bool hasIssuedBooks = false;
    
    for (const auto& pair : students) {
        const Student& student = pair.second;
        const auto& borrowedBooks = student.getBorrowedBooks();
        
        for (const auto& borrowedBook : borrowedBooks) {
            if (!borrowedBook.returned) {
                if (!hasIssuedBooks) {
                    std::cout << std::left << std::setw(15) << "Student ID" 
                              << std::setw(20) << "Student Name" 
                              << std::setw(8) << "Book ID" 
                              << std::setw(30) << "Book Title" << std::endl;
                    std::cout << std::string(73, '-') << std::endl;
                    hasIssuedBooks = true;
                }
                
                std::cout << std::left << std::setw(15) << student.getStudentId()
                          << std::setw(20) << student.getName()
                          << std::setw(8) << borrowedBook.bookId
                          << std::setw(30) << borrowedBook.bookTitle << std::endl;
            }
        }
    }
    
    if (!hasIssuedBooks) {
        std::cout << "No books are currently issued." << std::endl;
    }
}

// File operations
void Library::loadData() {
    loadAdminConfig();
    loadBooks();
    loadStudents();
}

void Library::saveData() {
    saveBooks();
    saveStudents();
}

void Library::loadBooks() {
    std::ifstream file(booksFile);
    if (file.is_open()) {
        books.clear();
        Book book;
        while (file.good()) {
            book.loadFromFile(file);
            if (book.getBookId() != 0) {
                books.push_back(book);
            }
        }
        file.close();
    }
}

void Library::saveBooks() {
    std::ofstream file(booksFile);
    if (dataStore) {
        dataStore->loadBooks(books);
        dataStore->loadStudents(students);
        dataStore->loadAdminPassword(adminPasswordHash);
    }
    // If adminPasswordHash is still empty after loading (e.g., first run or file missing)
    if (adminPasswordHash.empty()) {
        adminPasswordHash = std::to_string(hashPassword("admin123"));

void Library::saveAdminConfig() {
    std::ofstream file(adminConfigFile);
    if (file.is_open()) {
        file << adminPasswordHash << std::endl;
        file.close();
    }
}

// Input validation helpers
int Library::getValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        std::cout << "Invalid input! Please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string Library::getValidString(const std::string& prompt, bool allowSpaces) {
    std::string value;
    while (true) {
        std::cout << prompt;
        if (allowSpaces) {
            std::getline(std::cin, value);
        } else {
            std::cin >> value;
        }
        if (!value.empty()) {
            return value;
        }
        std::cout << "Input cannot be empty! Please try again." << std::endl;
    }
}

// Utility methods
void Library::displayMenu() const {
    displayHeader("LIBRARY MANAGEMENT SYSTEM");
    std::cout << Colors::CYAN << "  [1] " << Colors::WHITE << "Login as Administrator" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [2] " << Colors::WHITE << "Login as Student" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [3] " << Colors::WHITE << "Register as Student" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [4] " << Colors::WHITE << "Forgot Student ID?" << Colors::RESET << std::endl;
    std::cout << Colors::RED  << "  [5] " << Colors::WHITE << "Exit" << Colors::RESET << std::endl;
    std::cout << "\n" << Colors::MAGENTA << "Enter your choice: " << Colors::RESET;
}

void Library::displayAdminMenu() const {
    displayHeader("ADMINISTRATOR PANEL");
    std::cout << Colors::YELLOW << " --- Book Management ---" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [1]  " << Colors::WHITE << "Add Book" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [2]  " << Colors::WHITE << "Delete Book" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [3]  " << Colors::WHITE << "Update Book" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [4]  " << Colors::WHITE << "View All Books" << Colors::RESET << std::endl;
    
    std::cout << Colors::YELLOW << "\n --- Student Management ---" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [5]  " << Colors::WHITE << "Add Student" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [6]  " << Colors::WHITE << "Delete Student" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [7]  " << Colors::WHITE << "View All Students" << Colors::RESET << std::endl;
    
    std::cout << Colors::YELLOW << "\n --- System ---" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [8]  " << Colors::WHITE << "View Issued Books" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [9]  " << Colors::WHITE << "Change Admin Password" << Colors::RESET << std::endl;
    std::cout << Colors::RED  << "  [10] " << Colors::WHITE << "Logout" << Colors::RESET << std::endl;
    std::cout << "\n" << Colors::MAGENTA << "Enter your choice: " << Colors::RESET;
}

void Library::displayStudentMenu() const {
    displayHeader("STUDENT DASHBOARD");
    std::cout << Colors::YELLOW << " --- Books ---" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [1] " << Colors::WHITE << "Search Books" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [2] " << Colors::WHITE << "Issue Book" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [3] " << Colors::WHITE << "Return Book" << Colors::RESET << std::endl;
    
    std::cout << Colors::YELLOW << "\n --- My Account ---" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [4] " << Colors::WHITE << "View My Borrowed Books" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [5] " << Colors::WHITE << "View My Details" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "  [6] " << Colors::WHITE << "Change Password" << Colors::RESET << std::endl;
    std::cout << Colors::RED  << "  [7] " << Colors::WHITE << "Logout" << Colors::RESET << std::endl;
    std::cout << "\n" << Colors::MAGENTA << "Enter your choice: " << Colors::RESET;
}

void Library::clearScreen() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Library::pause() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
