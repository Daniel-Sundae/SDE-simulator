#include "ChartManager.cpp"
#include <memory>

class OutputHandler final : public IPresenterComponent<ChartManager> {
public:
	explicit OutputHandler(std::shared_ptr<ChartManager> chartManager);
	auto OnPathReceived(Path& path) -> void;
};