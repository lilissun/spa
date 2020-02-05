//
//  pruner.cpp
//  symbolic
//
//  Created by Li Li on 11/3/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "pruner.h"

#include "common/runtime.h"

namespace sym {
    void Pruner::upperbound_event(const emc::Name &name, size_t upperbound)
    {
        auto res = _event_upperbound.insert(std::make_pair(name, upperbound));
        if (res.second == false) {
            USER_ERROR("@provide upperbound duplicated for @event " + name.name());
        }
        return ;
    }
    
    bool Pruner::event_validate(const emc::Name &name, size_t count) const
    {
        auto index = _event_upperbound.find(name);
        if (index == _event_upperbound.end()) {
            return true;
        } else if (count <= index->second) {
            return true;
        } else {
            return false;
        }
    }
}