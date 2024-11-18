#pragma once

#include "command.h"
#include "event.h"
#include "game-map.h"
#include "item/item.h"
#include "player.h"
#include "session.h"
#include "window.h"

class Application {
  enum class State : std::uint8_t {
    unlogged_in,
    logging,
    greeting,
    starting_battle,
    battling,
    ended,
    should_stop,
  };

public:
  Application();
  void run();

private:
  [[nodiscard]] auto should_stop() const -> bool;
  void schedule_continuous_query_event();
  void tick();
  void poll_events();
  void update();
  void render();
  void show_user_info();
  void handle_greeting();
  void handle_starting_battle();
  void handle_battling();
  void handle_ended();
  void starting_new_game();

  void process_event(Event const &event);
  void add_to_show(std::string message);

  void async_request(Command const &command,
                     std::function<void(Event)> on_replied);

  asio::io_context _io_context;
  Session_ptr _session;
  State _state{State::unlogged_in};
  std::string _name;

  Window _window;

  player::Player_ptr _you;

  std::size_t _battle_id{};
  std::size_t _battled_rounds{};

  std::shared_ptr<Game_map> _game_map;

  std::map<double, std::string> _messages;
  std::map<std::string, player::Player> _players;
  std::map<std::string, item::Item_info> _store_items;

  static constexpr std::size_t buf_size{32};
  std::array<char, buf_size> _name_buf{};
  std::array<char, buf_size> _host_buf{"localhost"};
  std::array<char, buf_size> _message_input_buf{};
};

[[nodiscard]] auto connect(asio::io_context &io_context, std::string_view host,
                           std::string_view port) -> tcp::socket;
