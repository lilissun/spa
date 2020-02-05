//
//  timer.cpp
//  common
//
//  Created by Li Li on 13/3/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#include "timer.h"

#define MILLION 1000000
#define THOUSAND 1000

namespace com {
    Timer::Timer() : _paused(true), _accumulated(0)
    {
    }
    
    void Timer::start()
    {
        if (_paused) {
            gettimeofday(&_current_start, NULL);
            _paused = false;
        }
    }
    
    void Timer::pause()
    {
        if (_paused == false) {
            timeval current_pause;
            gettimeofday(&current_pause, NULL);
            _accumulated += (double)(current_pause.tv_sec - _current_start.tv_sec) * THOUSAND;
            _accumulated += (double)(current_pause.tv_usec - _current_start.tv_usec) / THOUSAND;
            _paused = true;
        }
    }
    
    void Timer::reset()
    {
        _accumulated = 0;
        _paused = true;
    }
    
    std::ostream & operator << (std::ostream &os, const Timer &timer)
    {
        os << timer._accumulated << "ms";
        return os;
    }
}