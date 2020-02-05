//
//  output.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__output__
#define __calculus__output__

#include "process.h"

namespace cal {
    // \overline{c}(_term)._next_process
    class Output : public Process {
    protected:
        Expression *_term; // owned
        Process *_next_process; // owned
        
    public:
        Output(Expression *term, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_OUTPUT, location), _term(term), _next_process(next_process) {}
        virtual ~Output();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__output__) */
