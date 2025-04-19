#pragma once

#include <memory>
#include "OutputHandler.hpp"
#include "PathEngine.hpp"

struct PathQuery;
struct PDFQuery;
class InputHandler;
struct Transaction;

class MainPresenter final : public IPresenterComponent<OutputHandler> {
public:
	explicit MainPresenter();
    auto Clear() const -> void;
    auto Cancel() const -> void;
    auto GetInputHandler() const -> InputHandler*;
    auto GetOutputHandler() const -> OutputHandler*;
    auto OnTransactionReceived(const Transaction &&transaction) -> void;
private:
	auto SamplePaths(const PathQuery& pathQuery) -> void;
    auto GeneratePDFData(const PDFQuery &pdfQuery) -> void;
	auto SampleDriftCurve(const PathQuery& query) -> void;

private:
    std::unique_ptr<InputHandler> m_inputHandler;
	std::unique_ptr<OutputHandler> m_outputHandler;
	std::unique_ptr<PathEngine> m_engine;
};
