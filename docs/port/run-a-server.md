# Running a server

`dissidia012-server` is the program behind Play Online ([playing online](play-online.md)): the lobby where copies
meet, the lobby's relay that carries everything in a lobby, and the battle relay that introduces two fighters so
their battle can go straight between them, and carries it when it cannot. It needs no game, no disc and no graphics.
The official one runs on a small rented Linux machine; anyone can run their own the same way. Releases carry no
ready-built server: it is built from this repository. How it works: [online matchmaking](online-matchmaking.md),
sections 8 to 15.

## What it needs

- A Linux machine with a public IPv4 address that is always on. One shared CPU and 1 GB of memory are plenty, and the
  traffic is small: a battle that goes through the server is a few kilobytes a second each way.
- These ports open to the internet, and nothing else needs to be:

  | Port | For |
  | --- | --- |
  | TCP 27312 | The lobby |
  | TCP 27313 | The lobby's relay |
  | UDP 27420 | Battles: introductions and the relay |

- Players on the same release of the port. The server keeps each release's players apart, so players of an older
  release meet only each other; a new release of the port needs no new server unless the server itself changed.

It also builds and runs on Windows (`cmake -S port/server -B build-server`, then `cmake --build build-server`), for
trying it out.

## Building it

On the server (Ubuntu 24.04 here; any recent Linux with GCC or clang, CMake and Git), as an ordinary user:

```
sudo apt install build-essential cmake git
git clone https://github.com/VCKing101/Dissidia012-pc.git
git clone https://github.com/TeamGDB/PortableKit.git
cd PortableKit
git checkout -b dissidia 33800c4
git am ../Dissidia012-pc/port/portablekit-patches/*.patch
cd ../Dissidia012-pc
cmake -S port/server -B build-server -DCMAKE_BUILD_TYPE=Release -DD012_PORTABLEKIT_DIR=../PortableKit
cmake --build build-server
```

The program is `build-server/dissidia012-server`. It needs only the C and C++ libraries. `--help` lists its options.

## Installing it as a service

As root, with the program built as above:

```
useradd --system --no-create-home --shell /usr/sbin/nologin dissidia
mkdir -p /opt/dissidia012 /etc/dissidia012
cp build-server/dissidia012-server /opt/dissidia012/
touch /etc/dissidia012/bans.txt
cp port/server/dissidia012-server.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable --now dissidia012-server
```

[`port/server/dissidia012-server.service`](../../port/server/dissidia012-server.service) runs it as the user
`dissidia`, which can change nothing on the system, and starts it again 5 seconds after it stops. Then open the
three ports, for example with ufw:

```
ufw allow 22/tcp
ufw allow 27312:27313/tcp
ufw allow 27420/udp
ufw enable
```

`systemctl status dissidia012-server` says whether it runs. On the Network page of a copy pointed at it (below),
Play online then counts the players on it.

## Options

| Option | What it does |
| --- | --- |
| `--port N` | The lobby's TCP port (27312); its relay is the next one up |
| `--battle-port N` | The battle relay's UDP port (27420) |
| `--ban-list FILE` | Addresses and copies to turn away (below) |
| `--max-players N` | Players online at most (512) |
| `--per-address N` | Players from one address at most (8) |
| `--rate KB` | What one address may send, in KB a second (512) |
| `--loopback` | Listen on 127.0.0.1 only, for copies on the same computer |
| `--regions-apart` | Keep players of the European disc (ULES01505) apart from those of the North American one (ULUS10566), as before 21 November 2026. By default they meet: both discs hold the same program |

Players reach a server on other ports by writing them in its address, `host:port` for the lobby. The battle port is
the copy's own setting (`online.battle_port` in its `settings.ini`), so keep 27420 for a public server.

## The ban list

One entry a line: an address as the log shows it (`203.0.113.7`), or a copy's MAC (`02:11:22:33:44:0a`, shown on
that player's Network page and in the log). `#` starts a comment. The server reads the file again within 5 seconds
of a change, and drops whoever it names.

## The log

`journalctl -u dissidia012-server` shows each login (nickname, MAC, release and address), every player turned away,
and, once a minute when it changed, how many players are online on each release and where they are: the lobbies by
number (1 Cornelia to 16 Zanarkand), Group Battle's rooms, 1-on-1 battles, party battles and Communications. A European disc's login shows the
North American code it plays under and its own: `(…, ULUS10566/<release>, disc ULES01505)`. The log holds players'
addresses: keep it briefly.

Each battle has `[battle-relay]` lines. When it starts: whether the fighters may try a direct path. At its end: how
long it lasted, how it ended (a fighter left, or nothing came for 20 s) and its path: `direct, the relay carried none
of its input`, or `through the relay` (with `from` and `until` when it moved partway). While the relay carries a
battle, every 30 s and in the end line, for each fighter (by nickname, or `fighter 1` and `fighter 2` when two
players share an address):

- **frames (n/s):** how far its game got. A battle runs at about 30 frames a second; fewer means that copy's game
  ran slow, or waited for the other.
- **round trip:** the server to that fighter and back, in ms (average, least to most), worked out from the echo in
  each copy's input. The two fighters' round trips added are the battle's.
- **quiet … at most:** the longest time with nothing from that fighter.
- **slack:** how many ms before it needed them the other fighter's inputs reached that copy (average, and the
  least). Negative: they came after it needed them, so that copy waited or, with rollback, guessed. The two fighters'
  slacks should be about the same.

A direct battle passes nothing through the server, so for those the copies' own logs (`data/logs/D012Native.log`,
its `[battle-net]` lines) are the place to look. The official server keeps a week, with this in
`/etc/systemd/journald.conf.d/dissidia-retention.conf` (then `systemctl restart systemd-journald`):

```
[Journal]
MaxRetentionSec=7day
```

## Updating it

Pull both repositories (PortableKit: check out `33800c4` again and apply the patches anew), build, copy the program
over the old one and run `systemctl restart dissidia012-server`. The restart drops the players online, as a lost
connection would.

## Pointing players at it

- **The official server**: every copy reads its address from [`online/server.json`](../../online/server.json) on this
  repository's main branch (`{"address": "host"}` or `"host:port"`) the first time it goes online, and keeps the last
  answer. Changing that file moves every player to another server, with no new release.
- **Any other server**: each player types its address in **Online server** on the Network page, under Ad hoc play.
  Play Online then goes there instead.
