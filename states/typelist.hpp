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
struct TypeListEnd
{
};

//
//
//

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
//
//

struct TypeListIndexBase
{
    static const constexpr size_t npos = (size_t) -1;
};

//
//
//

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
//
//

template<typename TList, typename T>
struct TypeListContains
{
    using TImpl = TypeListIndex<TList, T>;
    static const constexpr bool value = (TImpl::index != TImpl::npos);
};

//
//
//

template<typename TList, typename T>
struct TypeListAdd
{
    using TCurrentType = T;
    using TNextType = TList;
};

//
//
//

template<typename TList, typename T>
struct TypeListAddUnique
{
    using TImpl = typename std::conditional<!TypeListContains<TList, T>::value,
                                            TypeListAdd<TList, T>, TList>::type;
    using TCurrentType = typename TImpl::TCurrentType;
    using TNextType = typename TImpl::TNextType;
};

//
//
//

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
//
//

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
