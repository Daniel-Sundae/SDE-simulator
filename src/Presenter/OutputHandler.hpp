#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"

class OutputManager;

class OutputHandler final : public IPresenterComponent<OutputManager> {
public:
	explicit OutputHandler() = default;
	auto OnPathReceived(const PathQuery& query, const Path& path) const -> void;
};