// Where the players online are (d012_where.hpp).
//
// MIT licence; see port/README.md.

#include "d012_where.hpp"

#include <algorithm>
#include <limits>

namespace d012::where {
namespace {

bool digits(std::string_view text) {
    return !text.empty() && std::all_of(text.begin(), text.end(), [](char c) { return c >= '0' && c <= '9'; });
}

void add(std::uint16_t &count) {
    if (count < std::numeric_limits<std::uint16_t>::max()) ++count;
}

} // namespace

Place place_of(std::string_view group, unsigned &lobby) {
    lobby = 0u;
    if (group.size() > 5u && group.size() <= 7u && group.substr(0, 5u) == "LOBBY" && digits(group.substr(5u))) {
        const unsigned number = static_cast<unsigned>(std::stoul(std::string(group.substr(5u))));
        if (number < net::relay::kLobbies) {
            lobby = number;
            return Place::Lobby;
        }
        return Place::Menus;
    }
    if (group == "MLOBBY") return Place::GroupBattle;
    if (group.size() == 8u && digits(group)) return Place::Battle;
    if (group == "DISCOVER") return Place::Communications;
    return Place::Menus;
}

void Tally::update(const std::vector<Player> &players) {
    // Who came from where, kept for the players still online.
    std::map<std::uint64_t, From> came_from;
    // Each battle group's size, for battles whose players' way in was not seen
    // (a server started while they fought): two is a challenge's.
    std::map<std::string, unsigned, std::less<>> battle_sizes;
    for (const Player &player : players) {
        const auto known = came_from_.find(player.mac);
        From from = known != came_from_.end() ? known->second : From::Unknown;
        unsigned lobby = 0u;
        const Place place = player.group ? place_of(*player.group, lobby) : Place::Menus;
        if (place == Place::Lobby) from = From::Lobby;
        else if (place == Place::GroupBattle) from = From::GroupBattle;
        else if (place == Place::Battle) ++battle_sizes[player.version + '\n' + *player.group];
        came_from[player.mac] = from;
    }
    came_from_ = std::move(came_from);

    std::map<std::string, net::relay::Where, std::less<>> by_version;
    for (const Player &player : players) {
        net::relay::Where &where = by_version[player.version];
        add(where.online);
        unsigned lobby = 0u;
        switch (player.group ? place_of(*player.group, lobby) : Place::Menus) {
        case Place::Menus: add(where.menus); break;
        case Place::Lobby: add(where.lobbies[lobby]); break;
        case Place::GroupBattle: add(where.group_battle); break;
        case Place::Communications: add(where.communications); break;
        case Place::Battle: {
            const From from = came_from_[player.mac];
            const bool duel = from == From::Lobby ||
                              (from == From::Unknown && battle_sizes[player.version + '\n' + *player.group] == 2u);
            add(duel ? where.duels : where.party_battles);
            break;
        }
        }
    }
    by_version_ = std::move(by_version);
}

net::relay::Where Tally::of(std::string_view version) const {
    const auto found = by_version_.find(version);
    return found != by_version_.end() ? found->second : net::relay::Where{};
}

} // namespace d012::where
