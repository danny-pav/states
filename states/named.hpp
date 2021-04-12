//
//  named.hpp
//  states
//
//  Created by Daniel Pav on 4/8/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

namespace states
{
/* converts a const char * template parameter into a const char * name function */
template<const char* Name>
class Named
{
public:
    /* returns the name that is the template parameter */
    static const char* name() { return Name; }
};
} // namespace states
