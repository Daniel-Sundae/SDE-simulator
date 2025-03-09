#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"

class OutputManager;
struct PathQuery;

class OutputHandler final : public IPresenterComponent<OutputManager> {
public:
	explicit OutputHandler();
	auto OnPathsReceived(const PathQuery& query, const Paths& paths) const -> void;
	auto OnDriftDataReceived(const Path& driftData) const -> void;
	auto OnPDFDataReceived(const State ev, const PDFData& pdfData) const -> void;
	auto OnClear() const -> void;
};