//
//  timer.h
//  common
//
//  Created by Li Li on 13/3/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#ifndef __common__timer__
#define __common__timer__

#include <iostream>
#include <ctime>
#include <sys/time.h>

namespace com {
    const std::string timer_space = " ";
    
    class Timer {
    private:
        bool _paused;
        timeval _current_start;
        double _accumulated;
        
    public:
        Timer();
        
    public:
        void start();
        void pause();
        void reset();
        
        friend std::ostream & operator << (std::ostream &os, const Timer &timer);
    };
}

#endif /* __common__timer__ */
