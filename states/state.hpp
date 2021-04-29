//
//  state.hpp
//  states
//
//  Created by Daniel Pav on 4/1/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//
#pragma once

#include "named.hpp"
#include "noop.hpp"
#include "typenum.hpp"

namespace states
{
/* models a state by which has a name and an operation that is run on becoming that state */
template<const char* TName, typename TStateOp = NoOp>
class State
{
private:
    /* this type */
    typedef State<TName, TStateOp> TThisType;
    /* the implementation of the name */
    typedef Named<TName> TNameImpl;

public:
    /* returns the name of the state (as given by template paramter) */
    static const char* name() { return TNameImpl::name(); }

    /* runs the state operation on the data provided */
    template<typename TData>
    static void invoke(TData& data)
    {
        TStateOp()(data);
    }

    /* sets the state to this state and invokes the state operation on the data provided */
    template<typename TData, typename... Ts>
    static void become(TypeNum<Ts...>& state, TData& data)
    {
        state.template set<TThisType>();
        invoke(data);
    }

    /* visit the state using its name */
    template<typename TVisitor>
    static void visit(TVisitor& visitor)
    {
        visitor.visitState(name());
    }
};

} // namespace states
