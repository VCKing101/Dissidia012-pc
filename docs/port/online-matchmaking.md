# Online matchmaking: a public server (10 November 2026)

The owner asked whether online play could have matchmaking, with a VPS as the lobby server and each battle a direct
connection between its two players, and whether Oracle Cloud's free tier could host it. Sections 0 to 7 are the
research, on main 9f557af; the design agreed on 15 November and the build order are sections 8 and 9. V = seen in code or docs at the place given; I = inferred. PK =
`tools/local/port/PortableKit`. Background: [Communication Mode](communication-mode.md) (sections 14 and 18.3 for
how players connect today), [online play](online-play.md) (section 4, the ways to connect players) and
[playing online](play-online.md) (the players' guide).

## 0. The short answer

- **The owner's idea is the standard way to do it**: a lobby server everyone reaches, and battles direct between
  the two players. Dolphin's traversal server and PPSSPP's public ad hoc servers work the same way.
- **One correction: a lobby server does not by itself make a direct battle possible.** The two players' routers
  still drop each other's packets. The server also has to introduce them so their routers let the battle through
  (hole punching), and carry the battle itself when that fails: on mobile data, on a provider's shared address
  (carrier-grade NAT) and behind some strict routers. Without that fallback, the server only moves the problem.
- **Most of it is built already.** Today one player's copy hosts the session and runs the lobby, the lobby's relay
  and a battle relay. A public server is those three services on a machine that is always on, so nobody hosts,
  nobody opens router ports and nobody sends addresses by message.
- **Recommended order**: first, every battle through the server's relay, as PPSSPP 1.20 does (almost no new
  netcode). Measure the latency, then add direct battles with the relay as fallback if the extra hop costs players
  a frame of input delay.
- **Oracle Cloud's free tier works for this**, with two catches. A lobby server will look idle to Oracle, so a free
  account's machine would be stopped after a week: upgrade the account to Pay As You Go, which keeps it running
  and still costs nothing inside the free limits. And Oracle changes the free tier without notice (it halved its Arm
  allowance in June 2026), so the port should not depend on one server address: it should fetch the address,
  so moving to another host needs no release.

## 1. How players connect today

One player selects Host a session; the copy then runs (V):

| Service | Port | Carries | Code |
| --- | --- | --- | --- |
| The lobby (adhocctl) | TCP 27312 | Log-in, groups (`LOBBY0`..`LOBBY15`, `MLOBBY`, `DISCOVER`, the 8-digit battle groups), who is where | PK `host/adhoc/server.*` |
| The lobby's relay | TCP 27313 | Every ad hoc packet: Friend Card hellos, challenges, Group Battle rooms, the battle session's setup streams, Communications | PK `host/adhoc/server.*` |
| The battle relay | UDP 27420 | The port's battle link between two fighters, and the watchers' stream | `port/host/d012_net.cpp:1485` (`Relay`), open only while the copy hosts (`:1524`) |

Everyone else joins the host's address. Fighters send their battle link to the session's address on UDP 27420
(`d012_net.cpp:1790`, `from_session`), so when the host fights, its own link goes over loopback and the battle is
direct in effect.

What follows from that (V, [playing online](play-online.md)):
- The host's router must pass all three ports: NAT-PMP or UPnP, or forwarding by hand.
- A host on mobile data or a provider's shared address cannot host at all.
- Players find each other on Discord and send addresses by direct message.
- The session ends when the host's game closes.

PK can already run the lobby and its relay without the game: `D012Native --adhoc-server [port]`
(PK `host/main.cpp:271`, `run_adhoc_server`). It has no battle relay, which lives in the port, so battles through it
still need `online.battle_link` ([Communication Mode](communication-mode.md) 18.3).

## 2. The public server

```
                 ┌──────────────── public server (always on) ─────────────┐
                 │  lobby: TCP 27312        lobby's relay: TCP 27313      │
                 │  battle relay and introductions: UDP 27420             │
                 └───────▲──────────────────────────────────▲─────────────┘
   lobby, Friend Cards,  │                                  │  lobby, Friend Cards,
   challenges, setup     │                                  │  challenges, setup
                     Player A ◄────── battle inputs ──────► Player B
                               direct UDP after hole punching,
                               otherwise through the server's relay
```

**The lobby moves to the server.** Every copy connects to it when it goes online, with no Host or Join step. The
lobby connection stays open through the battle, as it does today: the battle session's setup streams run over it
(V, [Communication Mode](communication-mode.md) 4.1). Only the per-frame inputs go on the battle link.

