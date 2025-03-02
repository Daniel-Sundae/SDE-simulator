#pragma once
#include "IPresenterComponent.hpp"
//#include "BasicTypes.hpp"
#include "Types.hpp"

class OutputManager;
struct PathQuery;

class OutputHandler final : public IPresenterComponent<OutputManager> {
public:
	explicit OutputHandler();
	auto OnPathReceived(const PathQuery& query, const Path& path) const -> void;
	auto OnDriftLineReceived(const Path& driftLine) const -> void;
	auto OnClear() const -> void;
};