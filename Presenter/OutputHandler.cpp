#include "OutputHandler.hpp"
#include "OutputManager.hpp"

auto OutputHandler::OnPathReceived(const Path& path) const -> void
{
	Listener()->OnPathReceived(path);
}
