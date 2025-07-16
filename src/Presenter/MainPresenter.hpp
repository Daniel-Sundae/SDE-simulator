#pragma once

#include <memory>
#include "OutputHandler.hpp"
#include "PathEngine.hpp"

struct PathQuery;
class InputHandler;
struct Transaction;

class MainPresenter final : public IPresenterComponent<OutputHandler> {
public:
	explicit MainPresenter();
    void clear() const;
    void cancel() const;
    InputHandler* getInputHandler() const;
    OutputHandler* getOutputHandler() const;
    void onTransactionReceived(const Transaction &&transaction);
private:
	void samplePaths(const PathQuery& pathQuery);
	void sampleDriftCurve(const PathQuery& query);

private:
    std::unique_ptr<InputHandler> m_inputHandler;
	std::unique_ptr<OutputHandler> m_outputHandler;
	std::unique_ptr<PathEngine> m_engine;
};
