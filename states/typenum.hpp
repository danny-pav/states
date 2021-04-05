//
//  typenum.hpp
//  machine
//
//  Created by Daniel Pav on 4/1/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include <cstddef>
#include <ostream>
#include <type_traits>

#include "typelist.hpp"

namespace states
{
template<typename... Ts>
struct TypeNum
{
private:
    static const size_t INVALID{0};
    static const size_t FIRST{1};

private:
    template<size_t n, typename T, typename TFirst, typename... TOthers>
    size_t getImpl(TypeList<TFirst, TOthers...>) const
    {
        return std::is_same<T, TFirst>::value ? n : getImpl<n + 1, T>(TypeList<TOthers...>{});
    }
    template<size_t n, typename T>
    size_t getImpl(TypeList<>) const
    {
        return INVALID;
    }

public:
    template<typename T>
    bool is() const
    {
        return index_ == getImpl<FIRST, T>(TypeList<Ts...>{});
    }
    bool valid() const { return (index_ != INVALID); }

public:
    void clear() { index_ = INVALID; }
    template<typename T>
    void set()
    {
        index_ = getImpl<FIRST, T>(TypeList<Ts...>{});
    }

public:
    void dump(std::ostream& os) const { os << index_; }

private:
    size_t index_{INVALID};
};

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, const TypeNum<Ts...>& obj)
{
    obj.dump(os);
    return os;
}
} // namespace states
