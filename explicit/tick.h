//
//  tick.h
//  explicit
//
//  Created by Li Li on 22/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __explicit__tick__
#define __explicit__tick__

#include <iostream>

namespace emc {
    class Tick {
    protected:
        size_t _tick;
        
    public:
        Tick() : _tick(0) {}
        Tick(size_t count) : _tick(count) {}
        ~Tick() {}
        
    public:
        Tick(const Tick &) = delete;
        Tick & operator = (const Tick &) = delete;
        
    public:
        size_t current() const { return _tick; }
        size_t next() { return ++_tick; }
        bool jump(size_t tick) { if (_tick < tick) { _tick = tick;  return true; } else { return false; } }
    };
}

#endif /* defined(__explicit__tick__) */
