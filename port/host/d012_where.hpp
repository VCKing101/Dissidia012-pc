#pragma once

// Where the players online are in the game's Communication Mode, from the ad
// hoc server's list of players and the groups they are in
// (docs/port/communication-mode.md, sections 2 to 5):
//
//   LOBBY0 to LOBBY15   the Online Lobby's rooms, Cornelia to Zanarkand
//   MLOBBY              Group Battle: making, searching for and waiting in rooms
//   eight digits        a battle: an Online Lobby challenge's (fighter select
//                       and the fight) or a Group Battle's (fighting or
//                       watching), told apart by where its players came from
//   DISCOVER            Communications (Friend Cards and quests)
//   none                the game's menus, Friend Cards, Artifacts
//
// The public server keeps one (port/server/d012_server.cpp), and so does a copy
// while it hosts a session (d012_net.cpp); either answers the battle relay's
// WHERE with it (d012_battle_relay.hpp). It needs nothing of PortableKit's.
// MIT licence; see port/README.md.

#include "d012_battle_relay.hpp"

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace d012::where {

// One player online, as the ad hoc server lists them.
struct Player {
    std::uint64_t mac{};       // its six bytes, any order, as long as it is the same each time
    std::string version;       // the login's version tag
    std::optional<std::string> group;
};

// What a group's name says.
enum class Place { Menus, Lobby, GroupBattle, Battle, Communications };
[[nodiscard]] Place place_of(std::string_view group, unsigned &lobby);

class Tally {
public:
    // Everyone online now, a few times a second. Remembers which lobby group
    // each player was last in, to tell a battle's kind.
    void update(const std::vector<Player> &players);
    // The counts for the players with `version`.
    [[nodiscard]] net::relay::Where of(std::string_view version) const;

private:
    enum class From { Unknown, Lobby, GroupBattle };
    std::map<std::uint64_t, From> came_from_;
    std::map<std::string, net::relay::Where, std::less<>> by_version_;
};

} // namespace d012::where
