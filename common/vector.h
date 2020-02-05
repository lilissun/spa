//
//  vector.h
//  common
//
//  Created by Li Li on 5/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __common__vector__
#define __common__vector__

#include <vector>

namespace com {
    template <typename T>
    inline _LIBCPP_INLINE_VISIBILITY
    typename std::enable_if<std::is_copy_constructible<T>::value, bool>::type
    unsorted_unique(const std::vector<T> &v) {
        auto copy = v;
        std::sort(copy.begin(), copy.end());
        return std::unique(copy.begin(), copy.end());
    }
}

#endif /* defined(__common__vector__) */
