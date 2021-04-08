//
//  noop.hpp
//  machine
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

namespace states
{
/* */
struct NoOp
{
    /* */
    template<typename T>
    void operator()(T&)
    {
    }
};
} // namespace states
