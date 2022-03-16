#pragma once

#include <atomic>
#include <memory>
#include <vector>

namespace br::task
{
    class IJob;
    struct JobAccessor;

    template <typename T>
    using OwningJobHandle = std::shared_ptr<T>;

    using JobHandle = std::weak_ptr<IJob>;
    using RefCountedJobHandle = std::shared_ptr<IJob>;

    class IJob
    {
    public:
        virtual ~IJob();
        virtual void run() = 0;

    private:
        std::atomic_bool m_completed = false;
        std::vector<JobHandle> m_dependencies;

        friend struct JobAccessor;
    };

    void init(const int thread_count);
    void free();

    template <typename T, typename ... Args>
    OwningJobHandle<T> make_job(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args) ...);
    }

    void schedule(RefCountedJobHandle job);
    void schedule(RefCountedJobHandle job, const JobHandle depends);
    void schedule(RefCountedJobHandle job, const JobHandle* depends, const int depends_count);
    RefCountedJobHandle combine_dependencies(const JobHandle* depends, const int depends_count);
    void complete(JobHandle job);
    bool is_complete(JobHandle job);
}
