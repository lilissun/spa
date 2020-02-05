//
//  unique.h
//  calculus
//
//  Created by Li Li on 29/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__unique__
#define __calculus__unique__

#include <iostream>

#include "process.h"

namespace cal {
    // unique _expression in _database._next_process
    class Unique : public Process {
    protected:
        Expression *_expression; // owned
        Name *_database; // owned
        Process *_next_process; // owned
        
    public:
        Unique(Expression *expression, Name *database, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_UNIQUE, location), _expression(expression), _database(database), _next_process(next_process) {}
        virtual ~Unique();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__unique__) */
