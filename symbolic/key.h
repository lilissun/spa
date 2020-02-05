//
//  key.h
//  symbolic
//
//  Created by Li Li on 26/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __symbolic__key__
#define __symbolic__key__

#include <iostream>
#include <vector>
#include <set>

#include "fact.h"
#include "explicit/name.h"

namespace sym {
    class Tuple;
    class Key : public Fact {
        
        friend State;
        friend Event;
        
    protected:
        emc::Name _name;
        Tuple *_arguments; // owned
        
    protected:
        Key(const emc::Name &name, Tuple *arguments) : Fact(FactType::FACT_KEY), _name(name), _arguments(arguments) {}
        
    public:
        virtual ~Key();
        
    public:
        Key(const Key &) = delete;
        Key & operator = (const Key &) = delete;
        
    public:
        const emc::Name & name() const { return _name; }
        const Tuple * arguments() const { return _arguments; }
       
    public:
        std::string signature() const;
        std::string signature(const Unifier &unifier) const;
        
    public:
        Key * clone() const;
        Key * map(const Unifier &unifier) const;
        Key * rename(emc::Renamer &renamer, emc::Namer &namer);
        
    public:
        bool can_unify(const Key *key, Unifier &unifier) const;
        bool can_unify_to(const Key *key, Unifier &unifier) const;
        bool can_match(const Key *key, const Unifier &unifier) const;
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
}

#endif /* defined(__symbolic__key__) */
