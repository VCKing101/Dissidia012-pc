#pragma once

// Play online: the public server's address and how many players are on it
// (docs/port/online-matchmaking.md, sections 8 and 11), for PortableKit's
// Network page and ad hoc client (GameProfile::public_server and
// public_server_players).
//
// The address comes from a small file on the project's GitHub
// (kServerFile, online/server.json on main), fetched once a run the first time
// it is asked for, on a thread of its own, as the update check fetches. The
// last good answer is kept in the data folder (online-server.json) and used
// at once, so a copy that cannot reach GitHub still finds the server, and a
// server that moves needs only that file changed. Nothing is sent but the
// request.
//
// The count is the server's answer to STATUS on its battle port
// (d012_battle_relay.hpp): the players online with this copy's version tag.
// It is asked every few seconds while something shows it.
//
// D012_ONLINE_URL=<https address or file> reads the address from there instead,
// for tests. PortableKit's D012_ONLINE_SERVER (network.online_server) skips
// the file altogether.
// MIT licence; see port/README.md.

#include "d012_battle_relay.hpp"

#include <string>

namespace d012::online {

// The file on main that names the server.
inline constexpr const char *kServerFile =
    "https://raw.githubusercontent.com/VCKing101/Dissidia012-pc/main/online/server.json";

// The server's address ("host" or "host:port"), or empty with `note` saying
// why for the player. Starts the fetch the first time. Never waits.
[[nodiscard]] std::string public_server(std::string &note);

// Players on the server with this copy's version, or -1 while not known.
// Never waits.
[[nodiscard]] int players();

// Where the players with this copy's version are (d012_where.hpp), for the
// game's own Online Lobby screens (d012_lobby_status.cpp): the answer to
// WHERE (d012_battle_relay.hpp) from the server the game goes online with
// (`server`, PortableKit's adhoc_server_address(): the copy's own while it
// hosts, the session it joined, or Play online's), at its battle port, asked
// every few seconds while asked for. False while not known. Never waits; on
// the game's thread.
[[nodiscard]] bool where(const std::string &server, net::relay::Where &out);

} // namespace d012::online
