//
//  machine.hpp
//  machine
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
    /* */
    typedef TypeListUnique<typename TLinks::TFromType..., typename TLinks::TToType...> TStateTypes;
    /* */
    typedef TypeNum<TStateTypes> TStateNum;
    /* */
    typedef TypeListUnique<typename TLinks::TEventType...> TEventTypes;
    /* */
    typedef TypeNum<TEventTypes> TEventNum;

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
    /* handle an event, will change the state, following the appropriate link, performing the link op and the new state op
        returns true if handled */
    template<typename TEvent, typename TData>
    static bool handle(TStateNum& state, TData& data)
    {
        return handleImpl<TEvent, TData, TLinks...>(state, data);
    }

    /* */
    template<typename TData>
    static bool handle(TStateNum& state, const TEventNum& event, TData& data)
    {
        return handleImpl<TData, TLinks...>(state, event, data);
    }

    /* process the current given state */
    template<typename TData>
    static bool process(const TStateNum& state, TData& data)
    {
        return processImpl<TData, TLinks...>(state, data);
    }
};

} // namespace states
