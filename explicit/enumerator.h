//
//  enumerator.h
//  explicit
//
//  Created by Li Li on 3/1/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __explicit__enumerator__
#define __explicit__enumerator__

#include <iostream>
#include <vector>
#include "common/runtime.h"

namespace emc {
    class Enumerator {
    protected:
        std::vector<size_t> _current;
        std::vector<size_t> _dimensions;
        
    public:
        Enumerator() {}
        ~Enumerator() {}
        
    public:
        void add_dimension(size_t size) {
            _dimensions.push_back(size);
            _current.push_back(0);
        }
        
        const std::vector<size_t> & current() {
            return _current;
        }
        
        bool next() {
            ASSERT(_current.size() == _dimensions.size());
            size_t i = 0;
            while (i != _current.size()) {
                _current.at(i) += 1;
                if (_current.at(i) == _dimensions.at(i)) {
                    _current.at(i) = 0;
                    ++i;
                } else {
                    break;
                }
            }
            return i != _current.size();
        }
    };
}

#endif /* defined(__explicit__enumerator__) */
