//
//  nonce.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__nonce__
#define __calculus__nonce__

#include <iostream>

#include "process.h"

namespace cal {
    // new _nonce at _location._next_process
    class Nonce : public Process {
    protected:
        Name *_nonce; // owned
        Process *_next_process; // owned
        
    public:
        Nonce(Name *nonce, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_NONCE, location), _nonce(nonce), _next_process(next_process) {}
        virtual ~Nonce();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__nonce__) */
