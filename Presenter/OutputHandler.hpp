#pragma once
#include "IPresenterComponent.hpp"
#include "Types.hpp"

class OutputManager;

class OutputHandler final : public IPresenterComponent<OutputManager> {
public:
	explicit OutputHandler();
	auto OnPathReceived(Path& path) const -> void;
};