**The game's own lobbies are the matchmaking.** A public server changes what each Communication Mode row means:

| Row | On a public server |
| --- | --- |
| Online Lobby | Everyone online in Cornelia sees everyone else there and can challenge them. Each lobby's list has **16 slots** (V, main 0x0890A358), so 16 lobbies show up to 256 players. What a seventeenth player in one lobby sees is not known (I) |
| Group Battle | Every room is in the one group `MLOBBY`, so Search for a Room lists every open room on the server, up to 10 players each (V, [Communication Mode](communication-mode.md) 3) |
| Communications | Everyone in Communications at the same moment trades Friend Cards with everyone else, up to 16 (V, section 5): strangers get each other's cards. Fun, much like the PSP's sleep-mode trading, but players should know it |

**Private sessions stay.** Host a session and Join remain for friends, LAN play, VPNs and when the server is down.

## 3. Battles: through the relay first, direct later

### 3.1 Through the server's relay

The battle relay already works (`Relay`, V; tested on one PC, [Communication Mode](communication-mode.md) 18.3, and
with the Steam Deck hosting, 19.6). On a server it runs all the time instead of only while a copy
hosts. Both fighters only ever connect outward, so no player needs open ports, and every player can play.

The cost is the extra hop: A to server to B instead of A to B. How much that matters (I):
- Auto input delay is 2 frames up to a round trip of about 100 ms ([playing online](play-online.md)). Two
  players in the server's region gain a few tens of milliseconds, often still under 100 ms, so often no extra frame.
- Players far from the server, or far from each other, gain more. A server per region fixes that later.
- Rollback (on its branch, off by default) would hide the hop almost entirely.

So: build the relay version, measure real players' round trips through it against their direct round trips, and
only then decide whether direct battles are worth the work.

### 3.2 Direct, with the relay as the fallback

How it would work (I):
1. Both fighters already send their first packets to UDP 27420: the leader's HERE and the other's HELLO
   (`d012_net.cpp:133`, `:1170`). From them the server learns each fighter's public address and port as their
   router translated it.
