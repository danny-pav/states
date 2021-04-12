//
//  noop.hpp
//  states
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

namespace states
{
/* provides a default to do nothing as an action */
struct NoOp
{
    /* does nothing */
    template<typename T>
    void operator()(T&)
    {
    }
};
} // namespace states
