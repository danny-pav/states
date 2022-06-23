//
//  typenum.hpp
//  states
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
/*
 The number of a type given as a template parameter.  The list of template parameters can contain
 duplicates. This has POD semantics.  It is initialized to be INVALID.  Set the value by calling
 clear (sets value to INVALID as if newly constructed) or calling set<TYPE>.   Check the value
 by calling is<TYPE> and calling valid. The functions is and set may only be called with T
 parameters that are in the type list TList.  This will fail to compile if not. */
template<typename TList>
struct TypeNum
{
private:
    /* to inidicate "no type" */
    static const size_t npos = TypeListIndexBase::npos;

public:
    /* returns TRUE if the value corresponds to the type T of the Ts template parameter
     code will not compile if T is not in Ts */
    template<typename T>
    typename std::enable_if<TypeListContains<TList, T>::value, bool>::type is() const
    {
        return index_ == TypeListIndex<TList, T>::index;
    }
    /* returns TRUE if the value is not INVALID, the type can be found in the class's Ts template parameter */
    bool valid() const { return (index_ != npos); }
    /* returns the index value */
    size_t get() const { return index_; }
    
public:
    /* sets the value to INVALID */
    void clear() { index_ = npos; }
    /* sets the value to the index of type T of the Ts template parameter code will not compile
     if T is not in Ts */
    template<typename T>
    typename std::enable_if<TypeListContains<TList, T>::value, void>::type set()
    {
        index_ = TypeListIndex<TList, T>::index;
    }
    /* sets the index */
    bool set(size_t index)
    {
        const bool ok = (index == npos) || ((0 <= index) && (index < TypeListSize<TList>::size));
        if (ok)
            index_ = index;
        return ok;
    }

public:
    /* dumps the internal state (for debugging) */
    void dump(std::ostream& os) const { os << index_; }

private:
    /* index indicating the type, with npos as "no type" */
    size_t index_{npos};
};

/* dumps the internal state (for debugging) */
template<typename TList>
std::ostream& operator<<(std::ostream& os, const TypeNum<TList>& obj)
{
    obj.dump(os);
    return os;
}

} // namespace states
