//
//  named.hpp
//  machine
//
//  Created by Daniel Pav on 4/8/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

namespace states
{
template<const char * Name>
class Named
{
public:
    /* */
    static const char* name() { return Name; }
};
}
