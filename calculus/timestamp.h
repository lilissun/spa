//
//  timestamp.h
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__timestamp__
#define __calculus__timestamp__

#include "process.h"

namespace cal {
    // read _timestamp._next_process
    class Timestamp : public Process {
    protected:
        Name *_timestamp; // owned
        std::string _clock;
        Process *_next_process; // owned
        
    public:
        Timestamp(Name *timestamp, const std::string &clock, Process *next_process, const com::Location &location) : Process(ProcessType::PROCESS_TIMESTAMP, location), _timestamp(timestamp), _clock(clock), _next_process(next_process) {}
        virtual ~Timestamp();
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const;
    };
}

#endif /* defined(__calculus__timestamp__) */
