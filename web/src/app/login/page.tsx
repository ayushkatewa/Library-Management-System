"use client";

import Link from "next/link";
import { Library, Mail, Lock, ArrowRight } from "lucide-react";
import { useState } from "react";

export default function LoginPage() {
  const [role, setRole] = useState<'student' | 'admin'>('student');

  return (
    <div className="min-h-[80vh] flex items-center justify-center px-6">
      <div className="glass w-full max-w-md p-10 rounded-3xl space-y-8 relative overflow-hidden">
        <div className="absolute top-0 right-0 w-32 h-32 premium-gradient opacity-10 blur-3xl" />
        
        <div className="text-center space-y-2">
          <Link href="/" className="inline-flex items-center space-x-2 mb-6">
            <Library className="h-10 w-10 text-primary" />
            <span className="text-2xl font-bold tracking-tight">LuminaLib</span>
          </Link>
          <h1 className="text-3xl font-extrabold tracking-tight">Welcome Back</h1>
          <p className="text-muted text-sm">Sign in to your account to continue</p>
        </div>

        {/* Role Switcher */}
        <div className="flex bg-secondary/50 p-1 rounded-xl">
          <button 
            onClick={() => setRole('student')}
            className={`flex-1 py-2 text-xs font-bold rounded-lg transition-all ${role === 'student' ? 'bg-primary text-white shadow-lg' : 'text-muted hover:text-foreground'}`}
          >
            STUDENT
          </button>
          <button 
            onClick={() => setRole('admin')}
            className={`flex-1 py-2 text-xs font-bold rounded-lg transition-all ${role === 'admin' ? 'bg-primary text-white shadow-lg' : 'text-muted hover:text-foreground'}`}
          >
            LIBRARIAN
          </button>
        </div>

        <form className="space-y-6">
          <div className="space-y-4">
            <div className="relative">
              <Mail className="absolute left-4 top-1/2 -translate-y-1/2 h-5 w-5 text-muted" />
              <input 
                type="text" 
                placeholder={role === 'student' ? "Student ID (e.g. S1001)" : "Admin Username"}
                className="w-full bg-secondary/30 border border-muted/20 h-14 pl-12 pr-4 rounded-2xl focus:outline-none focus:ring-2 focus:ring-primary/40 transition-all"
              />
            </div>
            <div className="relative">
              <Lock className="absolute left-4 top-1/2 -translate-y-1/2 h-5 w-5 text-muted" />
              <input 
                type="password" 
                placeholder="Password"
                className="w-full bg-secondary/30 border border-muted/20 h-14 pl-12 pr-4 rounded-2xl focus:outline-none focus:ring-2 focus:ring-primary/40 transition-all"
              />
            </div>
          </div>

          <button className="w-full premium-gradient hover-glow text-white h-14 rounded-2xl font-bold flex items-center justify-center space-x-2 group">
            <span>Sign In</span>
            <ArrowRight className="h-5 w-5 group-hover:translate-x-1 transition-transform" />
          </button>
        </form>

        <div className="text-center text-sm text-muted">
          Don&apos;t have an account?{" "}
          <Link href="/signup" className="text-primary font-bold hover:underline">
            Create one now
          </Link>
        </div>
      </div>
    </div>
  );
}
