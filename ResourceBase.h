#pragma once
#include <atomic>

namespace RenewEngine
{
    class ResourceBase {
    public:
        bool IsReady() const { return m_ready; }
        void SetReady(bool b) { m_ready = b; }
        void MarkReady() { m_ready = true; }

    protected:
        std::atomic<bool> m_ready = false;
    };
}