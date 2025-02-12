#include "ChartManager.cpp"
#include <memory>

class OutputHandler final : public IPresenterComponent<ChartManager> {
public:
	explicit OutputHandler();
	auto OnPathReceived(Path& path) -> void;
};