#pragma once
#include <memory>
#include <vector>
#include "IPresenterComponent.hpp"
class AppInitializer {
public:
	explicit AppInitializer();

private:
	auto InitializeComponents() -> void;
	
	std::vector<std::unique_ptr<IViewComponent>> m_viewComponents;
	std::vector<std::unique_ptr<IPresenterComponent>> m_presenterComponents;
	std::vector<std::unique_ptr<IModelComponent>> m_modelComponents;
};