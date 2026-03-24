#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

class Book {
private:
    std::string title;
    std::string author;
    std::string isbn;
    std::string genre;
    int quantity;
    int available;
    int bookId;
    static int nextBookId;

public:
    // Constructors
    Book();
    Book(const std::string& title, const std::string& author, const std::string& isbn, int quantity, const std::string& genre = "General");
    
    // Getters
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getIsbn() const;
    std::string getGenre() const;
    int getQuantity() const;
    int getAvailable() const;
    int getBookId() const;
    
    // Setters
    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setIsbn(const std::string& isbn);
    void setGenre(const std::string& genre);
    void setQuantity(int quantity);
    void setAvailable(int available);
    
    // Book operations
    bool issueBook();
    bool returnBook();
    bool isAvailable() const;
    
    // Display methods
    void display() const;
    void displayShort() const;
    
    // File operations
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif
