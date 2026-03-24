#include "Book.h"
#include "Colors.h"
#include <fstream>
#include <iomanip>

// Initialize static member
int Book::nextBookId = 1;

// Constructors
Book::Book() : title(""), author(""), isbn(""), genre("General"), quantity(0), available(0), bookId(0) {}

Book::Book(const std::string& title, const std::string& author, const std::string& isbn, int quantity, const std::string& genre)
    : title(title), author(author), isbn(isbn), genre(genre), quantity(quantity), available(quantity), bookId(nextBookId++) {}

// Getters
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getIsbn() const { return isbn; }
std::string Book::getGenre() const { return genre; }
int Book::getQuantity() const { return quantity; }
int Book::getAvailable() const { return available; }
int Book::getBookId() const { return bookId; }

// Setters
void Book::setTitle(const std::string& title) { this->title = title; }
void Book::setAuthor(const std::string& author) { this->author = author; }
void Book::setIsbn(const std::string& isbn) { this->isbn = isbn; }
void Book::setGenre(const std::string& genre) { this->genre = genre; }
void Book::setQuantity(int quantity) { 
    this->quantity = quantity; 
    if (available > quantity) available = quantity;
}
void Book::setAvailable(int available) { this->available = available; }

// Book operations
bool Book::issueBook() {
    if (available > 0) {
        available--;
        return true;
    }
    return false;
}

bool Book::returnBook() {
    if (available < quantity) {
        available++;
        return true;
    }
    return false;
}

bool Book::isAvailable() const {
    return available > 0;
}

// Display methods
void Book::display() const {
    std::cout << Colors::BOLD << Colors::MAGENTA << "\n=== Book Details ===" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "ID:      " << Colors::WHITE << bookId << std::endl;
    std::cout << Colors::CYAN << "Title:   " << Colors::WHITE << title << std::endl;
    std::cout << Colors::CYAN << "Author:  " << Colors::WHITE << author << std::endl;
    std::cout << Colors::CYAN << "ISBN:    " << Colors::WHITE << isbn << std::endl;
    std::cout << Colors::CYAN << "Genre:   " << Colors::WHITE << genre << std::endl;
    std::cout << Colors::CYAN << "Total:   " << Colors::WHITE << quantity << std::endl;
    std::cout << Colors::CYAN << "Avail:   " << Colors::WHITE << available << std::endl;
    std::cout << Colors::CYAN << "Status:  " << (isAvailable() ? (std::string(Colors::GREEN) + "Available") : (std::string(Colors::RED) + "Out of Stock")) << Colors::RESET << std::endl;
}

void Book::displayShort() const {
    std::cout << Colors::CYAN << std::left << std::setw(5) << bookId << Colors::RESET 
              << std::setw(25) << title 
              << std::setw(20) << author 
              << std::setw(15) << isbn 
              << std::setw(12) << genre;
    
    if (available == 0) {
        std::cout << Colors::RED << std::setw(10) << "0/" << quantity << Colors::RESET << std::endl;
    } else {
        std::cout << Colors::GREEN << std::setw(10) << std::to_string(available) + "/" + std::to_string(quantity) << Colors::RESET << std::endl;
    }
}

// File operations
void Book::saveToFile(std::ofstream& file) const {
    file << bookId << "|" << title << "|" << author << "|" << isbn << "|" << genre << "|" << quantity << "|" << available << std::endl;
}

void Book::loadFromFile(std::ifstream& file) {
    std::string line;
    if (std::getline(file, line)) {
        if (line.empty()) {
            bookId = 0;
            return;
        }

        try {
            size_t pos = 0;
            
            // Parse bookId
            pos = line.find('|');
            if (pos == std::string::npos) { bookId = 0; return; }
            bookId = std::stoi(line.substr(0, pos));
            if (bookId >= nextBookId) nextBookId = bookId + 1;
            line.erase(0, pos + 1);
            
            // Parse title
            pos = line.find('|');
            if (pos == std::string::npos) { bookId = 0; return; }
            title = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Parse author
            pos = line.find('|');
            if (pos == std::string::npos) { bookId = 0; return; }
            author = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Parse isbn
            pos = line.find('|');
            if (pos == std::string::npos) { bookId = 0; return; }
            isbn = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            // Parse genre (new field — handle legacy files without genre)
            pos = line.find('|');
            if (pos == std::string::npos) {
                // Legacy format: no genre field, remaining is "quantity|available"
                genre = "General";
                // Remaining line is just available count from old format
                // Actually in old format: quantity|available (no genre)
                // But we already consumed isbn, so remaining = quantity|available
                // This case shouldn't happen for well-formed new files
                bookId = 0;
                return;
            }
            
            // Could be genre or quantity — detect by trying to parse as int
            std::string maybeGenre = line.substr(0, pos);
            line.erase(0, pos + 1);
            
            pos = line.find('|');
            if (pos != std::string::npos) {
                // New format: genre|quantity|available
                genre = maybeGenre;
                quantity = std::stoi(line.substr(0, pos));
                line.erase(0, pos + 1);
                available = std::stoi(line);
            } else {
                // Old format: quantity|available (maybeGenre is actually quantity)
                genre = "General";
                quantity = std::stoi(maybeGenre);
                available = std::stoi(line);
            }
        } catch (const std::exception&) {
            bookId = 0; // Mark as invalid on parse error
        }
    }
}
