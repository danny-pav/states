//
//  link.hpp
//  states
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include "noop.hpp"
#include "state.hpp"
#include "typenum.hpp"

namespace states
{
template<typename TFrom, typename TEvent>
struct LinkKey
{
};

/* */
template<typename TFrom, typename TEvent, typename TTo, typename TLinkOp = NoOp>
class Link
{
public:
    /* */
    typedef TFrom TFromType;
    /* */
    typedef TEvent TEventType;
    /* */
    typedef TTo TToType;
    /* */
    typedef LinkKey<TFrom, TEvent> TKeyType;

public:
    /* */
    template<typename TStateNum>
    static bool relevant(const TStateNum& state)
    {
        return state.template is<TFrom>();
    }

    /* */
    template<typename TTestEvent, typename TStateNum>
    static bool relevant(const TStateNum& state)
    {
        return std::is_same<TEvent, TTestEvent>::value && relevant(state);
    }

    /* */
    template<typename TData, typename TStateNum>
    static void follow(TStateNum& state, TData& data)
    {
        TLinkOp()(data);
        TTo::become(state, data);
    }
};

} // namespace states
