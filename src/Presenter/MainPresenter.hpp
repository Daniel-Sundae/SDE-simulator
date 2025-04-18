#pragma once

#include <memory>
#include "OutputHandler.hpp"
#include "PathEngine.hpp"

struct PathQuery;
struct PDFQuery;
class InputHandler;

class MainPresenter final : public IPresenterComponent<OutputHandler> {
public:
	explicit MainPresenter();
    // Remove const as these methods trigger state changes via OutputHandler
    auto OnQueriesReceived(const PathQuery &pQuery, const PathQuery &deterministicQuery, const PDFQuery &pdfQuery) -> void;
    auto Clear() -> void;
	auto GetInputHandler() const -> InputHandler*; // This can remain const
	auto GetOutputHandler() const -> OutputHandler*; // This can remain const
private:
    // Remove const from methods calling non-const OutputHandler methods
	auto SamplePaths(const PathQuery& pathQuery) -> void;
    auto GeneratePDFData(const PDFQuery &pdfQuery) -> void;
	auto SampleDriftCurve(const PathQuery& query) -> void;
private:
	std::unique_ptr<InputHandler> m_inputHandler;
	std::unique_ptr<OutputHandler> m_outputHandler;
	std::unique_ptr<PathEngine> m_engine;
};
