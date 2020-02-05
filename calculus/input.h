//
//  input.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__input__
#define __calculus__input__

#include "process.h"

namespace cal {
    // c(@variable _name)._next_process
    class Input : public Process {
    protected:
        Expression *_identifier; // owned
        Process *_next_process; // owned
        
    public:
        Input(Expression *identifier, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_INPUT, location), _identifier(identifier), _next_process(next_process) {}
        virtual ~Input();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__input__) */
