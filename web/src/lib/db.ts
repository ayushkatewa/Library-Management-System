import Database from 'better-sqlite3';
import path from 'path';
import fs from 'fs';

// Connection to the shared database
// Using an absolute path check to ensure it works in all environments
const dbDir = path.resolve(process.cwd(), '../data');
if (!fs.existsSync(dbDir)) {
  fs.mkdirSync(dbDir, { recursive: true });
}

const dbPath = path.join(dbDir, 'library.db');

export const db = new Database(dbPath);

// Initialize tables (Keep this for first-time setup or schema updates)
db.exec(`
  CREATE TABLE IF NOT EXISTS books (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    author TEXT NOT NULL,
    isbn TEXT UNIQUE,
    quantity INTEGER DEFAULT 1,
    available INTEGER DEFAULT 1,
    genre TEXT DEFAULT 'General'
  );

  CREATE TABLE IF NOT EXISTS students (
    student_id TEXT PRIMARY KEY,
    name TEXT NOT NULL,
    password_hash TEXT NOT NULL,
    fine REAL DEFAULT 0.0
  );

  CREATE TABLE IF NOT EXISTS borrowed_books (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_id TEXT NOT NULL,
    book_id INTEGER NOT NULL,
    issue_date TEXT NOT NULL,
    due_date TEXT NOT NULL,
    returned INTEGER DEFAULT 0,
    FOREIGN KEY(student_id) REFERENCES students(student_id),
    FOREIGN KEY(book_id) REFERENCES books(id)
  );
`);
