//
//  open.h
//  calculus
//
//  Created by Li Li on 13/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__open__
#define __calculus__open__

#include "process.h"

namespace cal {
    // secrecy(_term)._next_process
    class Open : public Process {
    protected:
        Expression *_term; // owned
        Process *_next_process; // owned
        
    public:
        Open(Expression *term, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_SECRECY, location), _term(term), _next_process(next_process) {}
        virtual ~Open();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__open__) */
