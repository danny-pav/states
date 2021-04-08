//
//  state.hpp
//  machine
//
//  Created by Daniel Pav on 4/1/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//
#pragma once

#include "noop.hpp"
#include "typenum.hpp"

namespace states
{
/* */
template<typename TName, typename TStateOp = NoOp>
class State
{
public:
    /* */
    typedef State<TName, TStateOp> TThisType;

public:
    /* */
    static const char* name() { return typeid(TName).name(); }

    /* */
    template<typename TData>
    static void invoke(TData& data)
    {
        TStateOp()(data);
    }

    /* */
    template<typename TData, typename... Ts>
    static void become(TypeNum<Ts...>& state, TData& data)
    {
        state.template set<TThisType>();
        invoke(data);
    }
};

} // namespace states
