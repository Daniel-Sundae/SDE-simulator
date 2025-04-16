#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"

class OutputDispatcher;
struct PathQuery;
class PDF;

class OutputHandler final : public IPresenterComponent<OutputDispatcher> {
public:
	explicit OutputHandler();
	auto OnPathsReceived(const PathQuery& query, Paths&& paths) -> void;
	auto OnDriftDataReceived(const Path& driftData) const -> void;
	auto OnPDFReceived(const PDF& pdf) -> void;
	auto OnClear() -> void;
private:
	auto IsInSupport(const State s) const -> bool;
	Range m_distributionSupport;
	Paths m_paths;
};