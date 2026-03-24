"use client";

import Link from "next/link";
import { Library, User, Mail, Lock, ArrowRight } from "lucide-react";

export default function SignupPage() {
  return (
    <div className="min-h-[80vh] flex items-center justify-center px-6">
      <div className="glass w-full max-w-md p-10 rounded-3xl space-y-8 relative overflow-hidden">
        <div className="absolute top-0 right-0 w-32 h-32 premium-gradient opacity-10 blur-3xl" />
        
        <div className="text-center space-y-2">
          <Link href="/" className="inline-flex items-center space-x-2 mb-6">
            <Library className="h-10 w-10 text-primary" />
            <span className="text-2xl font-bold tracking-tight">LuminaLib</span>
          </Link>
          <h1 className="text-3xl font-extrabold tracking-tight">Join the Library</h1>
          <p className="text-muted text-sm">Create your student account to start booking</p>
        </div>

        <form className="space-y-6">
          <div className="space-y-4">
            <div className="relative">
              <User className="absolute left-4 top-1/2 -translate-y-1/2 h-5 w-5 text-muted" />
              <input 
                type="text" 
                placeholder="Full Name"
                className="w-full bg-secondary/30 border border-muted/20 h-14 pl-12 pr-4 rounded-2xl focus:outline-none focus:ring-2 focus:ring-primary/40 transition-all"
              />
            </div>
            <div className="relative">
              <Mail className="absolute left-4 top-1/2 -translate-y-1/2 h-5 w-5 text-muted" />
              <input 
                type="text" 
                placeholder="Student ID (e.g. S1001)"
                className="w-full bg-secondary/30 border border-muted/20 h-14 pl-12 pr-4 rounded-2xl focus:outline-none focus:ring-2 focus:ring-primary/40 transition-all"
              />
            </div>
            <div className="relative">
              <Lock className="absolute left-4 top-1/2 -translate-y-1/2 h-5 w-5 text-muted" />
              <input 
                type="password" 
                placeholder="Secure Password"
                className="w-full bg-secondary/30 border border-muted/20 h-14 pl-12 pr-4 rounded-2xl focus:outline-none focus:ring-2 focus:ring-primary/40 transition-all"
              />
            </div>
          </div>

          <button className="w-full premium-gradient hover-glow text-white h-14 rounded-2xl font-bold flex items-center justify-center space-x-2 group">
            <span>Create Account</span>
            <ArrowRight className="h-5 w-5 group-hover:translate-x-1 transition-transform" />
          </button>
        </form>

        <div className="text-center text-sm text-muted">
          Already a member?{" "}
          <Link href="/login" className="text-primary font-bold hover:underline">
            Sign in
          </Link>
        </div>
      </div>
    </div>
  );
}
