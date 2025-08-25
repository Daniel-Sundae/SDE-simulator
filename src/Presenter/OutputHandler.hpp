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
    void onDistributionReceived(const Distribution& distribution, const Range support);
    void onStartTransaction(const PathQuery& query);
    void onDriftDataReceived(const Path& driftCurve);
    void jobProgress(size_t pathsCompleted);
    void setError(ErrorType error);
    void setOutputDispatcher(OutputDispatcher* outputDispatcher){ m_outputDispatcher = outputDispatcher; };
    void clearGUI() const;
    void cancelGUI() const;
    
private:
    void signalReadyIfNeeded() const;
    void prepareGUI(const PathQuery &query);
    OutputDispatcher* m_outputDispatcher = nullptr;
    bool m_pathsReceived = false;
    bool m_distributionReceived = false;
    PDF m_currentPDF;
};
