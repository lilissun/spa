//
//  init.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__init__
#define __calculus__init__

#include "process.h"

#include "explicit/name.h"

namespace cal {
    class Tuple;
    class Name;
    // _name(_arguments) @at _timestamp._next_process
    class Init : public Process {
    protected:
        Name *_name; // owned
        Tuple *_arguments; // owned
        Name *_timestamp; // owned
        Process *_next_process; // owned
        
    public:
        Init(Name *name, Tuple *arguments, Name *timestamp, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_INIT, location), _name(name), _arguments(arguments), _timestamp(timestamp), _next_process(next_process) {}
        virtual ~Init();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__init__) */
