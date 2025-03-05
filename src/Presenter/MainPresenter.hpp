#pragma once

#include <memory>
#include "OutputHandler.hpp"

class InputHandler;

class MainPresenter final : public IPresenterComponent<OutputHandler> {
public:
	explicit MainPresenter();
	auto SamplePaths(const PathQuery& pathQuery) const -> void;
	auto GetDrift(const PathQuery& query) const -> void;
	auto GetPDFData(const PDFQuery& pdfQuery) const -> void;
	auto Clear() const -> void;
	auto GetInputHandler() const -> InputHandler*;
	auto GetOutputHandler() const -> OutputHandler*;
private:
	std::unique_ptr<InputHandler> m_inputHandler;
	std::unique_ptr<OutputHandler> m_outputHandler;
};