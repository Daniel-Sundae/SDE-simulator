#include "ChartManager.cpp"
#include <memory>

class OutputHandler {
public:
	explicit OutputHandler(std::shared_ptr<ChartManager> chartManager)
private:
	std::shared_ptr<ChartManager> m_chartManager;
};