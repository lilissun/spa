//
//  null.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__null__
#define __calculus__null__

#include <iostream>

#include "process.h"

namespace cal {
    class Null : public Process {
    public:
        Null(const com::Location &location) : Process(ProcessType::PROCESS_NULL, location) {}
        virtual ~Null() {}
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__null__) */
