//
//  link.hpp
//  machine
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include "state.hpp"
#include "typenum.hpp"
#include "noop.hpp"

namespace states
{
template<typename TFrom, typename TEvent, typename TTo, typename TLinkOp = NoOp>
class Link
{
public:
    typedef TFrom TFromType;
    typedef TEvent TEventType;
    typedef TTo TToType;

public:
    template<typename... Ts>
    static bool relevant(const TypeNum<Ts...>& state)
    {
        return state.template is<TFrom>();
    }
    template<typename TTestEvent, typename... Ts>
    static bool relevant(const TypeNum<Ts...>& state)
    {
        return std::is_same<TEvent, TTestEvent>::value && relevant(state);
    }
    template<typename TData, typename... Ts>
    static void follow(TypeNum<Ts...>& state, TData& data)
    {
        TLinkOp()(data);
        TTo::become(state, data);
    }
};

} // namespace states
