//
//  typelist.hpp
//  states
//
//  Created by Daniel Pav on 4/1/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include <cstddef>
#include <type_traits>

namespace states
{

//
// TypeListEnd
//

/* placeholder type to signify the end of a typelist.  equivalent to a (char) 0 being used to
 signify the end of a c-style string */
struct TypeListEnd
{
};

//
// TypeList
//

/* recurisve definitions of a typelist ending in a TypeListEnd */
template<typename T, typename... Ts>
struct TypeList
{
    using TCurrentType = T;
    using TNextType = TypeList<Ts...>;
};

template<typename T>
struct TypeList<T>
{
    using TCurrentType = T;
    using TNextType = TypeListEnd;
};

//
// TypeListIndexBase
//

/* type to act as base to hold npos as the no position constant */
struct TypeListIndexBase
{
    static const constexpr size_t npos = (size_t) -1;
};

//
// TypeListIndex
//

/* recursive definition of TypeListIndex returning the 0-based index of the type T in the type list TLIST */
template<typename TList, typename T>
struct TypeListIndex;

template<size_t n, typename T, typename TList, typename TTest>
struct TypeListIndexImpl
{
    using TImpl = TypeListIndexImpl<n + 1, T, typename TList::TNextType, typename TList::TCurrentType>;
    using TSame = std::is_same<T, TTest>;
    static const constexpr size_t index = TSame::value ? n : TImpl::index;
};

template<size_t n, typename T, typename TTest>
struct TypeListIndexImpl<n, T, TypeListEnd, TTest>
{
    using TSame = std::is_same<T, TTest>;
    static const constexpr size_t index = TSame::value ? n : TypeListIndexBase::npos;
};

template<typename TList, typename T>
struct TypeListIndex
{
    using TImpl = TypeListIndexImpl<0, T, typename TList::TNextType, typename TList::TCurrentType>;
    static const constexpr size_t npos = TypeListIndexBase::npos;
    static const constexpr size_t index = TImpl::index;
};

//
// TypeListContains
//

/* uses TypeListIndex to indicate if the type T is in the type list TList */
template<typename TList, typename T>
struct TypeListContains
{
    using TImpl = TypeListIndex<TList, T>;
    static const constexpr bool value = (TImpl::index != TImpl::npos);
};

//
// TypeListAdd
//

/* constructs a typelist by adding a type T to a type list TList */
template<typename TList, typename T>
struct TypeListAdd
{
    using TCurrentType = T;
    using TNextType = TList;
};

//
// TypeListAddUnique
//

/* adds the type T to the type list TList if the type T is not already in the type list T */
template<typename TList, typename T>
struct TypeListAddUnique
{
    using TImpl = typename std::conditional<!TypeListContains<TList, T>::value, TypeListAdd<TList, T>, TList>::type;
    using TCurrentType = typename TImpl::TCurrentType;
    using TNextType = typename TImpl::TNextType;
};

//
// TypeListUnique
//

/* creates a typelist in which each type only appears at most once */
template<typename T, typename... Ts>
struct TypeListUnique
{
    using TImpl = TypeListAddUnique<TypeListUnique<Ts...>, T>;
    using TCurrentType = typename TImpl::TCurrentType;
    using TNextType = typename TImpl::TNextType;
};

template<typename T>
struct TypeListUnique<T>
{
    using TCurrentType = T;
    using TNextType = TypeListEnd;
};

//
// TypeListSize
//

/* computes the size of a type list TList */
template<typename TList>
struct TypeListSize
{
    static constexpr const size_t size = 1 + TypeListSize<typename TList::TNextType>::size;
};

template<>
struct TypeListSize<TypeListEnd>
{
    static constexpr const size_t size = 0;
};

} // namespace states
