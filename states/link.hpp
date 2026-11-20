//
//  link.hpp
//  states
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include "allvalid.hpp"
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
 * attempted to be traversed, TValidOp will be invoked.  If it passes, state will move and the TLinkOp is invoked.
 */
template<typename TFrom, typename TEvent, typename TTo, typename TLinkOp = NoOp, typename TValidOp = AllValid>
class Link
{
public:
    /* from type */
    using TFromType = TFrom;
    /* event type */
    using TEventType = TEvent;
    /* to type */
    using TToType = TTo;
    /* key is From, Event pair */
    using TKeyType = LinkKey<TFrom, TEvent>;

public:
    /* returns true if this link is relevant to this state and the event,
     by which the state is the starting state for the link and the event matches */
    template<typename TStateNum, typename TEventNum>
    static constexpr bool relevant(const TStateNum& state, const TEventNum& event)
    {
        return event.template is<TEvent>() && state.template is<TFrom>();
    }

    /* returns true if this link is relevant to this state and the event,
     by which the state is the starting state for the link and the event matches */
    template<typename TTestEvent, typename TStateNum>
    static constexpr bool relevant(const TStateNum& state)
    {
        return std::is_same<TEvent, TTestEvent>::value && state.template is<TFrom>();
    }

    /* follow the link, by running the link operation on the data and the state operation on the data, changing the
     state to the new state 
     returns true if successfully followed 
    */
    template<typename TData, typename TStateNum>
    static bool follow(TStateNum& state, TData& data)
    {
        const TData& const_data = data;
        const TValidOp valid_op;
        const bool is_valid = valid_op(const_data);
        if (!is_valid) 
            return false;
        const TLinkOp link_op;
        link_op(data);
        TTo::become(state, data);
        return true;
    }

    /* visit the link by visiting the start and end state and then the event */
    template<typename TVisitor>
    static void visit(TVisitor& visitor)
    {
        visitor.preLink();
        TFrom::visit(visitor);
        visitor.inLink1();
        TTo::visit(visitor);
        visitor.inLink2();
        TEvent::visit(visitor);
        visitor.postLink();
    }
};

} // namespace states
