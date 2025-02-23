#include "OutputHandler.hpp"
#include "OutputManager.hpp"

OutputHandler::OutputHandler()
	: IPresenterComponent()
{ }

auto OutputHandler::OnPathReceived(const PathQuery& query, const Path& path) const -> void
{
	Listener()->OnPathReceived(query, path);
}

auto OutputHandler::Clear() const -> void
{
	Listener()->Clear();
}
