import { db } from "@/lib/db";
import BookCard from "@/components/BookCard";
import Link from "next/link";
import { Sparkles, ArrowRight, Zap, ShieldCheck, Globe } from "lucide-react";

export default function Home() {
  // Fetch some books from the shared SQLite DB
  const books = db.prepare('SELECT * FROM books LIMIT 4').all();

  return (
    <div className="space-y-20 pb-20">
      {/* Hero Section */}
      <section className="relative pt-20 pb-32 overflow-hidden">
        <div className="absolute top-0 left-1/2 -translate-x-1/2 w-full h-full max-w-7xl">
          <div className="absolute top-0 left-10 w-72 h-72 bg-primary/20 rounded-full blur-[120px] animate-pulse" />
          <div className="absolute bottom-0 right-10 w-96 h-96 bg-accent/20 rounded-full blur-[140px]" />
        </div>
        
        <div className="container mx-auto px-6 relative text-center space-y-8">
          <div className="inline-flex items-center space-x-2 bg-secondary/50 border border-muted/20 px-4 py-1.5 rounded-full text-xs font-bold tracking-wide text-primary uppercase">
            <Sparkles className="h-3.5 w-3.5" />
            <span>Redefining Library Management</span>
          </div>
          
          <h1 className="text-6xl md:text-8xl font-extrabold tracking-tighter text-foreground max-w-5xl mx-auto leading-tight">
            The Future of <span className="text-primary premium-gradient bg-clip-text text-transparent">Reading</span> is Here.
          </h1>
          
          <p className="text-xl text-muted max-w-2xl mx-auto leading-relaxed">
            Experience a seamless, high-performance library ecosystem. 
            Unified across terminal and web, built for the next generation of readers.
          </p>
        </div>
      </section>

      {/* Stats Section */}
      <section className="container mx-auto px-6 grid grid-cols-1 md:grid-cols-3 gap-8">
        {[
          { icon: Zap, label: "Real-time Sync", desc: "CLI and Web always in harmony" },
          { icon: ShieldCheck, label: "Secure Hashing", desc: "Enterprise-grade data protection" },
          { icon: Globe, label: "Global Access", desc: "Browse your library from anywhere" }
        ].map((feat, i) => (
          <div key={i} className="glass p-8 rounded-3xl space-y-4 hover:border-primary/50 transition-colors">
            <div className="h-12 w-12 premium-gradient rounded-xl flex items-center justify-center shadow-lg">
              <feat.icon className="h-6 w-6 text-white" />
            </div>
            <h3 className="text-xl font-bold">{feat.label}</h3>
            <p className="text-sm text-muted">{feat.desc}</p>
          </div>
        ))}
      </section>

      {/* Featured Books Grid */}
      <section className="container mx-auto px-6 space-y-10">
        <div className="flex items-end justify-between">
          <div className="space-y-2">
            <h2 className="text-3xl font-extrabold tracking-tight">Featured Collection</h2>
            <p className="text-muted">Discover the most popular titles in our system.</p>
          </div>
          <Link href="/books" className="text-primary font-bold hover:underline flex items-center">
            View All <ArrowRight className="h-4 w-4 ml-1" />
          </Link>
        </div>
        
        <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-8">
          {books.length > 0 ? (
            books.map((book: any) => (
              <BookCard 
                key={book.id}
                id={book.id}
                title={book.title}
                author={book.author}
                genre={book.genre}
                available={book.available > 0}
              />
            ))
          ) : (
            // Placeholder/Empty state
            [1,2,3,4].map(i => (
              <div key={i} className="glass aspect-[3/4] rounded-3xl animate-pulse bg-secondary/20" />
            ))
          )}
        </div>
      </section>
    </div>
  );
}
