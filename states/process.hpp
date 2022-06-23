//
//  process.hpp
//  states
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

#include <type_traits>

#include "typelist.hpp"

namespace states
{
/* forward declare */
template<typename TMachine, typename TBegin, typename TEnd>
struct Reachable;

/* determines if the link goes from begin to end.  if so, value is true */
template<typename TBegin, typename TEnd, typename TLink>
struct Match
{
    using TSameBegin = std::is_same<TBegin, typename TLink::TFromType>;
    using TSameEnd = std::is_same<TEnd, typename TLink::TToType>;
    static const constexpr bool value = TSameBegin::value && TSameEnd::value;
};

/* forward declare */
template<typename TMachine, typename TBegin, typename TEnd, typename TVisited, typename TLinks>
struct ReachableImpl;

/* implementation for following a link that should not be followed so this is a dead end */
template<bool TFollow, typename TMachine, typename TBegin, typename TEnd, typename TVisited>
struct FollowImpl
{
    static const constexpr bool value = false;
};

/* implementation for following a link that should be followed, returns true if reachable */
template<typename TMachine, typename TBegin, typename TEnd, typename TVisited>
struct FollowImpl<true, TMachine, TBegin, TEnd, TVisited>
{
    using TNewVisited = TypeListAddUnique<TVisited, TBegin>;
    using TImpl = ReachableImpl<TMachine, TBegin, TEnd, TNewVisited, typename TMachine::TLinkList>;
    static const constexpr bool value = TImpl::value;
};

/* reachable using this link or the next link or if the link can be followed, by following that link */
template<typename TMachine, typename TBegin, typename TEnd, typename TVisited, typename TLinks>
struct ReachableImpl
{
    using TLink = typename TLinks::TCurrentType;
    using TMatch = Match<TBegin, TEnd, TLink>;
    using TSameBegin = typename TMatch::TSameBegin;
    using TNewTo = typename TLink::TToType;
    using TInVisited = TypeListContains<TVisited, typename TLink::TToType>;
    static const constexpr bool follow = TSameBegin::value && !TInVisited::value;
    using TUsingNext = FollowImpl<follow, TMachine, TNewTo, TEnd, TVisited>;
    static const constexpr bool value = TMatch::value ||
        ReachableImpl<TMachine, TBegin, TEnd, TVisited, typename TLinks::TNextType>::value ||
        TUsingNext::value;
};

/* reachable for end of link list: not reachable */
template<typename TMachine, typename TBegin, typename TEnd, typename TVisited>
struct ReachableImpl<TMachine, TBegin, TEnd, TVisited, TypeListEnd>
{
    static const constexpr bool value = false;
};

/* value is whether TEnd is reachable from TBegin using TMachine */
template<typename TMachine, typename TBegin, typename TEnd>
struct Reachable
{
    using TVisited = TypeList<TBegin>;
    using TImpl = ReachableImpl<TMachine, TBegin, TEnd, TVisited, typename TMachine::TLinkList>;
    static const constexpr bool value = TImpl::value;
};

/* represents a process.  Process traverses the links of TMachine from TBegin to TEnd, operating on the TData given.
 Create the object with a reference to the data.  This will be operated on during its use.  Then run the process by
 calling start and repeatedly calling next and checking done.  Return value of next will indicate if a relevant event is
 being called.  The done function will indicate when the TEnd state has been reached. A process object can be reused by
 calling reset.  This sets the process back to the newly constructed state.  Or start can be called again to return to
 the start state. [It is not necessary to call reset before calling start.] Start is not called automaticly because
 start may invoke an operation on the data given and did not want this to be an issue when the data being passed is a
 reference to an owning object.
 */
template<typename TMachine, typename TBegin, typename TEnd, typename TData>
class Process
{
public:
    /* creates a process setting the internal state to invalid (equivalent to reset) and storing a reference to the data
     */
    Process(TData& data) : state_(), data_(data) {}
    /* destroys the process */
    ~Process() = default;

private:
    Process(const Process&) = delete;
    Process(Process&&) = delete;
    Process& operator=(const Process&) = delete;
    Process& operator=(Process&&) = delete;

public:
    /* the state num type using the states form the machine given */
    using TStateNum = typename TMachine::TStateNum;
    /* the event num type using the events from the machine given */
    using TEventNum = typename TMachine::TEventNum;
    /* asserts that the begin state is in the from states */
    static_assert(TypeListContains<typename TMachine::TFromStateTypes, TBegin>::value, "");
    /* asserts that the end state is in the to states */
    static_assert(TypeListContains<typename TMachine::TToStateTypes, TEnd>::value, "");
    /* make sure the end is reachable from the begin */
    static_assert(Reachable<TMachine, TBegin, TEnd>::value, "End not reachable from Begin");
    
public:
    /* sets the process to no-state, equivalent to newly constructed */
    void reset() { state_.clear(); }

    /* sets the state to the TBegin state */
    void start()
    {
        state_.template set<TBegin>();
        TMachine::process(state_, data_);
    }

    /* processes the event given, calling the link op, then the state op, returns true if link exists */
    bool next(const TEventNum& event) { return state_.valid() ? TMachine::handle(state_, event, data_) : false; }

    /* processes the event given, calling the link op, then the state op, returns true if link exists */
    template<typename TEvent>
    bool next()
    {
        return state_.valid() ? TMachine::template handle<TEvent>(state_, data_) : false;
    }

    /* returns true if at the state specified */
    template<typename TState>
    bool at() const
    {
        return state_.template is<TState>();
    }

    /* invokes the state op for the current state, returns true if at a state */
    bool invoke() { return state_.valid() ? TMachine::process(state_, data_) : false; }

    /* returns true if at the TEnd state, equivalent to at<TEnd>() */
    bool done() const { return state_.template is<TEnd>(); }

    /* visits the process by visiting its machine and its begin and end states */
    template<typename TVisitor>
    static void visit(TVisitor& visitor)
    {
        visitor.preProcess();
        visitor.visitBegin(TBegin::name());
        TMachine::visit(visitor);
        visitor.visitEnd(TEnd::name());
        visitor.postProcess();
    }

private:
    /* the current state, may be invalid if reset */
    TStateNum state_;
    /* reference to the data to be operated on during processing */
    TData& data_;
};

} // namespace states
