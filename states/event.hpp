//
//  event.hpp
//  states
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include "named.hpp"
#include <typeinfo>

namespace states
{
/* and event is based on a name to distinguish it */
template<const char* Name>
class Event
{
private:
    /* the implementation of the name */
    typedef Named<Name> TNameImpl;

public:
    /* returns the name */
    static const char* name() { return TNameImpl::name(); }

    /* visit the event by using its name */
    template<typename TVisitor>
    static void visit(TVisitor& visitor)
    {
        visitor.visitEvent(name());
    }
};
} // namespace states
