//
//  replication.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__replication__
#define __calculus__replication__

#include <iostream>

#include "process.h"

namespace cal {
    // !_next_process
    class Replication : public Process {
    protected:
        Process *_next_process; // owned
        
    public:
        Replication(Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_REPLICATION, location), _next_process(next_process) {}
        virtual ~Replication() { delete _next_process; }
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__replication__) */
