import { db } from "@/lib/db";
import BookCard from "@/components/BookCard";
import { Search, Filter } from "lucide-react";

export default function BooksPage() {
  const books = db.prepare('SELECT * FROM books').all();

  return (
    <div className="container mx-auto px-6 py-12 space-y-12">
      <div className="flex flex-col md:flex-row md:items-center justify-between gap-6">
        <div className="space-y-1">
          <h1 className="text-4xl font-extrabold tracking-tight">Library Catalog</h1>
          <p className="text-muted">Explore our complete collection of {books.length} titles.</p>
        </div>
        
        <div className="flex items-center space-x-4">
          <div className="relative">
            <Search className="absolute left-3 top-1/2 -translate-y-1/2 h-4 w-4 text-muted" />
            <input 
              type="text" 
              placeholder="Filter by title or author..." 
              className="bg-secondary/50 border border-muted/20 text-sm h-11 w-72 pl-10 pr-4 rounded-xl focus:outline-none focus:ring-2 focus:ring-primary/40"
            />
          </div>
          <button className="glass h-11 px-4 rounded-xl flex items-center space-x-2 text-sm font-semibold">
            <Filter className="h-4 w-4" />
            <span>Genre</span>
          </button>
        </div>
      </div>

      <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 gap-8">
        {books.map((book: any) => (
          <BookCard 
            key={book.id}
            id={book.id}
            title={book.title}
            author={book.author}
            genre={book.genre}
            available={book.available > 0}
          />
        ))}
      </div>
    </div>
  );
}
