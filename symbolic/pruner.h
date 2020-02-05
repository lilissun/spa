//
//  pruner.h
//  symbolic
//
//  Created by Li Li on 11/3/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__pruner__
#define __symbolic__pruner__

#include <map>

#include "common/exception.h"
#include "explicit/name.h"

namespace sym {
    class Pruner {
    protected:
        std::map<emc::Name, size_t> _event_upperbound;
        
    public:
        Pruner() {}
        virtual ~Pruner() {}
        
    public:
        Pruner(const Pruner &pruner) = delete;
        Pruner & operator = (const Pruner &pruner) = delete;
        
    public:
        void upperbound_event(const emc::Name &name, size_t upperbound);
        bool event_validate(const emc::Name &name, size_t count) const;
    };
}

#endif /* defined(__symbolic__pruner__) */
