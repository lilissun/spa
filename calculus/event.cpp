//
//  event.cpp
//  calculus
//
//  Created by Li Li on 24/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "event.h"

#include "common/runtime.h"
#include "symbolic/event.h"
#include "symbolic/nonce.h"
#include "symbolic/variable.h"
#include "state.h"
#include "program.h"

namespace cal {
    Event::~Event()
    {
        delete _name;
        delete _arguments;
        delete _timestamp;
    }
    
    sym::Event * Event::evaluate(const Program *program, const State *state) const
    {
        if (_type == EventType::EVENT_INIT) {
            return program->init_event(_name->name(), program->new_variable(), _arguments->evaluate(state), _timestamp->evaluate(state));
        } else if (_type == EventType::EVENT_JOIN) {
            return program->join_event(_name->name(), program->new_variable(), _arguments->evaluate(state), _timestamp->evaluate(state));
        } else {
            ASSERT(_type == EventType::EVENT_ACCEPT);
            return program->accept_event(_name->name(), program->new_variable(), _arguments->evaluate(state), _timestamp->evaluate(state));
        }
    }
    
    void Event::info(std::ostream &os) const
    {
        if (_type == EventType::EVENT_INIT) {
            os << event_init << event_space;
        } else if (_type == EventType::EVENT_JOIN) {
            os << event_join << event_space;
        } else {
            ASSERT(_type == EventType::EVENT_ACCEPT);
            os << event_accept << event_space;
        }
        _name->info(os);
        _arguments->info(os);
        os << event_at;
        _timestamp->info(os);
    }
}