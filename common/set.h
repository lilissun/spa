//
//  set.h
//  common
//
//  Created by Li Li on 1/8/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __common__set__
#define __common__set__

#include <iostream>
#include <set>

namespace com {
    template<typename _Compare, typename _InputIterator1, typename _InputIterator2>
    bool
    __set_intersection_is_empty(_InputIterator1 __first1, _InputIterator1 __last1,
                       _InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp)
    {
        while (__first1 != __last1 && __first2 != __last2)
        {
            if (__comp(*__first1, *__first2))
                ++__first1;
            else
            {
                if (!__comp(*__first2, *__first1))
                {
                    return false;
                }
                ++__first2;
            }
        }
        return true;
    }
    
    template <class _InputIterator1, class _InputIterator2, class _Compare>
    inline _LIBCPP_INLINE_VISIBILITY
    bool
    set_intersection_is_empty(_InputIterator1 __first1, _InputIterator1 __last1,
                     _InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp)
    {
#ifdef _LIBCPP_DEBUG
        typedef typename add_lvalue_reference<__debug_less<_Compare> >::type _Comp_ref;
        __debug_less<_Compare> __c(__comp);
        return __set_intersection<_Comp_ref>(__first1, __last1, __first2, __last2, __result, __c);
#else  // _LIBCPP_DEBUG
        typedef typename std::add_lvalue_reference<_Compare>::type _Comp_ref;
        return __set_intersection_is_empty<_Comp_ref>(__first1, __last1, __first2, __last2, __comp);
#endif  // _LIBCPP_DEBUG
    }
    
    template <class _InputIterator1, class _InputIterator2>
    inline _LIBCPP_INLINE_VISIBILITY
    bool
    set_intersection_is_empty(_InputIterator1 __first1, _InputIterator1 __last1,
                     _InputIterator2 __first2, _InputIterator2 __last2)
    {
        return set_intersection_is_empty(__first1, __last1, __first2, __last2,
                                       std::__less<typename std::iterator_traits<_InputIterator1>::value_type,
                                       typename std::iterator_traits<_InputIterator2>::value_type>());
    }
    
    
}

#endif /* defined(__common__set__) */
