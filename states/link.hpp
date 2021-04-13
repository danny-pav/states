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
/* structure with the from state and event.  represents the unique key of links in a machine */
template<typename TFrom, typename TEvent>
struct LinkKey
{
};

/* represents a transition in a state diagram.  indicates a link from TFrom to TTo when TEvent occurs.  When this is
 * traverse the TLinkOp is invoked.
 */
template<typename TFrom, typename TEvent, typename TTo, typename TLinkOp = NoOp>
class Link
{
public:
    /* from type */
    typedef TFrom TFromType;
    /* event type */
    typedef TEvent TEventType;
    /* to type */
    typedef TTo TToType;
    /* key is From, Event pair */
    typedef LinkKey<TFrom, TEvent> TKeyType;

public:
    /* returns true if this link is relevant to this state, by which the state is the starting state for the link */
    template<typename TStateNum>
    static bool relevant(const TStateNum& state)
    {
        return state.template is<TFrom>();
    }

    /* returns true if the link is relevant to the state and the event */
    template<typename TTestEvent, typename TStateNum>
    static bool relevant(const TStateNum& state)
    {
        return std::is_same<TEvent, TTestEvent>::value && relevant(state);
    }

    /* follow the link, by running the link operation on the data and the state operation on the data, changing the
     state to the new state */
    template<typename TData, typename TStateNum>
    static void follow(TStateNum& state, TData& data)
    {
        TLinkOp()(data);
        TTo::become(state, data);
    }
};

} // namespace states
