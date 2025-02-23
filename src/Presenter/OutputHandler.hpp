#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"

class OutputManager;

class OutputHandler final : public IPresenterComponent<OutputManager> {
public:
	explicit OutputHandler();
	auto OnPathReceived(const PathQuery& query, const Path& path) const -> void;
	auto Clear() const -> void;
};