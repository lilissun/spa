//
//  order.h
//  explicit
//
//  Created by Li Li on 9/4/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#ifndef __explicit__order__
#define __explicit__order__

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "name.h"

namespace emc {
    class Order {
    protected:
        bool _sortable;
        std::map<Name, std::set<Name>> _order; // the order is strict, <a, a> does not exist
        
    public:
        Order() : _sortable(true), _order() {}
        Order(const Order &order) : _sortable(order._sortable), _order(order._order) {}
        Order & operator = (const Order &order) { _sortable = order._sortable; _order = order._order; return *this; }
        
    public:
        bool insert(Name left, Name right);
        bool insert(Name left, std::set<Name> rights);
        bool rename(Name from, Name to);
        bool remove(Name name);
        void clear() { _order.clear(); _sortable = true; }
        std::set<Name> keys() const;
        const std::set<Name> & values(const Name &key) const { return _order.at(key); }
        
    public:
        bool exist_pair(const Name &key, const Name &value) const;
        bool exist_key(const Name &key) const;
        
    public:
        bool sortable() const { return _sortable; }
    };
}

#endif /* __explicit__order__ */
