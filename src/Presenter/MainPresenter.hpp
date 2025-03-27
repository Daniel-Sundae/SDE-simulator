#pragma once

#include <memory>
#include "OutputHandler.hpp"

struct PathQuery;
struct PDFQuery;
class InputHandler;
class PathEngine;

class MainPresenter final : public IPresenterComponent<OutputHandler> {
public:
	explicit MainPresenter();
	auto SamplePaths(const PathQuery& pathQuery) const -> void;
	auto GetDrift(const PathQuery& query) const -> void;
	auto GeneratePDFData(const PDFQuery& pdfQuery) const -> void;
	auto Clear() const -> void;
	auto GetInputHandler() const -> InputHandler*;
	auto GetOutputHandler() const -> OutputHandler*;
private:
	std::unique_ptr<InputHandler> m_inputHandler;
	std::unique_ptr<OutputHandler> m_outputHandler;
	std::unique_ptr<PathEngine> m_engine;
};