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
template<typename... TLinks>
class Machine
{
public:
    typedef TypeList<typename TLinks::TFromType...> TFromTypes;
    typedef TypeList<typename TLinks::TToType...> TToTypes;
    typedef TypeList<typename TLinks::TFromType..., typename TLinks::TToType...> TStateTypes;
    typedef TypeNum<typename TLinks::TFromType..., typename TLinks::TToType...> TStateNum;
    typedef TypeList<typename TLinks::TEventType...> TEventTypes;
    typedef TypeNum<typename TLinks::TEventType...> TEventNum;

private:
    template<typename TEvent, typename TData, typename TFirst, typename... TOthers>
    static bool handleImpl(TStateNum& state, TData& data, TypeList<TFirst, TOthers...>)
    {
        if (TFirst::template relevant<TEvent>(state))
        {
            TFirst::follow(state, data);
            return true;
        }
        return handleImpl<TEvent>(state, data, TypeList<TOthers...>{});
    }
    template<typename TEvent, typename TData>
    static bool handleImpl(TStateNum& state, TData& data, TypeList<>)
    {
        return false;
    }

    template<typename TData, typename TFirst, typename... TOthers>
    static bool handleImpl(TStateNum& state, const TEventNum& event, TData& data, TypeList<TFirst, TOthers...>)
    {
        typedef typename TFirst::TEventType TFirstEvent;
        if (event.template is<TFirstEvent>() && TFirst::relevant(state))
        {
            TFirst::follow(state, data);
            return true;
        }
        return handleImpl(state, event, data, TypeList<TOthers...>{});
    }
    template<typename TData>
    static bool handleImpl(TStateNum& state, const TEventNum& event, TData& data, TypeList<>)
    {
        return false;
    }

    template<typename TData, typename TFirst, typename... TOthers>
    static bool processImpl(const TStateNum& state, TData& data, TypeList<TFirst, TOthers...>)
    {
        if (TFirst::relevant(state))
        {
            TFirst::TFromType::invoke(data);
            return true;
        }
        return processImpl(state, data, TypeList<TOthers...>{});
    }
    template<typename TData>
    static bool processImpl(const TStateNum& state, TData& data, TypeList<>)
    {
        return false;
    }

public:
    template<typename TEvent, typename TData>
    static bool handle(TStateNum& state, TData& data)
    {
        return handleImpl<TEvent>(state, data, TypeList<TLinks...>{});
    }
    template<typename TData>
    static bool handle(TStateNum& state, const TEventNum& event, TData& data)
    {
        return handleImpl(state, event, data, TypeList<TLinks...>{});
    }
    template<typename TData>
    static bool process(const TStateNum& state, TData& data)
    {
        return processImpl(state, data, TypeList<TLinks...>{});
    }
};
} // namespace states
