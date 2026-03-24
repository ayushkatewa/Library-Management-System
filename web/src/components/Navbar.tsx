import Link from "next/link";
import { Book, Library, Users, Search, ShoppingBag } from "lucide-react";

export default function Navbar() {
  return (
    <nav className="sticky top-0 z-50 glass py-3 border-b">
      <div className="container mx-auto px-6 flex items-center justify-between">
        <Link href="/" className="flex items-center space-x-2 group">
          <Library className="h-8 w-8 text-primary group-hover:scale-110 transition-transform" />
          <span className="text-xl font-bold tracking-tight text-foreground">
            Lumina<span className="text-primary">Lib</span>
          </span>
        </Link>
        
        <div className="hidden md:flex items-center space-x-8 text-sm font-medium">
          <Link href="/books" className="text-muted-foreground hover:text-foreground transition-colors flex items-center">
            <Book className="h-4 w-4 mr-1.5" />
            Library
          </Link>
          <Link href="/students" className="text-muted-foreground hover:text-foreground transition-colors flex items-center">
            <Users className="h-4 w-4 mr-1.5" />
            Students
          </Link>
          <button className="text-muted-foreground hover:text-foreground transition-colors relative">
            <ShoppingBag className="h-5 w-5" />
            <span className="absolute -top-1.5 -right-1.5 h-4 w-4 bg-primary text-[10px] flex items-center justify-center rounded-full text-white font-bold">
              0
            </span>
          </button>
        </div>

        <div className="flex items-center space-x-4">
          <div className="relative hidden sm:block">
            <Search className="absolute left-3 top-1/2 -translate-y-1/2 h-4 w-4 text-muted" />
            <input 
              type="text" 
              placeholder="Search library..." 
              className="bg-secondary/50 border border-muted/20 text-sm h-10 w-64 pl-10 pr-4 rounded-full focus:outline-none focus:ring-2 focus:ring-primary/40 transition-all"
            />
          </div>
          <Link href="/login" className="premium-gradient hover-glow text-primary-foreground text-sm font-semibold px-6 py-2 rounded-full transition-all text-center">
            Login
          </Link>
        </div>
      </div>
    </nav>
  );
}
