//
// Created by stang on 25-6-30.
//

#ifndef TIMESTEP_H
#define TIMESTEP_H

namespace Hazel
{
    class  Timestep
    {
    public:
        Timestep(float time = 0.0f): m_Time(time)
        {
        }

        operator float() const { return m_Time; }
        float GetSeconds() const { return m_Time; }
        float GetMilliseconds() const {return m_Time * 1000.0f; }

    private:
        float m_Time;
    };
}
#endif //TIMESTEP_H
