"use client";

import { BookOpen, Star, Clock, CheckCircle2, Loader2 } from "lucide-react";
import Link from "next/link";
import { useState } from "react";

interface BookCardProps {
  id: number;
  title: string;
  author: string;
  genre: string;
  available: boolean;
}

export default function BookCard({ id, title, author, genre, available }: BookCardProps) {
  const [isBooking, setIsBooking] = useState(false);
  const [isBooked, setIsBooked] = useState(false);

  const handleBook = async () => {
    setIsBooking(true);
    try {
      const resp = await fetch('/api/reserve', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ studentId: 'S1001', bookId: id }) // Defaulting to test student
      });
      if (resp.ok) {
        setIsBooked(true);
      }
    } catch (err) {
      console.error(err);
    } finally {
      setIsBooking(false);
    }
  };

  return (
    <div className="glass rounded-2xl overflow-hidden group hover-glow transition-all duration-500">
      <div className="h-48 premium-gradient relative overflow-hidden flex items-center justify-center p-8">
        <BookOpen className="h-20 w-20 text-white/40 group-hover:scale-110 group-hover:rotate-6 transition-transform duration-500" />
        <div className="absolute top-4 right-4 bg-black/20 backdrop-blur-md px-2 py-1 rounded-full flex items-center space-x-1">
          <Star className="h-3 w-3 text-yellow-400 fill-yellow-400" />
          <span className="text-[10px] font-bold text-white">4.8</span>
        </div>
      </div>
      
      <div className="p-5 space-y-4">
        <div className="space-y-1">
          <span className="text-[10px] font-bold uppercase tracking-widest text-primary block">
            {genre}
          </span>
          <h3 className="font-bold text-lg text-foreground line-clamp-1 group-hover:text-primary transition-colors">
            {title}
          </h3>
          <p className="text-sm text-muted">by {author}</p>
        </div>
        
        <div className="flex items-center justify-between border-t border-muted/10 pt-4">
          <div className="flex items-center space-x-1.5">
            <div className={`h-2 w-2 rounded-full ${available ? 'bg-green-500 shadow-[0_0_8px_rgba(34,197,94,0.6)]' : 'bg-red-500'}`} />
            <span className="text-xs font-medium text-muted">
              {available ? 'In Stock' : 'Out of Stock'}
            </span>
          </div>
          
          <Link href="/books" className="text-xs font-bold text-primary hover:underline flex items-center">
            Details
            <Clock className="h-3 w-3 ml-1" />
          </Link>
        </div>

        <button 
          onClick={handleBook}
          disabled={!available || isBooking || isBooked}
          className={`w-full h-11 rounded-xl text-xs font-bold transition-all duration-300 flex items-center justify-center space-x-2 
            ${isBooked ? 'bg-green-500/20 text-green-500 border border-green-500/30' : 
              available ? 'premium-gradient text-white hover:shadow-lg' : 'bg-muted/10 text-muted cursor-not-allowed'}`}
        >
          {isBooking ? <Loader2 className="h-4 w-4 animate-spin" /> : 
           isBooked ? <><CheckCircle2 className="h-4 w-4" /> <span>BOOKED</span></> : 
           <span>BOOK ONLINE</span>}
        </button>
      </div>
    </div>
  );
}
