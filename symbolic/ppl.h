//
//  ppl.h
//  symbolic
//
//  Created by Li Li on 3/5/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#ifndef __symbolic__ppl__
#define __symbolic__ppl__

#include <iostream>
#include <map>
#include <vector>
#include <set>

#include "explicit/name.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#ifdef DEBUG
#include "ppl/ppl.hh"
#else
#include <ppl.hh>
#endif
#pragma clang diagnostic pop

namespace PPL = Parma_Polyhedra_Library;
namespace Parma_Polyhedra_Library {
    typedef NNC_Polyhedron Zone;
}

namespace emc {
    class Renamer;
}

namespace sym {
    class Constraint;
    class Unifier;
}

namespace ppl {
    const std::string zone_constant_mark = "$";
    const std::string zone_variable_mark = "#";
    const std::string zone_empty = "[@empty]";
    const std::string zoneset_empty = "{[@empty]}";
    const std::string zone_universe = "[@universe]";
    
    class ZoneSet;
    class Zone {
    public:
        typedef size_t Dimension;
        
    protected:
        std::vector<emc::Name> _names;
        PPL::Zone _zone;
        
    public:
        Zone(bool universe)
        : _names(), _zone(0, universe ? PPL::UNIVERSE : PPL::EMPTY) {}
        Zone(const std::vector<emc::Name> &names, bool universe = true)
        : _names(names), _zone(names.size(), universe ? PPL::UNIVERSE : PPL::EMPTY) {}
        Zone(const Zone &zone)
        : _names(zone._names), _zone(zone._zone) {}
        ~Zone() {}
        
    public:
        const std::vector<emc::Name> & names() const { return _names; }
        
    protected:
        void add(const emc::Name &name);
        template<typename InputIterator>
        void add(const InputIterator &begin, const InputIterator &end) {
            for (auto name = begin; name != end; ++name) {
                add(*name);
            }
        }
        void remove(const emc::Name &name);
        void rename(const emc::Name &from, const emc::Name &to);
        
    public:
        void retain(const std::set<emc::Name> &names);
        void rename(const sym::Unifier &unifier);
        void rename(const emc::Renamer &renamer);
        
    protected:
        void reorder_names(const std::vector<emc::Name> &names);
        
    public:
        void update(const sym::Constraint *constraint);
        bool empty() const { return _zone.is_empty(); }
        bool universe() const { return _zone.is_universe(); }
        
    public:
        bool contain(const Zone &zone) const;
        ZoneSet make_contain(const Zone &zone, const std::set<emc::Name> names) const;
        ZoneSet make_empty(const std::set<emc::Name> names) const;
        void intersect(const Zone &zone);
        bool equal(const Zone &zone) const;
        
    public:
        std::vector<std::string> info() const;
        friend std::ostream & operator << (std::ostream &os, const Zone &zone);
        void print_info() const;
    };
    
    class ZoneSet {
    protected:
        std::vector<Zone> _zones;
        
    public:
        ZoneSet(bool universe) : _zones(universe ? std::vector<Zone>({Zone(true)}) : std::vector<Zone>()) {}
        ~ZoneSet() {}
        
    public:
        void update(const sym::Constraint *constraint);
        bool universe() const { return _zones.size() == 1 && _zones.back().universe(); }
        bool empty() const { return _zones.empty(); }
        size_t size() const { return _zones.size(); }
        const Zone & at(size_t index) const { return _zones.at(index); }
        Zone & at(size_t index) { return _zones.at(index); }
        
    public:
        void insert(const Zone &zone);
        void intersect(const Zone &zone);
        void multiply(const ZoneSet &zones);
        void append(const ZoneSet &zones);
        
    public:
        friend std::ostream & operator << (std::ostream &os, const ZoneSet &zones);
        void print_info() const;
    };
}

#endif /* __symbolic__ppl__ */
