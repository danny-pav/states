#pragma once

namespace states
{
struct AllValid
{
    template <typename TData>
    bool operator()(const TData&) const
    {
        return true;
    }
};
} // namespace states