2. The server sends each fighter the other's address. Both send to each other at once; most home routers then pass
   the replies, because each sees them as answers to its own packets (hole punching). It works through most home
   routers, not all ([RFC 5128](https://www.rfc-editor.org/rfc/rfc5128.html)).
3. **The path is chosen before the battle's first frame**, during the round-trip probe the link already runs to set
   the input delay: direct if the direct path answered, the relay otherwise. A path that changes in the middle of a
   battle would change the round trip the delay was set for.
4. Watchers stay on the relay: they already watch a little behind the fighters.

The ready-made alternative is ICE (libjuice, MPL-2.0) with a TURN server (coturn) as the relay, as psxrecomp does
([online play](online-play.md) 3.2 and 4). It handles more router types, but adds two libraries and a second
server, and the battle link is already its own UDP protocol. Hand-made punching on the existing packets comes first
(I).

### 3.3 What the players' IP addresses show

- The server sees every player's address. It should log as little of them as it can and keep logs briefly.
- A direct battle shows each fighter's address to the other, as a session does today. A setting "Always through
  the server" would hide it, at the relay's cost.

## 4. What it would take

1. **A server program.** One small Linux program with the lobby, the lobby's relay and the battle relay, no game,
   no graphics, run as a service (systemd). It is mostly existing code: PK `run_adhoc_server` and the port's
   `Relay` moved somewhere both can be built without the game. Builds for x86-64 and Arm (aarch64).
2. **Where the port finds the server.** The port fetches a small file (the server's address, maybe several by
   region) from the project's GitHub, through the same WinHTTP or libcurl path as the update check
   (`port/host/d012_updates.cpp`). Moving the server then means editing that file, with no release.
3. **The Network page.** A "Play online" row that uses the public server, above Host a session and Join.
4. **Versions kept apart.** Today the version check comes at Enter Battle (V, [playing online](play-online.md)). On
   a public server, players would see Friend Cards from other versions and only find out at Enter Battle. The server
   should put each version in lobbies of its own, or turn old versions away with a message to update.
5. **Hardening.** The ad hoc server was written for friends who trust each other. A public one needs:
   - a limit on connections per address, and rate limits;
   - malformed packets dropped;
   - **a duplicate virtual MAC refused rather than swapped in.** Today a second copy with the same MAC makes the
     server drop each in turn (V, seen in tests). A MAC is visible to everyone in a lobby, so on a public server
     anyone could knock a player off by copying theirs. The first holder keeps it; the second is told to make a new
     one (a copied settings.ini does this by accident too).
6. **Optional.** Players online, shown on the Network page and in the Discord status; a server per region.
7. **Later, if measured.** Direct battles (3.2).
8. **Docs.** The players' guide, and a short note on what the server sees.

## 5. Hosting on Oracle Cloud's free tier

### 5.1 What is free

From Oracle's own page, read on 10 November 2026 ([Always Free Resources][oci-free]):

| Resource | Always Free allowance |
| --- | --- |
| AMD machines (VM.Standard.E2.1.Micro) | Up to two, each 1/8 OCPU (can use more when spare), 1 GB memory, one public IP, **up to 50 Mbps to the internet** |
| Arm machines (VM.Standard.A1.Flex) | 1,500 OCPU hours and 9,000 GB hours a month, which is **2 OCPUs and 12 GB** in all, split over one or two machines |
| Outbound data | **10 TB a month** |
| Storage | 200 GB of block volumes in all (each machine's boot volume about 50 GB) |
| Where | The account's **home region only**, chosen at sign-up |
| How long | "for the life of the account", for free and paid accounts alike |

### 5.2 Does the server fit

Yes, with room to spare (I unless marked):
- **Bandwidth is not a problem.** The busiest traffic is not battles but lobby hellos: each copy sends a
  0x1258-byte (4.6 KB) hello every 5 s (V, [Communication Mode](communication-mode.md) 10), one copy for each
  other player in its lobby (V, PK `host/adhoc/client.cpp:1488`). A full lobby of 16 is then about 16 × 15 × 4.6 KB every 5 s, about 1.8 Mbps out of
  the server. All 16 lobbies full is about 30 Mbps, under the AMD machine's 50. A battle's link is about 15-30 MB an
  hour a player ([online play](online-play.md) 4, an estimate from before the current link; measure it), so the
  10 TB a month would carry hundreds of relayed battles running day and night.
- **CPU and memory are not a problem.** Passing small packets on needs almost nothing; 1 GB of memory is far more
  than the server needs.
- **The AMD machine is enough and simplest.** It is x86-64, so the server builds like the port's Linux build
  already does. The Arm machine needs an aarch64 build: easy, but one more thing to build.
- **Ports are opened in two places**: the network's security list in Oracle's console (TCP 27312-27313, UDP 27420),
  and the machine's own firewall. Oracle's Ubuntu images come with iptables rules that reject everything except
  SSH; new rules must go above the final REJECT line ([Oracle's developer blog][oci-iptables]).
- **The home region decides latency for everyone**, and it cannot be changed later. Pick the region nearest most
  players (5.6).

### 5.3 The catches

- **The server will look idle, and a free account's idle machines are stopped.** Oracle counts a machine as idle
  when, over 7 days, its 95th-percentile CPU use is under 20% and its network use under 20% (memory too, on Arm)
  ([Always Free Resources][oci-free]). A lobby server stays far under both. Oracle's email to free accounts in
  2023 said idle machines "will be stopped", can be restarted while the shape is available, and that converting
  the account to **Pay As You Go** keeps them from being stopped, with no charge while usage stays within the free
  limits ([the email, as quoted by 51sec][oci-idle]). Scripts that fake load to look busy exist; they burn
  resources against the rule's purpose and could be judged abuse. Upgrading is the honest route.
- **Pay As You Go means a card that can be charged.** Anything beyond the free limits is billed. Set a budget alert
  in the console and use only Always Free shapes. Forum users report a temporary verification hold on the card when
  upgrading (anecdotal).
- **Oracle changes the free tier without notice.** On 15 June 2026 it halved the Arm allowance (from 4 OCPUs and
  24 GB) with no announcement, only a change to the docs, and free accounts' machines over the new limit were shut
  down until resized ([InfoQ][infoq]). The AMD machines were not part of that change. Free can always become less
  free, so the server must be quick to move (4.2).
- **"Out of host capacity".** Free shapes can be sold out in a region for a while; the fix is to retry, try another
  availability domain or upgrade the account ([Always Free Resources][oci-free]).
- **The account is the owner's.** Sign-up takes a real name and a card. Nothing of it shows to players; the server
  shows only its address.

### 5.4 Other places

| Where | Cost | Fit |
| --- | --- | --- |
| Oracle Cloud, AMD micro, Pay As You Go | Free within limits | **Best start** |
| Google Cloud e2-micro | Free: one machine in three US regions only, **1 GB of outbound data a month** ([Google][gcp-free]) | The lobby alone for a few players; far too little data to relay battles (I) |
| Hetzner, US (Ashburn, Hillsboro) | **€17.99 a month** for the smallest US machine (CPX11), 1 TB of traffic, read 15 November 2026 ([Hetzner][hetzner-rp]). The €6 machines are Europe only, too far for these players | No longer the cheap fallback for North America |
| Akamai (Linode) Nanode | US$5 a month: 1 GB, 1 TB of traffic, then US$0.005 a GB ([Akamai][akamai-cpu]) | Plain paid machine; Chicago among its regions (I) |
| DigitalOcean Basic Droplet | US$4 (512 MB, 500 GB) or US$6 (1 GB, 1 TB) a month ([DigitalOcean][do-droplets]) | Plain paid machine; its North American regions are on the coasts and Toronto (I) |
| Vultr Cloud Compute | About US$5 a month for 1 GB and 1 TB (third-party listings; Vultr's page refused the fetch) | Regions include Chicago, Dallas, Miami and Mexico City (third-party listing, [datacenters.com][vultr-dc]) |
| OVHcloud VPS-1 | From $6.20 a month on the Canadian page: 2 vCores, 4 GB, unlimited traffic ([OVHcloud][ovh-vps]) | Most machine for the money, DDoS protection included; North American sites on the east and west edges (I) |
| The owner's own PC or a Raspberry Pi at home | Electricity | The router already forwards the ports to the owner's PC, but a public server would publish the owner's home address. Not recommended |

1 TB a month is ample for a small community (I): relayed battles at the 15-30 MB per player-hour estimate (5.2) take
tens of thousands of player-hours to reach it, and only a lobby of 16 kept full around the clock (about 0.8 GB an
hour) would reach it within a month. The Hetzner row replaces a 30 October figure of about €6, which was a European
price.

### 5.5 Verdict

Use Oracle's free tier to start: the AMD micro machine, the account upgraded to Pay As You Go, a budget alert, and
the server's address in the file the port fetches. If Oracle stops the machine or changes the terms, the same
program moves to a US$5 machine (Akamai or Vultr, in Chicago), and the port follows the file.

**15 November:** the owner parked Oracle as too fiddly to keep free. If the server is built, a US$5-6 machine in
Chicago (Akamai or Vultr), or OVHcloud's VPS-1 for more machine, is the plain choice (I).

### 5.6 The region (11 November 2026)

**Where the players are.** The owner asked on the Discord server: the players are in Canada, the United States and
Central America. So the server goes in North America.

**Signing up.** The account is an Oracle Cloud Infrastructure (OCI) account, made from the Oracle Cloud Free Tier
page (oracle.com/cloud/free). It starts as a 30-day trial with US$300 of credit plus the Always Free resources; it
needs a phone number and a card, and the card is not charged unless the account is upgraded
([Oracle's free tier guide][oci-start]). **The home region is chosen during sign-up and cannot be changed**, and
Always Free machines run only there ([Oracle's free tier guide][oci-start]). Free, trial and Pay As You Go accounts
use one region ([Oracle, regions][oci-regions]).

**The candidates** ([Oracle, regions][oci-regions]):

| Region | Identifier | Availability domains | For these players (I) |
| --- | --- | --- | --- |
| US Midwest (Chicago) | `us-chicago-1` | 3 | The middle of the continent: close to Toronto and Montreal, and about as far from either coast; Central America a little further than from Ashburn |
| US East (Ashburn) | `us-ashburn-1` | 3 | Closest to Miami, through which much of Central America's traffic reaches the US, and to eastern Canada; the west coast further. Oracle's busiest region, so the likeliest to be out of free machines |
| US West (Phoenix), US West (San Jose) | `us-phoenix-1`, `us-sanjose-1` | 3, 1 | Good for the west; eastern Canada and Central America further |
| Mexico Central (Queretaro), Mexico Northeast (Monterrey) | `mx-queretaro-1`, `mx-monterrey-1` | 1 each | Good for Mexico and Central America; Canada furthest |
| Canada Southeast (Toronto), (Montreal) | `ca-toronto-1`, `ca-montreal-1` | 1 each | Good for eastern Canada; Central America furthest |

**Recommendation (I): Chicago, with Ashburn second.** With every battle through the relay, a battle's path is
player to server to player, so the server should sit in the middle of the players rather than near any one group.
For pairs across the continent the two come out within a few milliseconds of each other; Chicago is the more central
and less crowded of the two.

**Measured from the owner's PC** (TCP connect to each region's Object Storage address, three tries, V): San Jose
about 165 ms, Phoenix 210, Chicago 250, Ashburn 265, Toronto 300. The owner is far from all of them, so on a North
American server the owner is the distant player; the server is placed for the players.

**Checking before the permanent choice.** A few players can time their own connection to each region. On Windows,
in PowerShell (it connects to each region's public Object Storage address and closes again; nothing is sent):

```powershell
foreach ($r in 'ca-toronto-1','us-ashburn-1','us-chicago-1','us-phoenix-1','us-sanjose-1','mx-queretaro-1') { $ip = [Net.Dns]::GetHostAddresses("objectstorage.$r.oraclecloud.com")[0]; $ms = (Measure-Command { $c = New-Object Net.Sockets.TcpClient; $c.Connect($ip, 443); $c.Close() }).TotalMilliseconds; '{0,-15} {1,4:N0} ms' -f $r, $ms }
```

The region with the lowest worst result across the players is the one to pick. If sign-up does not offer it, or it
has no free machines, take the next.

**Done (11 November):** the account exists, with its home region US Midwest (Chicago).

### 5.7 Setting up the machine

In order, from the Console ([Oracle, creating an instance][oci-launch]):

1. **A budget first.** Budgets, Create Budget: the root compartment, Monthly, an amount of 1, and an alert rule on
   Actual Spend over 1% of it, to the owner's email. Any charge at all then sends an email (minimum amount 1,
   [Oracle, budgets][oci-budget]).
2. **The machine.** Compute, Instances, Create instance. Image: Canonical Ubuntu 24.04 (Minimal suits 1 GB of
   memory). Shape: Virtual machine, Specialty and previous generation, `VM.Standard.E2.1.Micro`, marked Always
   Free-eligible. The micro shape exists in only one availability domain of a region with several
   ([Oracle, Always Free][oci-free]), so if it is not offered, try the next domain under Placement. Networking: a new
   virtual cloud network and public subnet, with a public IPv4 address. SSH keys: Paste public keys, with a key made
   for the server. Boot volume: the default.
3. **The cloud firewall.** In the new network's Default Security List, ingress rules from `0.0.0.0/0`: TCP
   27312-27313 (the lobby and its relay) and UDP 27420 (the battle relay). SSH (TCP 22) is open by default.
4. **The machine's own firewall.** Oracle's Ubuntu image blocks everything but SSH with an iptables REJECT rule, so the
   same ports go in above it and are saved ([Oracle, iptables][oci-iptables]).

The public address stays with the machine while it is stopped and is lost only when the machine is terminated
([Oracle, public IPs][oci-pubip]). It still goes in the fetched file (section 4), not in the program. Nothing marked
other than Always Free-eligible should be created: that runs on trial credit.

## 6. Matchmaking queues

"Matchmaking" can also mean a queue that pairs players by itself, by skill or region. The game's lobbies are "find
someone and challenge them" instead. A queue would be the port's own menus and server logic, outside the game's
screens, and with today's number of players it would mostly sit empty (I). The game's lobbies on a public server
come first; a queue only if the number of players grows.

## 7. Decisions for the owner

- Run an always-on public server, and be the one who answers for it (players' addresses, misbehaving players)?
- One public lobby for everyone, private room codes, or both?
- ~~Where most players are~~: Canada, the United States and Central America (11 November); the home region follows
  (5.6).
- Whether Communications on the public server trades cards with strangers, or only within private sessions.

All answered on 15 November: section 8.

## 8. The design (agreed 15 November 2026)

The owner answered each question in turn; (O) marks the owner's choice where it overrides a recommendation above.
Building started from main aad5f443.

**The server**
- A separate headless program in the repo, `dissidia012-server`, for Linux and Windows: PK's lobby and its relay
  (TCP 27312-27313), the battle relay (UDP 27420, moved out of the game into code both build) and the introductions
  for direct battles. No graphics, sound or disc.
- Players see only players on their own version: the version the battle start checks, the port's source hash and
  its compiler (`d012_comm_battle.cpp:742`). Copies send it at log-in; the server keeps each version's groups apart.
- Everyone on a version shares the game's 16 lobbies; Group Battle and Communications work there too, so strangers
  trade Friend Cards. No room codes: private play is Host and Join.
- A small status query answers how many players are on a version.
- No accounts. The server logs who connected (name, address, version), limits connections and traffic per address,
  drops malformed packets, refuses a duplicate virtual MAC (4.5), and reads a ban list file (addresses and copy IDs)
  the owner edits.
- The code is public in the repo; releases carry no ready-built server (O). The owner runs the official one.
- It is tested on the owner's PC first; the host (Akamai or Vultr in Chicago, about US$5) is chosen once it works.

**Battles: direct, with the relay as fallback, from the first version (O, instead of 3.1's relay first)**
- Both fighters check in with the relay as today (HERE, HELLO); the server tells each the other's address as their
  routers translated it; both send to each other at once (3.2). The path is chosen during the round-trip probe before
  the first frame: direct if it answered, the relay otherwise.
- If the direct path dies mid-battle, the link moves to the relay rather than ending the battle; the input delay
  stays as set, so a slower relay shows as waits (I).
- Watchers always use the relay.
- **Always through the server**, a Network page switch, off by default: with it on for either fighter, the battle is
  relayed and neither address is sent.
- Lockstep needs no anti-cheat: both copies compute the battle, so a cheat on one side breaks the battle rather than
  winning it (I).

**The game**
- The server's address comes from a file on main in Dissidia012-pc, fetched as the update check fetches, the last
  good answer kept; a setting points at another server (testing, a community-run one).
- The Network page: Play online, Host a session, Join a session, Always through the server, the server setting, and
  a row that cancels the current choice (O): it stops playing online, leaves the session or stops hosting, so nothing
  is chosen and the chooser shows again.
- Only Play online is remembered across runs (O). Host and Join last for the run; joined addresses stay in the
  recent list. (Today Join is remembered.)
- **The chooser** (O: drawn like the game's menus, with the native dialogs' skin, [native dialogs](native-dialogs.md)):
  when the game goes online with nothing chosen (Online Lobby, Group Battle, Communications), the game pauses
  behind it. Rows: Play online (with the players on this version), Host a session, Join a session, Back. Every
  screen of it has Back (O).
  - Join: a second list with sessions on the local network, recent addresses and Enter an address (the game-styled
    keyboard).
  - Host: a notice with the internet address, copied for sending; or, plainly, why the router refused or that the
    provider shares the address, suggesting Play online. OK carries on.
  - The server unreachable, or not serving this version: the chooser says so and still offers Host and Join.
  - Back: the game then gives its own switch-off message, as today.
- Windows, Linux (Steam Deck) and Android, the chooser by touch too.

## 9. Build order

1. **The server program**: PK's server and the battle relay in a program of their own; the status query; the
   per-address limits, the duplicate MAC refusal and the ban list; tests on loopback.
2. **Versions at log-in**: the copy's version in PK's log-in, the server's groups keyed by it (a PortableKit patch).
3. **Play online**: the address file and its cache, the Network page rows and the cancel row, only Play online
   remembered.
4. **The chooser**: a hook where the game reads the wireless switch with nothing chosen, the game-styled lists,
   notice and keyboard, touch on Android.
5. **Direct battles**: the introductions, the probe choosing the path, the mid-battle move to the relay, and Always
   through the server.
6. **Tests**: two copies on one PC through a local server, then the Deck and a phone over the home network, then a
   rented machine.
7. **Docs**: the players' guide ([playing online](play-online.md)), what the server sees, and how to run a server.

## 10. The server program (step 1, built 15 November 2026)

**What it is.** `port/server/d012_server.cpp`, built by `port/server/CMakeLists.txt` on its own or in the port's
build with `-DD012_BUILD_SERVER=ON`. It links PK's ad hoc code (`host/adhoc/`, `host/profile.cpp`) and the port's
battle relay, now in `port/host/d012_battle_relay.cpp` so the game (while hosting) and the server share it. No
graphics, sound or disc.

**PortableKit changes** (future patch 0107, branch `public-server`):
- A login carries a version tag after the nickname's NUL (`protocol.hpp`, `kVersionMark`), so older servers read
  the nickname as before. `GameProfile::adhoc_version` gives it; the port's is the first 16 digits of the source hash
  and the compiler's kind (`d012_profile.cpp`), what a battle's start checks.
- The server keeps players of different tags apart: groups, scans, broadcasts, chat.
- `ServerConfig::public_server`: player ids are numbers, never addresses; a MAC stays with its first connection
  unless the new login comes from the same address or the old connection has been quiet 10 s; a relay connection
  must name a MAC logged in from its address (it waits 2 s for a login it overtook); per-address limits on
  connections and on bytes a second (an address over its rate is read no faster, as a slow link).
- `refuse_address` and `refuse_mac` hooks, asked at connection and login and again every second for players online.
- `ServerPlayer::version`, `ServerStatus::refused`.

**The battle relay on the server** admits datagrams only from addresses with a player logged in, since anyone on
a version can list the battle groups the tags come from. It answers STATUS (kind 11) with the players online on the
asked version and in all; a query must be at least as long as the answer, so a forged sender gains nothing.

**Running it.**

```
dissidia012-server [--port 27312] [--battle-port 27420] [--ban-list bans.txt]
                   [--max-players 512] [--per-address 8] [--rate 512] [--loopback]
```

The machine's firewall passes TCP 27312-27313 and UDP 27420. The ban list holds one address (as the log shows it)
or one MAC (`02:11:22:33:44:0a`) a line, `#` for comments; it is read again within 5 s of a change, and whoever it
names is dropped. The log (standard output) has each login with name, MAC, version and address, every refusal, and
once a minute, when it changed, who is online by version. It holds players' addresses: keep it briefly.

On Linux, built from a clone of this repository and of PortableKit with the port's patches (`git checkout -b
dissidia 33800c4`, `git am port/portablekit-patches/*.patch`):

```
cmake -S port/server -B build-server -DD012_PORTABLEKIT_DIR=<that clone>
cmake --build build-server
```

**Tested (15 November, Windows, MSVC):**
- PK's loopback ad hoc tests with 21 new checks: the tag in a login, versions apart, the relay tied to the login, the
  rate, the per-address limit and both refusals. All pass, with the existing ones.
- The server on loopback against a script (scratch, not kept): STATUS counts by version, a battle link relayed both
  ways between two admitted fighters, a MAC added to the ban list dropped and refused again. The same with the
  server built by GCC 13 on Linux (WSL Ubuntu 24.04, `g++ -std=c++20`, no CMake there).

- The whole game builds with the changes (clang, llvm-mingw, release), with the server in its build
  (`-DD012_BUILD_SERVER=ON`); `d012_tests` passes, with new checks of the STATUS query and answer.

Steps 1 and 2 of section 9 are done: the version tag at log-in came with the server's changes.

**Two game copies through the server (15 November, this PC, V).** The server with `--loopback` on its usual ports;
two copies of the clang build (`D012_ADHOC=1`, `D012_ADHOC_SERVER=127.0.0.1`, MACs `:01` and `:02`,
`D012_ADHOC_LOOPBACK=1`, the game's code libraries copied from the 11 November party build, whose interface is the
same), driven by live input scripts. Today's Join a session path, no client changes:
- The server logged both in with the same tag, `ULUS10566/c62c0809acf02eac-std`.
- Both entered lobby 9 (Midgar, `LOBBY8`); each listed the other's Friend Card as Online.
- Copy 1 sent Battle Request [1-on-1] at level 20; copy 2 was asked and accepted; both joined the battle's group
  (`75541845`), picked Lightning, and Enter Battle on both started it (stage Empyreal Paradox).
- Both linked "through the relay" (the server's, at 127.0.0.1:27420) with 2 frames of input delay. With a minute of
  random moves on both, the game's check values agreed every 640 frames to frame 3,210, with no wait over 0 ms; HP
  and Brave changed on both.
- STATUS from outside answered 2 players on that tag and 0 on another.

**Not tested yet:** the battle to its end and a rematch, Group Battle and Communications through the server, the
CMake build on Linux, a real machine, two machines, many players.

## 11. Play online (step 3, built 15 November 2026)

**PortableKit** (future patch 0108, branch `public-server`):
- `GameProfile::public_server` (the address, or empty with a note for the player) and `public_server_players`
  (the count, -1 while not known). With them the Network page starts with **Play online**: "Play online   N players
  on"; chosen, "Playing online" and a red **Stop playing online**. Hosting keeps **Stop hosting**; a joined session
  gets a red **Leave the session**. The three cancel rows leave nothing chosen, so the game asks again (the chooser,
  step 4).
- `network.online` (remembered) and `network.online_server` (the server Play online uses instead of the game's own;
  the Ad hoc play section's **Online server** row replaces **Server** for such a game). Environment:
  `D012_ONLINE`, `D012_ONLINE_SERVER`.
- With a public server, a joined session lasts for the run (`adhoc_joined()`); `network.server` from settings.ini is
  no longer used (a copy that joined someone before starts with nothing chosen), but `D012_ADHOC_SERVER` still
  joins, for tests and launchers. Choosing one of Play online, Host and Join ends the others.
- `adhoc_server_address()`: hosting, else joined this run, else an environment's server, else Play online's.
- Without a public server, every row and rule is as before.

**The port** (`host/d012_online.*`, `host/d012_online_rules.*`):
- The address file is `online/server.json` on main (`{"address": "host[:port]"}`; `""` says there is no server
  yet), fetched once a run when first asked for, through the update check's HTTPS (`updates::get`), and kept in the
  data folder as `online-server.json`, used at once the next time. `D012_ONLINE_URL` reads another address or file.
- The count is STATUS on the server's battle port every 5 s while the page shows it, for this copy's tag.
- Player-facing notes: "Finding the server…", "There is no public server yet.", "The server's address could not be
  fetched: …".

**Tested (15 November, this PC, V):** `d012_tests` (new checks of the address file, including the repository's own);
two copies, the server on loopback, `D012_ONLINE_URL` pointing at a file naming 127.0.0.1:
- Copy 1, whose settings.ini still had `network.server=203.0.113.7` from an earlier join: the Network page showed
  Play online first, "0 players on", and no joined session (the old address only under recent). Choosing it showed
  "Playing online" and Stop playing online, and saved `network.online=1`.
- Copy 2 with `D012_ONLINE=1`. Both went online with no address typed; the server logged both with one tag; both
  saw each other in lobby 9; copy 1's page said "2 players on".
- A challenge, accept and battle: "linked ... through the relay", in step at frame 650 on both.
- Stop playing online: back to "Play online", `network.online=0` saved. Join by typing 127.0.0.1: "Joined 127.0.0.1"
  and Leave the session; settings.ini's `network.server` unchanged and `network.online` still 0.

**Not tested:** the file fetched from GitHub (it is not on main until this branch is merged and pushed: until then
the fetch answers 404 and the page says the address could not be fetched), Linux and Android, the Deck, a copy with
no data folder for the cache.

## Sources

- [Oracle, Always Free Resources][oci-free], read 10 November 2026: the allowances, the home region, the idle rule.
- [Oracle's January 2023 email about idle machines][oci-idle], as reproduced by a third party (51sec).
- [Oracle's developer blog][oci-iptables]: iptables on Oracle's Ubuntu images.
- [InfoQ, July 2026][infoq]: the June 2026 cut to the Arm allowance.
- [Google Cloud, free features][gcp-free]: the e2-micro machine and its 1 GB of outbound data.
- [RFC 5128](https://www.rfc-editor.org/rfc/rfc5128.html): hole punching through routers.
- [Oracle, regions and availability domains][oci-regions]: the region list, one region for free and Pay As You Go
  accounts.
- [Oracle, Get Started with the Free Tier][oci-start]: sign-up, the trial, the permanent home region.
- [Oracle, creating an instance][oci-launch], read 11 November 2026: the Create instance page and its options.
- [Oracle, creating a budget][oci-budget]: the fields and the minimum amount.
- [Oracle, public IP addresses][oci-pubip]: an ephemeral address stays assigned while the machine is stopped.
- Read 15 November 2026: [Hetzner, Regular Performance][hetzner-rp] (US prices and traffic),
  [Akamai, CPU pricing][akamai-cpu], [DigitalOcean, Droplet pricing][do-droplets], [OVHcloud, VPS][ovh-vps],
  [datacenters.com, Vultr][vultr-dc] (third-party location list).

[oci-free]: https://docs.oracle.com/en-us/iaas/Content/FreeTier/resourceref.htm
[oci-idle]: https://blog.51sec.org/2023/02/oracle-cloud-cleaning-up-idle-compute.html
[oci-iptables]: https://blogs.oracle.com/developers/post/enabling-network-traffic-to-ubuntu-images-in-oracle-cloud-infrastructure
[infoq]: https://infoq.com/news/2026/07/oracle-cloud-free-tier-limits/
[gcp-free]: https://docs.cloud.google.com/free/docs/free-cloud-features
[oci-regions]: https://docs.oracle.com/en-us/iaas/Content/General/Concepts/regions.htm
[oci-start]: https://docs.oracle.com/en/learn/cloud_free_tier/
[oci-launch]: https://docs.oracle.com/en-us/iaas/Content/Compute/Tasks/launchinginstance.htm
[oci-budget]: https://docs.oracle.com/en-us/iaas/Content/Billing/Tasks/create-budget.htm
[oci-pubip]: https://docs.oracle.com/en-us/iaas/Content/Network/Tasks/managingpublicIPs.htm
[hetzner-rp]: https://www.hetzner.com/cloud/regular-performance/
[akamai-cpu]: https://www.akamai.com/cloud/pricing/cpu
[do-droplets]: https://www.digitalocean.com/pricing/droplets
[ovh-vps]: https://www.ovhcloud.com/en-ca/vps/
[vultr-dc]: https://datacenters.com/providers/vultr
