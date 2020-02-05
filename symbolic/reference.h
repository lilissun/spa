//
//  reference.h
//  symbolic
//
//  Created by Li Li on 30/3/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#ifndef __symbolic__reference__
#define __symbolic__reference__

#include <iostream>

#include "explicit/name.h"
#include "term.h"

namespace sym {
    class ReferencePattern : public Pattern {
        
        friend ReferencePattern * make_reference_pattern();
        
    protected:
        ReferencePattern() : Pattern(TermType::TERM_NUMBER) {}
        
    public:
        virtual ~ReferencePattern() {}
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    ReferencePattern * make_reference_pattern();
    
    class Reference : public Term {
        
        friend Reference * make_reference(const std::string &name, emc::Namer &namer, emc::Indexer &indexer);
        friend Reference * make_reference(const std::string &name, emc::Namer &namer);
        friend Reference * make_reference(const std::string &name, emc::Name::ID id, emc::Namer &namer);
        
    protected:
        emc::Name _name;
        
    protected:
        Reference(const emc::Name &name) : Term(TermType::TERM_REFERENCE), _name(name) {}
        
    public:
        virtual ~Reference() {}
        
    public:
        virtual bool same(const emc::Name &name) const { return _name == name; }
        virtual const emc::Name & name() const { return _name; }
        
    protected:
        virtual TermSort _sort() const { return TermSort::TERM_SORT_COMPLEX; }
        
    public:
        virtual Term * clone() const;
        Reference * reference_clone() const { return (Reference *)clone(); }
        virtual Term * map(const Unifier &unifier) const;
        Reference * reference_map(const Unifier &unifier) const { return (Reference *)map(unifier); }
        virtual Term * rename(emc::Renamer &renamer, emc::Namer &namer);
        
    public:
        virtual void unify(const Term *term, Unifier &unifier) const;
        virtual void unify_to(const Term *term, Unifier &unifier) const;
        virtual bool match(const Pattern *pattern, const Unifier &unifier) const;
        virtual size_t depth(const emc::Name &name, const Unifier &unifier) const;
        virtual size_t weight() const;
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const;
        virtual void signature(const Unifier &unifier, std::ostream &os) const;
        virtual void signature(std::ostream &os) const;
        
    public:
        virtual emc::Value evaluate(const emc::Valuator &valuator) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    Reference * make_reference(const std::string &name, emc::Namer &namer, emc::Indexer &indexer);
    Reference * make_reference(const std::string &name, emc::Namer &namer);
    Reference * make_reference(const std::string &name, emc::Name::ID id, emc::Namer &namer);
}

#endif /* __symbolic__reference__ */
