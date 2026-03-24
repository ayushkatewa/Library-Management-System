#include "Library.h"
#include "FileStore.h"
#include "Colors.h"
#include <iostream>
#include <string>
#include <memory>

int main() {
    // Inject the persistence engine (FileStore for now)
    auto store = std::make_unique<FileStore>("data/books.txt", "data/students.txt", "data/admin_config.txt");
    Library library(std::move(store));
    
    std::cout << Colors::BOLD << Colors::BLUE << "\n--- Welcome to the Modern Library System ---" << Colors::RESET << std::endl;
    
    while (true) {
        library.displayMenu();
        int choice = Library::getValidInt("");

        switch (choice) {
            case 1: { // Login as Administrator
                std::string password;
                std::cout << "Enter admin password: ";
                std::cin >> password;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (library.authenticateAdmin(password)) {
                    std::cout << Colors::GREEN << "Login successful!" << Colors::RESET << std::endl;
                    handleAdminMenu(library);
                } else {
                    std::cout << Colors::RED << "Invalid password!" << Colors::RESET << std::endl;
                    library.pause();
                }
                break;
            }
            
            case 2: { // Login as Student
                std::string studentId, password;
                std::cout << "Enter Student ID: ";
                std::cin >> studentId;
                std::cout << "Enter password: ";
                std::cin >> password;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                Student* student = library.authenticateStudent(studentId, password);
                if (student != nullptr) {
                    std::cout << Colors::GREEN << "Login successful! Welcome, " << Colors::BOLD << student->getName() << Colors::RESET << std::endl;
                    handleStudentMenu(library, student);
                } else {
                    std::cout << Colors::RED << "Invalid credentials!" << Colors::RESET << std::endl;
                    library.pause();
                }
                break;
            }
            
            case 3: // Register as Student
                handleStudentRegistration(library);
                break;
                
            case 4: // Forgot Student ID
                handleForgotStudentId(library);
                break;
                
            case 5: // Exit
                std::cout << "Thank you for using Library Management System!" << std::endl;
                library.saveData();
                return 0;
                
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
                library.pause();
        }
        
        library.clearScreen();
    }
    
    return 0;
}

void handleAdminMenu(Library& library) {
    while (true) {
        library.displayAdminMenu();
        int choice = Library::getValidInt("");
        
        switch (choice) {
            case 1: { // Add Book
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string title = Library::getValidString("Enter book title: ");
                std::string author = Library::getValidString("Enter author name: ");
                std::string isbn = Library::getValidString("Enter ISBN: ");
                std::string genre = Library::getValidString("Enter genre (e.g. Fiction, Science, History): ");
                int quantity = Library::getValidInt("Enter quantity: ");
                
                library.addBook(title, author, isbn, quantity, genre);
                library.pause();
                break;
            }
            
            case 2: { // Delete Book
                int bookId = Library::getValidInt("Enter book ID to delete: ");
                library.deleteBook(bookId);
                library.pause();
                break;
            }
            
            case 3: { // Update Book
                int bookId = Library::getValidInt("Enter book ID to update: ");
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string title = Library::getValidString("Enter new title: ");
                std::string author = Library::getValidString("Enter new author: ");
                std::string isbn = Library::getValidString("Enter new ISBN: ");
                std::string genre = Library::getValidString("Enter new genre: ");
                int quantity = Library::getValidInt("Enter new quantity: ");
                
                library.updateBook(bookId, title, author, isbn, quantity, genre);
                library.pause();
                break;
            }
            
            case 4: // View All Books
                library.displayAllBooks();
                library.pause();
                break;
                
            case 5: { // Add Student
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string name = Library::getValidString("Enter student name: ");
                std::string password = Library::getValidString("Enter password (min 4 chars): ");
                
                while (password.length() < 4) {
                    std::cout << "Password must be at least 4 characters!" << std::endl;
                    password = Library::getValidString("Enter password (min 4 chars): ");
                }
                
                library.addStudent(name, password);
                library.pause();
                break;
            }
            
            case 6: { // Delete Student
                std::string studentId;
                std::cout << "Enter student ID to delete: ";
                std::cin >> studentId;
                library.deleteStudent(studentId);
                library.pause();
                break;
            }
            
            case 7: // View All Students
                library.displayAllStudents();
                library.pause();
                break;
                
            case 8: // View Issued Books
                library.displayIssuedBooks();
                library.pause();
                break;
                
            case 9: { // Change Admin Password
                std::string oldPass, newPass, confirmPass;
                std::cout << "Enter current password: ";
                std::cin >> oldPass;
                std::cout << "Enter new password (min 4 chars): ";
                std::cin >> newPass;
                std::cout << "Confirm new password: ";
                std::cin >> confirmPass;
                
                if (newPass != confirmPass) {
                    std::cout << "Passwords do not match!" << std::endl;
                } else {
                    library.changeAdminPassword(oldPass, newPass);
                }
                library.pause();
                break;
            }
                
            case 10: // Logout
                std::cout << "Logged out successfully!" << std::endl;
                library.pause();
                return;
                
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
                library.pause();
        }
    }
}

