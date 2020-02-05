//
//  some.h
//  explicit
//
//  Created by Li Li on 19/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __explicit__some__
#define __explicit__some__

#include <iostream>

#include "common/runtime.h"

namespace emc {
    template <typename T>
    class Some {
    protected:
        bool _empty;
        T _value;
        
    public:
        Some() : _empty(true), _value() {}
        Some(const T &value) : _empty(false), _value(value) {}
        Some(const Some &some) : _empty(some._empty), _value(some._value) {}
        ~Some() {}
        
    public:
        bool empty() const { return _empty; }
        const T & value() const { ASSERT(_empty == false); return _value; }
        void value(const T &value) { _empty = false; _value = value; }
        void clear() { _empty = true; }
        
    public:
        Some & operator = (const Some &some) {
            _empty = some._empty;
            _value = some._value;
            return *this;
        }
        
    public:
        bool operator == (const Some &some) const {
            if (_empty != some._empty) {
                return false;
            }
            
            if (_empty) {
                return true;
            } else {
                return _value == some._value;
            }
        }
        bool operator != (const Some &some) const { return !(*this == some); }

    public:
        friend std::ostream & operator << (std::ostream &os, const Some &some) {
            if (some._empty) {
                os << "[@empty]";
            } else {
                os << "[" << some._value << "]";
            }
            return os;
        }
    };
}

#endif /* defined(__explicit__some__) */
