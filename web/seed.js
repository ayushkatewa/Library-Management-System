const Database = require('better-sqlite3');
const path = require('path');
const fs = require('fs');

const dbDir = path.resolve(__dirname, '../data');
if (!fs.existsSync(dbDir)) fs.mkdirSync(dbDir, { recursive: true });

const db = new Database(path.join(dbDir, 'library.db'));

// Clear existing
db.exec(`
  DELETE FROM books;
  DELETE FROM students;
`);

// Optimized batch insertion for 10,000+ books
const insertBook = db.prepare('INSERT INTO books (title, author, isbn, quantity, available, genre) VALUES (?, ?, ?, ?, ?, ?)');

const genres = ['Technology', 'Fiction', 'Science', 'Sci-Fi', 'History', 'Philosophy', 'Art', 'Biography'];
const authors = ['Andrew Hunt', 'F. Scott Fitzgerald', 'Robert C. Martin', 'Stephen Hawking', 'Andy Weir', 'Yuval Noah Harari', 'Isaac Asimov', 'George Orwell'];

console.log("Starting massive seed of 10,000 books...");

const insertMany = db.transaction((count) => {
  for (let i = 1; i <= count; i++) {
    insertBook.run(
      `Book Title #${i}`,
      authors[Math.floor(Math.random() * authors.length)],
      `ISBN-${1000000 + i}`,
      Math.floor(Math.random() * 10) + 1,
      Math.floor(Math.random() * 5) + 1,
      genres[Math.floor(Math.random() * genres.length)]
    );
  }
});

insertMany(10000);

// Add sample students
const insertStudent = db.prepare('INSERT INTO students (student_id, name, password_hash, fine) VALUES (?, ?, ?, ?)');
insertStudent.run('S1001', 'Ayush Katewa', 'hash_placeholder', 0.0);
insertStudent.run('S1002', 'John Doe', 'hash_placeholder', 15.50);

console.log("Database seeded successfully with 10,000+ records!");
