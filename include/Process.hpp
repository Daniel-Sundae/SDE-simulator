#pragma once

#include <string>

enum ProcessName{
    Wiener,
    Ito
};

class Process {
protected:
    Process(
        const ProcessName name
    );
    Process(
        const ProcessName name,
        const unsigned int dimensions,
        const unsigned int steps
    );
    virtual ~Process() = default;
    //struct Point;
public:
    struct Point;
    virtual std::vector<Point> Sample() const = 0;
private:
    unsigned int m_dimensions;
    unsigned int m_timesteps;
    const ProcessName m_name; 
};

struct Process::Point{
    double m_time;
    std::vector<double> m_data;
    Point(double time, std::vector<double> data):
        m_time(time),
        m_data(std::move(data))
    {}
};