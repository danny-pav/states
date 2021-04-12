//
//  process.hpp
//  states
//
//  Created by Daniel Pav on 4/2/21.
//  Copyright © 2021 Daniel Pav. All rights reserved.
//

#pragma once

namespace states
{
/* */
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
    typedef typename TMachine::TStateNum TStateNum;
    /* the event num type using the events from the machine given */
    typedef typename TMachine::TEventNum TEventNum;

public:
    /* sets the process to no-state, equivalent to newly constructed */
    void reset() { state_.clear(); }

    /* sets the state to the TBegin state, returns true if state is in the link set as a from state */
    bool start()
    {
        state_.template set<TBegin>();
        if (TMachine::process(state_, data_))
            return true;
        reset();
        return false;
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

private:
    TStateNum state_;
    TData& data_;
};

} // namespace states
