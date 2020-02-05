//
//  event.h
//  calculus
//
//  Created by Li Li on 24/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__event__
#define __calculus__event__

#include <iostream>

#include "symbolic/model.h"

#include "tuple.h"
#include "name.h"

namespace sym {
    class Event;
}

namespace cal {
    enum EventType {
        EVENT_INIT,
        EVENT_JOIN,
        EVENT_ACCEPT,
        EVENT_TYPE_END,
    };
    
    const std::string event_space = " ";
    const std::string event_init = "@init";
    const std::string event_join = "@join";
    const std::string event_accept = "@accept";
    const std::string event_at = "@";
    
    class Event : public sym::Model {
    protected:
        EventType _type;
        Name *_name;
        Tuple *_arguments;
        Name *_timestamp;
        
    public:
        Event(EventType type, Name *name, Tuple *arguments, Name *timestamp) : _type(type), _name(name), _arguments(arguments), _timestamp(timestamp) {}
        virtual ~Event();
        
    public:
        EventType type() const { return _type; }
        sym::Event * evaluate(const Program *program, const State *state) const;
        
    public:
        void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__event__) */
