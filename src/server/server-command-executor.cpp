#include "server-command-executor.h"
#include "battle.h"
#include "command.h"
#include "server.h"

Server_command_executor::Server_command_executor(Server *server)
    : _server{server}
{
}

auto Server_command_executor::server() -> Server *
{
  return _server;
}

auto Say_server_command_executor::execute(std::string from,
                                          Command const &command) -> Event
{
  auto const content{command.get_arg<std::string>(0)};
  Event broadcast{"broadcast"s};
  broadcast.set_param("from", std::move(from));
  broadcast.add_arg(content);
  server()->_session_service.push_event_all(broadcast);
  return Event{"ok"};
}
Say_server_command_executor::Say_server_command_executor(Server *server)
    : Server_command_executor{server}
{
}
constexpr auto Say_server_command_executor::name() -> std::string
{
  return "say";
}

// TODO(shelpam): here commented out just for debug. Please recover here.
//
// auto Query_event_server_command_executor::execute(
//     std::string /*from*/, Command const &command) -> Event
// {
//   auto const game_id{command.get_arg<std::int64_t>(0)};
//   auto &game{server()->_games.at(game_id)};
//   if (game.ended()) {
//     server()->_games.extract(game_id);
//     return "ended";
//   }
//   auto &events_queue{game.pending_events()};
//   if (events_queue.empty()) {
//     return "no";
//   }
//   auto event{std::move(events_queue.front())};
//   events_queue.pop();
//   return event;
// }
// Query_event_server_command_executor::Query_event_server_command_executor(
//     Server *server)
//     : Server_command_executor{server}
// {
// }
// constexpr auto Query_event_server_command_executor::name() -> std::string
// {
//   return "query-event";
// }

Fuck_server_command_executor::Fuck_server_command_executor(Server *server)
    : Server_command_executor{server}
{
}

auto Fuck_server_command_executor::execute(std::string from,
                                           Command const &command) -> Event
{
  Command new_command(command.name());
  new_command.set_param("fucker", from);
  for (auto const &[name, _] : server()->_players) {
    if (name != from) {
      server()->_session_service.push_event(name, new_command);
    }
  }
  return Event{"ok"};
}

constexpr auto Fuck_server_command_executor::name() -> std::string
{
  return "fuck";
}

Escape_server_command_executor::Escape_server_command_executor(Server *server)
    : Server_command_executor{server}
{
}

auto Escape_server_command_executor::execute(std::string /* from */,
                                             Command const &command) -> Event
{
  auto const game_id{command.get_param<std::size_t>("game-id")};
  server()->_games.at(game_id).stop(Stop_cause::escaping);
  return Event{"ok"};
}

constexpr auto Escape_server_command_executor::name() -> std::string
{
  return "escape";
}
