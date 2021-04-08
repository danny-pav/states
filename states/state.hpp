//
//  state.hpp
//  states
//
//  Created by Daniel Pav on 4/1/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//
#pragma once

#include "named.hpp"
#include "noop.hpp"
#include "typenum.hpp"

namespace states
{
template<const char* TName, typename TStateOp = NoOp>
class State
{
private:
    /* */
    typedef State<TName, TStateOp> TThisType;
    /* */
    typedef Named<TName> TNameImpl;

public:
    /* */
    static const char* name() { return TNameImpl::name(); }

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
