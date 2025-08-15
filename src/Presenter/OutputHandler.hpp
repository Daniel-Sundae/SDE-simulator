#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"

class OutputDispatcher;
struct PathQuery;
struct PDF;

class OutputHandler{
public:
    OutputHandler() = default;
    void onPathsReceived(Paths&& paths);
    void onPDFReceived(PDF&& pdf);
    void onDriftDataReceived(Path &&driftCurve);
    void prepareGUI(const PathQuery &pQuery);
    void setOutputDispatcher(OutputDispatcher* outputDispatcher){ m_outputDispatcher = outputDispatcher; };
    void clear() const;
private:
    OutputDispatcher* m_outputDispatcher = nullptr;
};
