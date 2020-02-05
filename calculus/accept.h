//
//  accept.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__accept__
#define __calculus__accept__

#include "process.h"

#include "explicit/name.h"

namespace cal {
    class Tuple;
    // _name(_arguments) @at _timestamp._next_process
    class Accept : public Process {
    protected:
        Name *_name; // owned
        Tuple *_arguments; // owned
        Name *_timestamp; // owned
        Process *_next_process; // owned
        
    public:
        Accept(Name *name, Tuple *arguments, Name *timestamp, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_ACCEPT, location), _name(name), _arguments(arguments), _timestamp(timestamp), _next_process(next_process) {}
        virtual ~Accept();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__accept__) */
