//
//  model.h
//  symbolic
//
//  Created by Li Li on 26/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __symbolic__model__
#define __symbolic__model__

#include <iostream>

namespace sym {
    class Model {
    public:
        Model() {}
        virtual ~Model() {}
        
    public:
        virtual void info(std::ostream &os) const = 0;
#if defined(DEBUG)
        void print_info() const;
#endif
    };
}

#endif /* defined(__symbolic__model__) */
