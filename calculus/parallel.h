//
//  parallel.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__parallel__
#define __calculus__parallel__

#include <iostream>
#include <vector>

#include "process.h"

namespace cal {
    // _process_1 | _process_2 | ... | _process_n
    class Parallel : public Process {
    protected:
        std::vector<Process *> _next_processes; // owned
        
    public:
        Parallel(const std::vector<Process *> &next_processes, const com::Location &location) : Process(ProcessType::PROCESS_PARALLEL, location), _next_processes(next_processes) {}
        virtual ~Parallel();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__parallel__) */
