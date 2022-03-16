#include <borealis/Task.hpp>
#include <borealis/util/Trace.hpp>
#include <concurrentqueue.h>
#include <thread>

namespace br::task
{
    moodycamel::ConcurrentQueue<RefCountedJobHandle> _queue;
    std::vector<std::thread> _workers;
    std::atomic_bool _quit = false;
    std::atomic_int _job_count = 0;

    struct JobAccessor
    {
        static void worker_main()
        {
            while (!_quit)
            {
                RunResult result;
                while ((result = run_one_job()) == RunResult::Completed) { }

                if (result == RunResult::Requeued)
                {
                    std::this_thread::yield();
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
            }
        }

        enum class RunResult
        {
            Completed,
            Requeued,
            Empty
        };

        static RunResult run_one_job()
        {
            if (RefCountedJobHandle handle; _queue.try_dequeue(handle))
            {
                BR_ASSERT(!is_complete(handle.get()));

                if (!are_dependencies_complete(handle.get()))
                {
                    _queue.enqueue(std::move(handle));
                    return RunResult::Requeued;
                }

                handle->run();
                handle->m_completed.store(true);
                --_job_count;

                return RunResult::Completed;
            }

            return RunResult::Empty;
        }

        static bool is_complete(IJob* job)
        {
            return job->m_completed.load();
        }

        static bool are_dependencies_complete(IJob* job)
        {
            bool complete = true;

            for (const JobHandle& depends : job->m_dependencies)
            {
                RefCountedJobHandle concrete = depends.lock();
                if (concrete && !concrete->m_completed)
                {
                    complete = false;
                    break;
                }
            }

            if (complete)
            {
                job->m_dependencies.clear(); // release ref on deps to allow them to be freed earlier
            }

            return complete;
        }

        static void add_dependency(IJob* job, JobHandle depend)
        {
            job->m_dependencies.emplace_back(std::move(depend));
        }

        static void add_dependencies(IJob* job, const JobHandle* depends, const int depends_count)
        {
            const int original_size = (int)job->m_dependencies.size();
            job->m_dependencies.resize(original_size + depends_count);
            std::copy(depends, depends + depends_count, job->m_dependencies.data() + original_size);
        }
    };

    IJob::~IJob()
    {
        BR_ASSERT(JobAccessor::is_complete(this));
        BR_ASSERT(JobAccessor::are_dependencies_complete(this));
    }

    void init(const int thread_count)
    {
        for (int i = 0; i < thread_count; ++i)
        {
            _workers.emplace_back(&JobAccessor::worker_main);
        }
    }

    void free()
    {
        while (JobAccessor::run_one_job() != JobAccessor::RunResult::Empty)
        {
            // TODO: Jobs that add other jobs infinitely?
        };

        _quit = true;

        for (std::thread& thread : _workers)
        {
            thread.join();
        }
    }

    void schedule(RefCountedJobHandle job)
    {
        BR_ASSERT(!JobAccessor::is_complete(job.get()));
        _queue.enqueue(std::move(job));
        ++_job_count;
    }

    void schedule(RefCountedJobHandle job, JobHandle depends)
    {
        JobAccessor::add_dependency(job.get(), std::move(depends));
        schedule(std::move(job));
    }

    void schedule(RefCountedJobHandle job, const JobHandle* depends, const int depends_count)
    {
        JobAccessor::add_dependencies(job.get(), depends, depends_count);
        schedule(std::move(job));
    }

    RefCountedJobHandle combine_dependencies(const JobHandle* depends, const int depends_count)
    {
        struct DummyJob : public IJob
        {
            void run() override { }
        };

        auto job = make_job<DummyJob>();
        schedule(job, depends, depends_count);
        return job;
    }

    void complete(JobHandle job)
    {
        while (!is_complete(job))
        {
            // TODO: an improve this to specifically try to do this one task
            // needs another field inside the task to tell us whether it has started or not
            JobAccessor::run_one_job();
        }
    }

    bool is_complete(JobHandle job)
    {
        RefCountedJobHandle concrete = job.lock();
        return !concrete || JobAccessor::is_complete(concrete.get());
    }

    int outstandin_job_count()
    {
        return _job_count.load();
    }
}
