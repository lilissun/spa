//
//  secrecy.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__secrecy__
#define __calculus__secrecy__

#include "process.h"

namespace cal {
    // secrecy(_term)._next_process
    class Secrecy : public Process {
    protected:
        Expression *_term; // owned
        Process *_next_process; // owned
        
    public:
        Secrecy(Expression *term, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_SECRECY, location), _term(term), _next_process(next_process) {}
        virtual ~Secrecy();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__secrecy__) */
