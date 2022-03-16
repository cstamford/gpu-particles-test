#pragma once

#include <borealis/util/Trace.hpp>

#include <atomic>
#include <list>
#include <vector>

namespace br::gfx::impl
{
    template <typename T, typename Id>
    class RenderData
    {
    private:
        std::vector<T> _data;
        std::list<Id> _free;
        std::atomic_uint32_t _next = 0;

    public:
        T& operator[](const Id id)
        {
            BR_ASSERT(id.idx != INVALID_IDX);
            return _data[id.idx];
        }

        Id next()
        {
            if (!_free.empty())
            {
                Id id = _free.front();
                _free.pop_front();
                return id;
            }

            const uint32_t next = _next.fetch_add(1);
            _data.resize(next + 1);
            return { next };
        }

        void free(const Id id)
        {
            _data[id.idx] = T();
            _free.push_back(id);
        }

        typename decltype(_data)::iterator begin() { return std::begin(_data); }
        typename decltype(_data)::iterator end() { return std::end(_data); }
    };
}
