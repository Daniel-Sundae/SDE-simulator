#include "OutputHandler.hpp"
#include "OutputManager.hpp"

auto OutputHandler::OnPathReceived(const PathQuery& query, const Path& path) const -> void
{
	Listener()->OnPathReceived(query, path);
}
