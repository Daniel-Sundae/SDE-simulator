#include <vector>
#include "Process.hpp"


Process::Process(const ProcessName name):m_name(name){
    m_dimensions = 1;
    m_timesteps = 100;
}
//---------------------------------------------------------------------------//
Process::Process(
    const ProcessName name,
    const unsigned int dimensions,
    const unsigned int steps
):
m_name(name),
m_dimensions(dimensions),
m_timesteps(steps)
{}
//---------------------------------------------------------------------------//


