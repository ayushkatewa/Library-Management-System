import { db } from "@/lib/db";
import { User, Shield, Info } from "lucide-react";

export default function StudentsPage() {
  const students = db.prepare('SELECT * FROM students').all();

  return (
    <div className="container mx-auto px-6 py-12 space-y-12">
      <div className="space-y-1">
        <h1 className="text-4xl font-extrabold tracking-tight">Student Directory</h1>
        <p className="text-muted">Manage system users and view borrowing status.</p>
      </div>

      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-8">
        {students.length > 0 ? (
          students.map((student: any) => (
            <div key={student.student_id} className="glass p-6 rounded-2xl flex items-start space-x-4 group hover:border-primary/50 transition-colors">
              <div className="h-12 w-12 premium-gradient rounded-full flex items-center justify-center shrink-0">
                <User className="h-6 w-6 text-white" />
              </div>
              <div className="space-y-2">
                <h3 className="font-bold text-lg">{student.name}</h3>
                <div className="flex flex-col text-sm text-muted space-y-1">
                  <span className="flex items-center">
                    <Shield className="h-3.5 w-3.5 mr-1 text-primary" />
                    ID: {student.student_id}
                  </span>
                  <span className="flex items-center">
                    <Info className="h-3.5 w-3.5 mr-1 text-accent" />
                    Fines: ${student.fine.toFixed(2)}
                  </span>
                </div>
              </div>
            </div>
          ))
        ) : (
          <div className="col-span-full py-20 text-center glass rounded-3xl border-dashed border-2 border-muted/20">
            <Users className="h-12 w-12 text-muted mx-auto mb-4" />
            <p className="text-muted font-medium">No students registered in the system yet.</p>
          </div>
        )}
      </div>
    </div>
  );
}
import { Users } from "lucide-react";
