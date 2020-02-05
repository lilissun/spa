//
//  selector.h
//  symbolic
//
//  Created by Li Li on 8/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __symbolic__selector__
#define __symbolic__selector__

#include <iostream>
#include <vector>
#include <map>

#include "explicit/name.h"

namespace sym {
    class Event;
    class State;
    class Selector {
    protected:
        // _not_selectable_event consists of [Event ID] * [list of [Event Pattern * Removeable]].
        // 1. when the event pattern is not nullptr,
        // we should not select the target event if it can be matched;
        // 2. when the event pattern is nullptr,
        // we should always not select it.
        // the bool value shows if we can remove the event
        // when it is not selectable and not related to others
        std::map<emc::Name, std::vector<std::pair<const Event *, bool>>> _not_selectable_events; // owned
        // _initial_state give the initial value of the states.
        // when the target state can be matched,
        // we should take it as fulfilled 
        std::map<emc::Name, std::vector<const State *>> _state_patterns; // owned
        
    public:
        Selector() {}
        virtual ~Selector();
        
    public:
        Selector(const Selector &selector) = delete;
        Selector & operator = (const Selector &selector) = delete;
        
    public:
        void provide_event(const emc::Name &name, bool removeable);
        void provide_event(const Event *pattern, bool removeable);
        bool is_selectable_event(const Event *event) const;
        bool is_removable_event(const Event *event) const;

    public:
        void set_state_pattern(const State *pattern);
        bool is_state_in_pattern(const State *state) const;
        std::vector<const State *> state_patterns(const State *state) const;
    };
}

#endif /* defined(__symbolic__selector__) */