void handleStudentMenu(Library& library, Student* currentStudent) {
    while (true) {
        library.displayStudentMenu();
        int choice = Library::getValidInt("");
        
        switch (choice) {
            case 1: { // Search Books
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string query = Library::getValidString("Enter search query (title, author, ISBN, or genre): ");
                
                std::vector<Book> results = library.searchBooks(query);
                if (results.empty()) {
                    std::cout << "No books found matching your query." << std::endl;
                } else {
                    std::cout << "\n=== Search Results ===" << std::endl;
                    std::cout << std::left << std::setw(5) << "ID" 
                              << std::setw(25) << "Title" 
                              << std::setw(20) << "Author" 
                              << std::setw(15) << "ISBN" 
                              << std::setw(12) << "Genre"
                              << std::setw(10) << "Available" << std::endl;
                    std::cout << std::string(87, '-') << std::endl;
                    
                    for (const auto& book : results) {
                        book.displayShort();
                    }
                }
                library.pause();
                break;
            }
            
            case 2: { // Issue Book
                int bookId = Library::getValidInt("Enter book ID to issue: ");
                library.issueBook(currentStudent->getStudentId(), bookId);
                library.pause();
                break;
            }
            
            case 3: { // Return Book
                int bookId = Library::getValidInt("Enter book ID to return: ");
                library.returnBook(currentStudent->getStudentId(), bookId);
                library.pause();
                break;
            }
            
            case 4: // View My Borrowed Books
                currentStudent->displayBorrowedBooks();
                library.pause();
                break;
                
            case 5: // View My Details
                currentStudent->display();
                library.pause();
                break;
                
            case 6: { // Change Password
                std::string oldPass, newPass, confirmPass;
                std::cout << "Enter current password: ";
                std::cin >> oldPass;
                std::cout << "Enter new password (min 4 chars): ";
                std::cin >> newPass;
                std::cout << "Confirm new password: ";
                std::cin >> confirmPass;
                
                if (newPass != confirmPass) {
                    std::cout << "Passwords do not match!" << std::endl;
                } else {
                    library.changeStudentPassword(currentStudent->getStudentId(), oldPass, newPass);
                }
                library.pause();
                break;
            }
                
            case 7: // Logout
                std::cout << "Logged out successfully!" << std::endl;
                library.pause();
                return;
                
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
                library.pause();
        }
    }
}

void handleStudentRegistration(Library& library) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\n=== Student Registration ===" << std::endl;
    std::string name = Library::getValidString("Enter your name: ");
    std::string password = Library::getValidString("Enter password (min 4 chars): ");
    
    while (password.length() < 4) {
        std::cout << "Password must be at least 4 characters!" << std::endl;
        password = Library::getValidString("Enter password (min 4 chars): ");
    }
    
    if (library.addStudent(name, password)) {
        std::cout << "\n=== Registration Successful! ===" << std::endl;
        std::cout << "Your Student ID has been generated above." << std::endl;
        std::cout << "Please note down your Student ID for future login." << std::endl;
        std::cout << "You can now login using option 2 from the main menu." << std::endl;
    } else {
        std::cout << "Registration failed!" << std::endl;
    }
    
    library.pause();
}

void handleForgotStudentId(Library& library) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\n=== Forgot Student ID? ===" << std::endl;
    std::string name = Library::getValidString("Enter your registered name: ");
    
    Student* student = library.findStudentByName(name);
    if (student != nullptr) {
        std::cout << "\nFound your account!" << std::endl;
        std::cout << "Your Student ID: " << student->getStudentId() << std::endl;
        std::cout << "Name: " << student->getName() << std::endl;
        std::cout << "You can now login using option 2 from the main menu." << std::endl;
    } else {
        std::cout << "\nNo account found with that name." << std::endl;
        std::cout << "Please check your name, or register as a new student." << std::endl;
    }
    
    library.pause();
}
