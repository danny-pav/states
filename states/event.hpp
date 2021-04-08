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
/* */
template<const char* Name>
class Event
{
private:
    typedef Named<Name> TNameImpl;

public:
    /* */
    static const char* name() { return TNameImpl::name(); }
};
} // namespace states
