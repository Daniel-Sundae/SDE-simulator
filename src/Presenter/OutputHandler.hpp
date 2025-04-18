#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"
#include "PathQuery.hpp"
#include <QtCore/qobject.h>

class OutputDispatcher;
struct PathQuery;
class PDF;

// Inherit from QObject and IPresenterComponent
class OutputHandler final : public QObject, public IPresenterComponent<OutputDispatcher> {
    Q_OBJECT // Added Q_OBJECT macro

public:
	explicit OutputHandler(QObject* parent = nullptr); // Added optional parent

    // Method called by worker thread
    auto HandleWorkerResult(const PathQuery& query, Paths paths) -> void;

    // Other existing public methods
    auto OnDriftDataReceived(Path &&driftCurve) -> void;
    auto OnDriftDataReceived(const Path &driftData) const -> void;
    auto OnPDFReceived(const PDF& pdf) -> void;
	auto OnClear() -> void;

public slots:
	auto OnPathsReceived(const PathQuery& query, Paths paths) -> void;

signals:
    auto internalPathsReady(PathQuery query, Paths paths) -> void;

private:
	auto IsInSupport(const State s) const -> bool;
private:
	Range m_distributionSupport;
	Paths m_paths;
	Path m_driftCurve;
};
