#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"

class OutputDispatcher;
struct PathQuery;
struct PDF;

class OutputHandler{
public:
    OutputHandler() = default;
    void onPathsReceived(const Paths& paths);
    void onPDFReceived(const PDF& pdf);
    void onDriftDataReceived(const Path& driftCurve);
    void prepareGUI(const PathQuery &pQuery);
    void setOutputDispatcher(OutputDispatcher* outputDispatcher){ m_outputDispatcher = outputDispatcher; };
    void clear() const;
private:
    OutputDispatcher* m_outputDispatcher = nullptr;
};
