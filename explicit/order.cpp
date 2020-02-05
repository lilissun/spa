//
//  order.cpp
//  explicit
//
//  Created by Li Li on 9/4/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#include "order.h"

namespace emc {
    bool Order::insert(Name left, Name right)
    {
        // ensure this is sorteble
        if (_sortable == false) {
            return false;
        }
        
        // ensure left and right are not the same
        if (left == right) {
            _sortable = false;
            return false;
        }
        
        // ensure (right, left) does not exist
        auto right_ite = _order.find(right);
        if (right_ite != _order.end() && right_ite->second.find(left) != right_ite->second.end()) {
            _sortable = false;
            return false;
        }
        
    
        // add (left, right) to _order
        auto left_ite = _order.find(left);
        if (left_ite == _order.end()) {
            _order.insert(std::make_pair(left, std::set<Name>({right})));
        } else {
            left_ite->second.insert(right);
        }
        // forall x: (right, x) in this, add (left, x) to _order
        if (right_ite != _order.end()) {
            _order.at(left).insert(right_ite->second.begin(), right_ite->second.end());
        }
        
        // forall x, y: (x, left) and (right, y) in this, add (x, right) and (x, y) to _order
        // (right, x) and (y, x) musts not exist because of the non-existence of (right, left)
        for (auto i = _order.begin(); i != _order.end(); ++i) {
            if (i->second.find(left) != i->second.end()) {
                i->second.insert(right);
                if (right_ite != _order.end()) {
                    i->second.insert(right_ite->second.begin(), right_ite->second.end());
                }
            }
        }
        
        return true;
    }
    
    bool Order::insert(Name left, std::set<Name> rights)
    {
        for (auto right_ite = rights.begin(); right_ite != rights.end(); ++right_ite) {
            if (insert(left, *right_ite) == false) {
                return false;
            }
        }
        return true;
    }
    
    bool Order::rename(Name from, Name to)
    {
        if (_sortable == false) {
            return false;
        }
        
        if (exist_pair(from, to)) {
            _sortable = false;
            return false;
        }
        
        if (exist_pair(to, from)) {
            _sortable = false;
            return false;
        }
        
        if (insert(from, to) == false) {
            return false;
        }
        _order.at(from).erase(to);
        
        if (insert(to, from) == false) {
            return false;
        }
        
        _order.erase(from);
        for (auto i = _order.begin(); i != _order.end(); ++i) {
            i->second.erase(from);
        }
        
        return true;
    }
    
    bool Order::remove(Name name)
    {
        if (_sortable == false) {
            return false;
        }
        
        _order.erase(name);
        for (auto i = _order.begin(); i != _order.end(); ++i) {
            i->second.erase(name);
        }
        return true;
    }
    
    std::set<Name> Order::keys() const
    {
        std::set<Name> keys;
        for (auto i = _order.begin(); i != _order.end(); ++i) {
            keys.insert(i->first);
        }
        return keys;
    }
    
    bool Order::exist_pair(const Name &key, const Name &value) const
    {
        auto key_ite = _order.find(key);
        if (key_ite == _order.end()) {
            return false;
        } else if (key_ite->second.find(value) == key_ite->second.end()) {
            return false;
        } else {
            return true;
        }
    }
    
    bool Order::exist_key(const Name &key) const
    {
        auto key_ite = _order.find(key);
        if (key_ite == _order.end()) {
            return false;
        } else {
            return true;
        }
    }
}