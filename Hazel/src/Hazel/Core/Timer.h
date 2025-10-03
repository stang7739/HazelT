//
// Created by stang on 25-10-2.
//

#ifndef TIME_H
#define TIME_H
#include <chrono>
template <typename Fn>
    class Timer
{
public:
    Timer(const char* name, Fn&& func): m_Name(name), m_Func(func), m_Stoppped(false)
    {
        Reset();
    }

    ~Timer()
    {
        if (!m_Stoppped)
        {
            Stop();
        }
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().
            count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().
            count();

        m_Stoppped = true;

        float duration = (end - start) * 0.001f; // Convert to milliseconds
        m_Func({m_Name, duration});
    }
    void Timer::Reset()
    {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    float Timer::Elapsed()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_StartTime).count() * 0.001f * 0.001f * 0.001f;
    }

    float Timer::ElapsedMillis()
    {
        return Elapsed() * 1000.0f;
    }
private:
    const char* m_Name;
    Fn m_Func;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
    bool m_Stoppped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name,[&](ProfileResult profileResult ){m_ProfileResults.push_back(profileResult);})

#endif //TIME_H
