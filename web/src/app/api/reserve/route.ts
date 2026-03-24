import { NextResponse } from 'next/server';
import { db } from '@/lib/db';

export async function POST(request: Request) {
  try {
    const { studentId, bookId } = await request.json();

    if (!studentId || !bookId) {
      return NextResponse.json({ error: 'Missing information' }, { status: 400 });
    }

    // Check if book is available
    const book: any = db.prepare('SELECT available FROM books WHERE id = ?').get(bookId);
    
    if (!book || book.available <= 0) {
      return NextResponse.json({ error: 'Book no longer available' }, { status: 400 });
    }

    // Process reservation
    const issueDate = new Date().toISOString().split('T')[0];
    const dueDate = new Date(Date.now() + 14 * 24 * 60 * 60 * 1000).toISOString().split('T')[0];

    const transaction = db.transaction(() => {
      // Create issue record
      db.prepare('INSERT INTO borrowed_books (student_id, book_id, issue_date, due_date) VALUES (?, ?, ?, ?)').run(
        studentId, bookId, issueDate, dueDate
      );
      
      // Update book availability
      db.prepare('UPDATE books SET available = available - 1 WHERE id = ?').run(bookId);
      
      // Log activity
      db.prepare('INSERT INTO activity_log (action) VALUES (?)').run(`Online Reservation: ${studentId} booked book ID ${bookId}`);
    });

    transaction();

    return NextResponse.json({ message: 'Reservation successful!' });
  } catch (err: any) {
    return NextResponse.json({ error: err.message }, { status: 500 });
  }
}
