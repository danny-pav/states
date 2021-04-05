//
//  event.hpp
//  machine
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include <typeinfo>

namespace states
{
template<typename TName>
class Event
{
public:
    static const char* name() { return typeid(TName).name(); }
};
} // namespace states
