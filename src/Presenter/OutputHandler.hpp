#pragma once
#include "IPresenterComponent.hpp"
//#include "BasicTypes.hpp"
#include "Types.hpp"

class OutputManager;
struct PathQuery;

class OutputHandler final : public IPresenterComponent<OutputManager> {
public:
	explicit OutputHandler();
	auto OnPathsReceived(const PathQuery& query, const Paths& paths) const -> void;
	auto OnDriftLineReceived(const Path& driftLine) const -> void;
	auto OnClear() const -> void;
};