#pragma once
#include "OutputHandler.hpp"

class MainPresenter final : public IPresenterComponent<OutputHandler> {
public:
	auto SamplePath(PathQuery pathQuery) -> void;
}