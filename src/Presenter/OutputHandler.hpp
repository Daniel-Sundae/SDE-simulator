#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"
#include "PathQuery.hpp"
#include <QtCore/QObject>

class OutputDispatcher;
struct PathQuery;
struct PDF;

class OutputHandler final : public QObject, public IPresenterComponent<OutputDispatcher> {
    Q_OBJECT

public:
    explicit OutputHandler(QObject* parent = nullptr);
    // Method called by worker thread
    void handleWorkerResult(Paths&& paths);
    void onDriftDataReceived(Path &&driftCurve);
    void prepareGUI(const PathQuery &pQuery);
    void onPDFReceived(PDF&& pdf);
    void clear() const;

public slots:
    void onPathsReceived(const Paths& paths);

signals:
    void internalPathReadySignal(Paths paths);
};
