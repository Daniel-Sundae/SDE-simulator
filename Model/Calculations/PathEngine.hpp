#pragma once
#include "Types.hpp"
#include <QtCore/qobject.h>
class Engine() : public QObject
{
	Q_OBJECT
public:
	explicit Engine();
	
signal:
	auto Path(Path& path) -> void;

slots:
	auto RequestPath(SDEType type) -> void;

}
