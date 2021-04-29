//
//  machine.hpp
//  states
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include "typenum.hpp"

namespace states
{
/* A set of links and the operations that can be performed on them and the types associated with them:
 1. a state num which can be any of the from or to states
 2. an event num which can be any of the event states
 3. handle an event which finds the link that has the same from state and the same event and follows it
 4. process a state which means to run the state's operation
 */
template<typename... TLinks>
class Machine
{
public:
    /* list of links */
    typedef TypeList<TLinks...> TLinkList;
    /* list of unique states that are start states */
    typedef TypeListUnique<typename TLinks::TFromType...> TFromStateTypes;
    /* list of unique states that are end states */
    typedef TypeListUnique<typename TLinks::TToType...> TToStateTypes;
    /* list of unique states that are start or end states */
    typedef TypeListUnique<typename TLinks::TFromType..., typename TLinks::TToType...> TStateTypes;
    /* typenum representing a state from the list of unique states */
    typedef TypeNum<TStateTypes> TStateNum;
    /* list of unique events */
    typedef TypeListUnique<typename TLinks::TEventType...> TEventTypes;
    /* typenum representing an event from the list of unique events */
    typedef TypeNum<TEventTypes> TEventNum;

private:
    /* non-unique list of key types */
    typedef TypeList<typename TLinks::TKeyType...> TKeyTypes;
    /* unique list of key types */
    typedef TypeListUnique<typename TLinks::TKeyType...> TUniqueKeyTypes;
    /* asserts that the list of links does not have any duplicate start, event pairs */
    static_assert(TypeListSize<TKeyTypes>::size == TypeListSize<TUniqueKeyTypes>::size,
                  "set of links must have unique set of from/event pairs.");

private:
    /* link case for handle by event type, if relevant, follow link, else try others */
    template<typename TEvent, typename TData, typename TFirst, typename... TOthers>
    static bool handleImpl(TStateNum& state, TData& data)
    {
        if (TFirst::template relevant<TEvent>(state))
        {
            TFirst::follow(state, data);
            return true;
        }
        return handleImpl<TEvent, TData, TOthers...>(state, data);
    }

    /* base case for handle by event type, do nothing */
    template<typename TEvent, typename TData>
    static bool handleImpl(TStateNum& state, TData& data)
    {
        return false;
    }

    /* link case for handle by event num, if relevant, follow link, else try others */
    template<typename TData, typename TFirst, typename... TOthers>
    static bool handleImpl(TStateNum& state, const TEventNum& event, TData& data)
    {
        if (TFirst::relevant(state, event))
        {
            TFirst::follow(state, data);
            return true;
        }
        return handleImpl<TData, TOthers...>(state, event, data);
    }

    /* base case for handle by event num, do nothing */
    template<typename TData>
    static bool handleImpl(TStateNum& state, const TEventNum& event, TData& data)
    {
        return false;
    }

    /* invokes the operation on the data for the state and returns true (always) for success */
    template<typename TData, typename TState>
    static bool invokeImpl(TData& data)
    {
        TState::invoke(data);
        return true;
    }

    /* link case for process, if the link has the same statrt state, process else try the other links */
    template<typename TData, typename TFirst, typename... TOthers>
    static bool processImpl(const TStateNum& state, TData& data)
    {
        typedef typename TFirst::TFromType TState;
        return state.template is<TState>() ? invokeImpl<TData, TState>(data)
                                           : processImpl<TData, TOthers...>(state, data);
    }

    /* base case for process, do nothing */
    template<typename TData>
    static bool processImpl(const TStateNum& state, TData& data)
    {
        return false;
    }

    /* visits the static structure of the machine, by visiting the first link and then each subsequent link */
    template<typename TVisitor, typename TFirst, typename... TOthers>
    static void visitImpl(TVisitor& visitor)
    {
        TFirst::visit(visitor);
        visitImpl<TVisitor, TOthers...>(visitor);
    }

    /* visits the static structure of the machine */
    template<typename TVisitor>
    static void visitImpl(TVisitor& visitor)
    {
    }

public:
    /* handle an event, will change the state, following the appropriate link, performing the link op and the new state
       op returns true if handled */
    template<typename TEvent, typename TData>
    static typename std::enable_if<TypeListContains<TEventTypes, TEvent>::value, bool>::type handle(TStateNum& state,
                                                                                                    TData& data)
    {
        return handleImpl<TEvent, TData, TLinks...>(state, data);
    }

    /* handles the transition from the state using the event given
     advances the state, returns true if tthe state is valid and
     can be advanced */
    template<typename TData>
    static bool handle(TStateNum& state, const TEventNum& event, TData& data)
    {
        return handleImpl<TData, TLinks...>(state, event, data);
    }

    /* process the current given state, without advancing in any way
        returns true if state is valid */
    template<typename TData>
    static bool process(const TStateNum& state, TData& data)
    {
        return processImpl<TData, TLinks...>(state, data);
    }

    /* visit the machine by visiting its links */
    template<typename TVisitor>
    static void visit(TVisitor& visitor)
    {
        visitImpl<TVisitor, TLinks...>(visitor);
    }
};

} // namespace states
