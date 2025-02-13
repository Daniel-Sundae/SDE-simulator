#include "OutputHandler.hpp"
#include "OutputManager.hpp"

OutputHandler::OutputHandler()
	: m_listener(nullptr)
{}

auto OutputHandler::OnPathReceived(Path& path) const -> void
{
	Listener()->OnPathReceived(path);
}
