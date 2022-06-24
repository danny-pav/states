//
//  nextevent.hpp
//  states
//
//  Created by Daniel Pav on 6/24/22.
//  Copyright © 2022 Daniel Pav. All rights reserved.
//

#ifndef nextevent_hpp
#define nextevent_hpp

#include "typelist.hpp"

namespace states
{
/* functions for turning an event into a value and applying that value to a process as an event.  This allows for the state operations to use this to store a size_t as the next event.
 */
template <typename ...TEvents>
struct NextEvent
{
public:
    using Index = size_t;
    
private:
    static const constexpr Index npos = TypeListIndexBase::npos;
    using TEventList = TypeList<TEvents...>;
    using TEventListSize = TypeListSize<TEventList>;
    
private:
    template <typename TProcess, Index N>
    static bool applyIndexImpl(TProcess& process, Index index, std::true_type)
    {
        if (index == N)
        {
            using TEvent = typename TypeListAt<TEventList, N>::TType;
            return process.template next<TEvent>();
        }
        return applyIndex<TProcess, N + 1>(process, index);
    }
    template <typename TProcess, Index N>
    static bool applyIndexImpl(TProcess& process, Index index, std::false_type)
    {
        return false;
    }
    template<typename TProcess, size_t N>
    static bool applyIndex(TProcess& process, Index index)
    {
        using TInRange = std::integral_constant<bool, (0 <= N) && (N < TEventListSize::size)>;
        return applyIndexImpl<TProcess, N>(process, index, TInRange());
    }

public:
    static Index none()
    {
        return npos;
    }
    template <typename TEvent>
    static typename std::enable_if<TypeListContains<TEventList, TEvent>::value, Index>::type store()
    {
        return TypeListIndex<TEventList, TEvent>::index;
    }
    template <typename TProcess>
    static bool apply(TProcess& process, Index index)
    {
        return (index != npos) ? applyIndex<TProcess, 0>(process, index) : false;
    }
};
}

#endif /* nextevent_hpp */
