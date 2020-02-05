//
//  selector.cpp
//  symbolic
//
//  Created by Li Li on 8/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#include "selector.h"



#include "common/runtime.h"
#include "event.h"
#include "state.h"
#include "unifier.h"

namespace sym {
    Selector::~Selector()
    {
        for (auto i = _not_selectable_events.begin(); i != _not_selectable_events.end(); ++i) {
            for (auto j = i->second.begin(); j != i->second.end(); ++j) {
                delete j->first;
            }
        }
        _not_selectable_events.clear();
        
        for (auto i = _state_patterns.begin(); i != _state_patterns.end(); ++i) {
            for (auto j = i->second.begin(); j != i->second.end(); ++j) {
                delete *j;
            }
        }
        _state_patterns.clear();
    }
    
    void Selector::provide_event(const emc::Name &name, bool removeable)
    {
        auto it = _not_selectable_events.find(name);
        
        if (it != _not_selectable_events.end()) {
            USER_ERROR("@provide duplicated for @event " + name.name());
            return ;
        }
        
        _not_selectable_events[name].push_back(std::make_pair(nullptr, removeable));
        return ;
    }
    
    void Selector::provide_event(const Event *pattern, bool removeable)
    {
        ASSERT(pattern != nullptr);
        
        emc::Name name = pattern->name();
        auto it = _not_selectable_events.find(name);

        if (it != _not_selectable_events.end()) {
            for (auto i = it->second.begin(); i != it->second.end(); ++i) {
                if (i->first == nullptr) {
                    USER_ERROR("@provide duplicated for @event " + name.name());
                    return ;
                }
                
                if (pattern->can_match(i->first, Unifier())) {
                    USER_ERROR("@provide duplicated for @event " + name.name());
                    return ;
                }
            }
        }
        
        _not_selectable_events[name].push_back(std::make_pair(pattern, removeable));
        return ;
    }
    
    bool Selector::is_selectable_event(const Event *event) const
    {
        ASSERT(event != nullptr);
        auto it = _not_selectable_events.find(event->name());
        if (it == _not_selectable_events.end()) {
            return true;
        } else if (it->second.front().first == nullptr) {
            ASSERT(it->second.size() == 1);
            return false;
        } else {
            for (auto i = it->second.begin(); i != it->second.end(); ++i) {
                ASSERT(i->first != nullptr);
                if (event->can_match(i->first, Unifier())) {
                    return false;
                }
            }
            return true;
        }
    }
    
    bool Selector::is_removable_event(const Event *event) const
    {
        ASSERT(event != nullptr);
        auto it = _not_selectable_events.find(event->name());
        if (it == _not_selectable_events.end()) {
            return false;
        } else if (it->second.front().first == nullptr) {
            ASSERT(it->second.size() == 1);
            return it->second.front().second;
        } else {
            for (auto i = it->second.begin(); i != it->second.end(); ++i) {
                ASSERT(i->first != nullptr);
                if (event->can_match(i->first, Unifier())) {
                    return i->second;
                }
            }
            return false;
        }
    }
    
    void Selector::set_state_pattern(const State *pattern)
    {
        if (pattern == nullptr) {
            USER_ERROR("pattern should not be nullptr for @state" + pattern->name().string());
        }
        
        auto it = _state_patterns.find(pattern->name());
        if (it != _state_patterns.end()) {
            for (auto i = it->second.begin(); i != it->second.end(); ++i) {
                if (pattern->can_match(*i, Unifier())) {
                    USER_ERROR("initial state pattern duplicated for @state " + pattern->name().string());
                    return ;
                }
            }
        }
        
        _state_patterns[pattern->name()].push_back(pattern);
        return ;
    }
    
    bool Selector::is_state_in_pattern(const State *state) const
    {
        auto it = _state_patterns.find(state->name());
        if (it == _state_patterns.end()) {
            return true;
        } else {
            for (auto i = it->second.begin(); i != it->second.end(); ++i) {
                ASSERT(*i != nullptr);
                Unifier unifier;
                if (state->can_unify_to(*i, unifier)) {
                    return true;
                }
            }
            return false;
        }
    }
    
    std::vector<const State *> Selector::state_patterns(const State *state) const
    {
        auto it = _state_patterns.find(state->name());
        if (it == _state_patterns.end()) {
            return std::vector<const State *>();
        } else {
            return it->second;
        }
    }
}