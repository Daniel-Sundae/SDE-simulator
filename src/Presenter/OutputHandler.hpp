#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"

class OutputManager;
struct PathQuery;

class OutputHandler final : public IPresenterComponent<OutputManager> {
public:
	explicit OutputHandler();
	auto OnPathsReceived(const PathQuery& query, const Paths& paths) -> void;
	auto OnDriftDataReceived(const Path& driftData) const -> void;
	auto OnPDFReceived(const PDF& pdf) -> void;
	auto OnClear() -> void;
private:
	auto IsInSupport(const State s) const -> bool;
	Range m_distributionSupport;
};