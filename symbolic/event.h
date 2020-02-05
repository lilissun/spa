//
//  event.h
//  symbolic
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __symbolic__event__
#define __symbolic__event__

#include <iostream>
#include <vector>
#include <set>

#include "explicit/name.h"
#include "fact.h"

namespace sym {
    class Tuple;
    class Event : public Fact {
        
        friend Event * make_event(const std::string &name, size_t keycount, Tuple *arguments, emc::Namer &namer);
        
    protected:
        emc::Name _name;
        size_t _keycount;
        Tuple *_arguments; // owned
        
    protected:
        Event(const emc::Name &name, size_t keycount, Tuple *arguments) : Fact(FactType::FACT_EVENT), _name(name), _keycount(keycount), _arguments(arguments) {}
        
    public:
        virtual ~Event();
        
    public:
        Event(const Event &) = delete;
        Event & operator = (const Event &) = delete;
        
    public:
        const emc::Name & name() const { return _name; }
        const size_t keycount() const { return _keycount; }
        const Tuple * arguments() const { return _arguments; }
        
    public:
        Key * key() const;
        Key * key(const Unifier &unifier) const;
        std::string signature(bool key_only) const;
        std::string signature(const Unifier &unifier, bool key_only) const;
        
    public:
        Event * clone() const;
        Event * map(const Unifier &unifier) const;
        Event * rename(emc::Renamer &renamer, emc::Namer &namer);
        
    public:
        size_t weight() const;
        
    public:
        bool can_unify(const Event *event, Unifier &unifier) const;
        bool can_unify_to(const Event *event, Unifier &unifier) const;
        bool can_match(const Event * event, const Unifier &unifier) const;
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    Event * make_event(const std::string &name, size_t keycount, Tuple *arguments, emc::Namer &namer);
}

#endif /* defined(__symbolic__event__) */
