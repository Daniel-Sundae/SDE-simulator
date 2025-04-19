#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"
#include "PathQuery.hpp"
#include <QtCore/QObject>

class OutputDispatcher;
struct PathQuery;
class PDF;

class OutputHandler final : public QObject, public IPresenterComponent<OutputDispatcher> {
    Q_OBJECT

public:
	explicit OutputHandler(QObject* parent = nullptr);
    // Method called by worker thread
    auto HandleWorkerResult(Paths&& paths) -> void;
    auto OnDriftDataReceived(Path &&driftCurve) -> void;
    auto PrepareGUI(const PathQuery &pQuery) -> void;
    auto OnPDFReceived(const PDF &pdf) -> void;
    auto Clear() const -> void;

    
    public slots:
	auto OnPathsReceived(const Paths& paths) -> void;
    
    signals:
    auto internalPathsReady(Paths paths) -> void;
    
private:
    auto HasSupport() const -> bool;
    auto DeleteSupport() -> void;
    auto IsInSupport(const State s) const -> bool;

private:
	Range m_distributionSupport;
	Paths m_paths;
	Path m_driftCurve;
};
