//
//  model.cpp
//  symbolic
//
//  Created by Li Li on 26/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "model.h"

#include <iostream>

namespace sym {
#if defined(DEBUG)
    void Model::print_info() const
    {
        info(std::cout);
        std::cout << std::endl;
    }
#endif
}