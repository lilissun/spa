//
//  node.cpp
//  common
//
//  Created by Li Li on 3/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "node.h"

namespace com {
    Node * encode(void *ptr) {
        return new Node(ptr);
    }
    
    Node * append(Node *first, Node *last) {
        Node *current = first;
        while(current->_next != nullptr) {
            current = current->_next;
        }
        current->_next = last;
        return first;
    }
}