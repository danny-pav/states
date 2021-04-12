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
/* */
template<typename... TLinks>
class Machine
{
public:
    /* list of unique states that are start or end states */
    typedef TypeListUnique<typename TLinks::TFromType..., typename TLinks::TToType...> TStateTypes;
    /* typenum representing a state from the list of unique states */
    typedef TypeNum<TStateTypes> TStateNum;
    /* list of unique events */
    typedef TypeListUnique<typename TLinks::TEventType...> TEventTypes;
    /* typenum representing an event from the list of unique events */
    typedef TypeNum<TEventTypes> TEventNum;

private:
    /* */
    typedef TypeList<typename TLinks::TKeyType...> TKeyTypes;
    /* */
    typedef TypeListUnique<typename TLinks::TKeyType...> TUniqueKeyTypes;
    /* asserts that the list of links does not have any duplicate start, event pairs */
    static_assert(TypeListSize<TKeyTypes>::size == TypeListSize<TUniqueKeyTypes>::size,
                  "set of links must have unique set of from/event pairs.");

private:
    /* */
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

    /* */
    template<typename TEvent, typename TData>
    static bool handleImpl(TStateNum& state, TData& data)
    {
        return false;
    }

    /* */
    template<typename TData, typename TFirst, typename... TOthers>
    static bool handleImpl(TStateNum& state, const TEventNum& event, TData& data)
    {
        typedef typename TFirst::TEventType TFirstEvent;
        if (event.template is<TFirstEvent>() && TFirst::relevant(state))
        {
            TFirst::follow(state, data);
            return true;
        }
        return handleImpl<TData, TOthers...>(state, event, data);
    }

    /* */
    template<typename TData>
    static bool handleImpl(TStateNum& state, const TEventNum& event, TData& data)
    {
        return false;
    }

    /* */
    template<typename TData, typename TFirst, typename... TOthers>
    static bool processImpl(const TStateNum& state, TData& data)
    {
        if (TFirst::relevant(state))
        {
            TFirst::TFromType::invoke(data);
            return true;
        }
        return processImpl<TData, TOthers...>(state, data);
    }

    /* */
    template<typename TData>
    static bool processImpl(const TStateNum& state, TData& data)
    {
        return false;
    }

public:
    /* handle an event, will change the state, following the appropriate link, performing the link op and the new state
       op returns true if handled */
    template<typename TEvent, typename TData>
    static bool handle(TStateNum& state, TData& data)
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
};

} // namespace states
