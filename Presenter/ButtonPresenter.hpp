#pragma once

class ButtonPresenter {

public:
	explicit ButtonPresenter();
	auto OnButtonPressed(ProcessType type) -> void;
private:
	auto BuildPathEngineQuery();
};
