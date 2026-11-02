# Online play: first investigation (30 October 2026)

The owner asked how the port could get online two-player versus, and whether PortableKit's ad hoc multiplayer code
would do it. This is research only: nothing was built, run or tested, and PortableKit's ad hoc tests were not run
(they raise a firewall prompt on this PC). Code facts come from reading the port and PortableKit; web facts come from
each project's own source and docs, linked. Statements marked **(I)** are inference.

PortableKit paths below are relative to `tools/local/port/PortableKit/` (a local clone, not part of this repo).
`PortableKit-versus` has the same networking code, and none of the port's patches touches it.

## 0. The short answer

- **PortableKit's ad hoc code works as plumbing, but it cannot run Dissidia's online mode today.** It has a
  matchmaking server and a relay that carries all game data, LAN discovery, and a "Host a session" menu. Dissidia's
  Communication Mode lobby, though, is built on a PSP library (`sceNetAdhocMatching`) that PortableKit does not
  implement at all, so the lobby would probably stall. No game has been documented finishing a session through this
  code yet.
- **Even with that fixed, the game's own online mode is the weaker route.** It sends only an 8-direction stick, so
  the port's analog movement and right-stick camera are lost. It is strict lockstep tuned for wireless LAN, over a
  TCP relay that freezes on packet loss. And PPSSPP players report Dissidia 012 desyncing in its ad hoc battles.
- **The recommended route is port-level netplay:** two copies of the port, each running the same battle, exchanging
  only each player's controller input every frame. This is the "delay-based lockstep" model used by Dolphin
  netplay. It reuses what local versus already has: player 2's input goes in through one function, the same way the
  game's own ad hoc code does it, so analog and the right-stick camera survive.
- **Rollback netcode (GGPO-style) is possible later but is the biggest job in the list**, because the port has no
  save states. Start with delay-based; add rollback only if delay feels bad at real distances.
- **The first step is not networking at all:** prove that two copies fed the same inputs stay identical
  (determinism). Every online route depends on it.

## 1. Terms used here

| Term | Meaning |
| --- | --- |
| Ad hoc | The PSP's local wireless mode: PSPs talk directly, no internet. Dissidia's Communication Mode uses it. |
| Lockstep / delay-based netcode | Both machines run the whole game and exchange only inputs. A frame runs once both players' inputs for it have arrived. Inputs are applied a few frames late (the "input delay") so they have time to arrive. |
| Rollback netcode | Like lockstep, but the game runs ahead with a guess of the remote input. When the real input arrives and differs, the game rewinds to that frame and re-runs it. Feels responsive; needs fast save/restore of the whole game state. |
| Determinism | Same start + same inputs = exactly the same result, bit for bit, on both machines. Lockstep and rollback both depend on it. A single different float means the two players see different fights (a desync). |
| Peer-to-peer (P2P) | The two players' PCs send packets directly to each other. Lowest latency. |
| Relay | Both players send to a server, which forwards to the other. Works for everyone, adds latency, someone has to run the server. |
| NAT / port forwarding | Home routers hide PCs behind one address; outside packets are dropped unless the router is told to pass them on (port forwarding), or UPnP does it automatically. This is why "just connect to my IP" often fails. |
| Hole punching, STUN, TURN, ICE | Standard ways to get P2P through routers: a small server tells each side the other's public address, both send at once, and most routers then let the packets through. TURN is the relay fallback when that fails. ICE is the standard that tries direct, then hole-punched, then relayed. |

## 2. What exists today

### 2.1 PortableKit's ad hoc support

**Design: a PSP ad hoc server plus a relay, the same layout as PPSSPP's.**
- Two TCP services on one server (`host/adhoc/protocol.hpp:3-33`):
  - matchmaking ("adhocctl") on TCP 27312: login, groups, scan, ping;
  - a data relay on TCP 27313. Every game socket is one TCP connection to it, so all game data goes through the
    server and "neither player needs a reachable address" (`protocol.hpp:11-14`).
- No direct peer-to-peer path, and no NAT traversal or UPnP (nothing in `host/adhoc/` for it). Joiners only connect
  outward; **the host must be reachable** on TCP 27312-27313: on a LAN, a VPN (Tailscale, ZeroTier, Hamachi), or by
  port forwarding (`menu.cpp:1146-1148`, `1172-1176`).
- Three ways to get a server: "Host a session" runs one inside the game (`host/adhoc/host.cpp:47-85`);
  `--adhoc-server` runs it alone (`host/main.cpp:102-107`); or type any third-party server's address
  (`menu.cpp:1229-1235`).
- LAN discovery on UDP 27314 (broadcast and multicast), or a typed address for VPNs without broadcast
  (`host/adhoc/discovery.hpp:6-13`).
- The opcodes match the PRO ad hoc server protocol PPSSPP uses, and the code says PPSSPP clients could use it
  (`server.hpp:3-6`). Its relay on 27313 matches the layout of PPSSPP 1.20's new relay (aemu_postoffice, section
  3.1). **Interop with PPSSPP has never been tested.**

**What it implements of the PSP's network libraries** (`host/hle/hle_adhoc.cpp`, `docs/HLE_COVERAGE.md`):

| Library | State |
| --- | --- |
| sceNetAdhoc PDP (datagrams), PTP (streams) | Implemented |
| sceNetAdhocctl (scan, groups, peers) | Implemented, except `GetPeerInfo`, `GetState`, `GetAddrByName` and GameMode. `Create`, `Connect`, `Join` are marked UNVERIFIED: "no game traced yet" (`hle_adhoc.cpp:414-447`) |
| **sceNetAdhocMatching** (the host/join handshake many games build their lobbies on) | **Not implemented at all** (`HLE_COVERAGE.md:77`) |

**How mature it is.**
- Written in about a week (19-26 September 2026) inside the Monster Hunter Portable 3rd port and moved into the
  framework; nothing since. The menu still says "Online Guild Hall" (`menu.cpp:1137`).
- Its tests cover the server, client and discovery over loopback only, not the game-facing layer, and not any real
  network (`tests/adhoc_tests.cpp:1-4`; `CMakeLists.txt:228-243`).
- No document in either repo says a full multiplayer session has worked through this framework code.

**In the Dissidia port.**
- Compiled in, off by default (`settings.hpp:127`, Network > Ad hoc play), product code `ULUS10566`
  (`port/host/d012_profile.cpp:87`).
- Dissidia imports 10 `sceNetAdhocMatching` functions (14 call sites) and `sceNetAdhocctlGetPeerInfo` (3); all
  missing ([gap analysis](portablekit-gap-analysis.md); [bring-up log](bring-up-log.md) lines 171-172). Missing
  functions return 0 (`host/system.cpp:47-59`), so **(I)** Communication Mode would never see a matching event and
  its lobby would stall. Not run.

### 2.2 The game's own online versus

From [local versus, section 8](local-versus.md) and [pad module](../research/pad-module.md) (lines 420-439):
- Communication Mode, FIELD mode 5. Each PSP draws its own player's view.
- Strict lockstep on input: each frame the game packs the pad into 16 bits (`0x0890CE58`), sends it with a short
  history of earlier words (`0x0890D664`), waits until every player's word for the frame has arrived, then unpacks
  them into the raw pads (`0x0890D090`, loop at `0x0890DE30`). Battle logic runs at 30 Hz.
- The 16 bits hold the buttons and **only 8 stick directions**; there are no spare bits.
- **(I)** Whether the game applies inputs a few frames late (which would tolerate internet latency) is not traced
  yet; if it applies them on the same frame, any one-way delay above 33 ms slows the battle down.
- **(I)** Over PortableKit's TCP relay, one lost packet stalls everything until it is resent (often 200 ms or
  more); the game's own word history, meant to cover wireless losses, does not help over TCP.

### 2.3 The local versus input path (what port netplay would reuse)

- Player 2's controller reaches the game in one place: the port's wrapper of `sceCtrlReadBufferPositive`, once per
  game pad read. It writes player 2's buttons and stick into raw pad 1 and sets it to the "fed" mode, **the same
  thing the game's ad hoc code does** (`port/host/d012_versus.cpp:307-319`, `815-948`).
- An override makes slot 1 human (`d012_versus.cpp:427-450`); the opponent pick and player 2's right-stick camera use
  the same pad record.
- **(I)** A remote player 2 fits that seam: give the wrapper the remote player's input for the frame instead of the
  second local pad, waiting (PortableKit's `kernel().wait_host`, as `hle_adhoc.cpp:125-143` does) until it arrives.
  Analog movement and the right-stick camera come along unchanged.

## 3. How others do it

### 3.1 PPSSPP

- Its ad hoc server is a port of AEMU's "PRO online" server: matchmaking only on TCP 27312, exchanging each player's
  address ([proAdhocServer.cpp](https://github.com/hrydgard/ppsspp/blob/master/Core/HLE/proAdhocServer.cpp),
  [proAdhoc.h](https://github.com/hrydgard/ppsspp/blob/master/Core/HLE/proAdhoc.h)).
- Classic mode sends game data **directly between players over UDP**, with a port offset (10000 recommended), so it
  needs port forwarding, UPnP or a VPN
  ([sceNetAdhoc.cpp](https://raw.githubusercontent.com/hrydgard/ppsspp/master/Core/HLE/sceNetAdhoc.cpp);
  [how to play](http://www.ppsspp.org/docs/multiplayer/how-to-play/)).
- PPSSPP 1.20 added a **relay**, aemu_postoffice on TCP 27313 (GPL-3.0), so no port forwarding is needed, at the cost
  of latency through the server; 1.21 adds a UDP option for it
  ([1.20 notes](http://www.ppsspp.org/news/release-1.20/),
  [design](https://github.com/Kethen/aemu_postoffice/blob/main/design.md),
  [network settings](http://www.ppsspp.org/docs/settings/network/)).
- About 20 public servers are listed, most with relay
  ([server list](http://www.ppsspp.org/docs/multiplayer/adhoc-servers/),
  [quickstart](http://www.ppsspp.org/docs/multiplayer/quickstart/)).
- Its docs say ad hoc works best for latency-tolerant games (Monster Hunter, Ridge Racer); fighting games are not
  named. **Dissidia 012 has no official entry; forum users report it desyncing mid-battle, "even on PC only"**
  ([forum thread](https://forums.ppsspp.org/showthread.php?pid=145836), secondary source).
- PPSSPP has no netplay layer of its own (no input sync or rollback); the libretro core lists netplay as unsupported
  ([libretro PPSSPP](https://docs.libretro.com/library/ppsspp/)).

### 3.2 Netcode models elsewhere

- **Delay-based lockstep.**
  - Dolphin netplay: identical game and settings, a pad buffer (about one frame per 15 ms of latency), a desync
    checker, and a "traversal server" that only helps hole-punching (it never relays game data)
    ([Dolphin guide](https://dolphin-emu.org/docs/guides/netplay-guide/),
    [TraversalServer.cpp](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/Common/TraversalServer.cpp)).
  - Glenn Fiedler's write-up of lockstep over UDP: every packet repeats all unacknowledged inputs, so a lost packet
    costs nothing; TCP stalls on loss ([deterministic lockstep](https://gafferongames.com/post/deterministic_lockstep/)).
- **Rollback.**
  - GGPO (MIT; builds on Windows only) needs the game to save and load its whole state and to advance frames without
    drawing; fully deterministic; about 1 frame of delay for fighting games; a "sync test" mode catches
    nondeterminism ([developer guide](https://github.com/pond3r/ggpo/blob/master/doc/DeveloperGuide.md)).
  - RetroArch saves the whole emulated machine every frame, which works only where save states are clean and fast
    ([netplay](https://docs.libretro.com/development/retroarch/netplay/)).
  - NetherRealm spent about 2 man-years on save/load alone when adding rollback to Mortal Kombat X (secondary report
    of their GDC talk: [Infil](https://words.infil.net/w02-netcode-p5.html)).
- **Recompiled ports have done this already.**
  - Conker's Bad Fur Day Recompiled (netplay fork): rollback, room codes, LAN discovery, relay by default ("no port
    forwarding needed") ([README](https://github.com/easymod0/CBFD-Recompiled-Netplay)).
  - psxrecomp: rollback or delay, a WebSocket lobby, ICE through libjuice, a TURN relay fallback, direct IP for LAN.
    Its licence is non-commercial, so take ideas only, not code
    ([NETPLAY.md](https://raw.githubusercontent.com/RetroPortingToolKit/psxrecomp/master/docs/NETPLAY.md)).

### 3.3 Rollback cost for this port (I)

- Copying the 32 MB of guest RAM takes roughly 2-4 ms; 8 frames of history is 256 MB.
- The hard part is everything outside guest RAM that must be saved and restored exactly: PortableKit's kernel (PSP
  threads, semaphores, timers), the GE graphics state, audio, file positions, and the port's own state (camera,
  BGM). Guest threads' host stacks cannot simply be copied.
- Re-running up to 7-8 frames inside one display frame with drawing and sound suppressed means hooking the GE and
  audio paths.
- PortableKit has no save states today (`savestate` and `rollback` appear nowhere in it). Rollback first needs
  robust save states, a project in itself.

## 4. Connecting the players

| Option | What the player does | Who runs a server | Notes |
| --- | --- | --- | --- |
| LAN, or a VPN (Tailscale, ZeroTier, Hamachi) | Both join the VPN; one hosts, the other types the address | Nobody | Works with PortableKit's ad hoc today; free; one extra app to install |
| Direct IP + port forwarding | Host forwards a port in the router | Nobody | Fails behind carrier-grade NAT and mobile internet ([PPSSPP](http://www.ppsspp.org/docs/multiplayer/how-to-play/)); many players can't set it up |
| UPnP | Automatic port forwarding | Nobody | Router may not support it, and may report success without opening the port ([libretro](https://docs.libretro.com/guides/netplay-getting-started/)); miniupnpc is BSD-3 ([miniupnp](https://github.com/miniupnp/miniupnp)) |
| Room code + hole punching | Type a short code | A tiny rendezvous server (almost no traffic) | Works through most home routers, not all ([RFC 5128](https://www.rfc-editor.org/rfc/rfc5128.html)); libjuice (MPL-2.0) does standard ICE ([libjuice](https://github.com/paullouisageneau/libjuice)) |
| Relay fallback (TURN or a small UDP relay) | Nothing extra | A small VPS | Works for everyone; adds latency. Inputs are tiny: about 15-30 MB per player per hour (I). Hetzner's smallest plan is €5.99/month ([Hetzner](https://www.hetzner.com/cloud/)); coturn is the standard TURN server ([coturn](https://github.com/coturn/coturn)) |
| Public PSP ad hoc relays (PPSSPP's list) | Type the server name | Community volunteers | Only for the game's own ad hoc route; TCP |
| Steam Datagram Relay | — | Valve | **Not available**: needs a Steam release ([SDR](https://partner.steamgames.com/doc/features/multiplayer/steamdatagramrelay)) |
| Epic Online Services P2P | — | Epic (free relay) | Closed SDK, an Epic product registration and credentials in a public repo; may not suit a fan port of a Square Enix game (I) ([EOS relay control](https://dev.epicgames.com/docs/api-ref/enums/eos-e-relay-control)) |
| Discord | Share room codes in a channel | — | Discord's game networking and lobbies were shut down in May 2023 ([change log](https://docs.discord.com/developers/change-log)); use it for codes and chat only |

## 5. The routes compared

| | A. Game's own ad hoc over PortableKit | B. Port netplay, delay-based (recommended) | C. Port netplay, rollback |
| --- | --- | --- | --- |
| How it works | The game's Communication Mode, its packets carried by PortableKit's relay | Two copies of the port exchange inputs each frame; the existing local versus feeds the remote player as player 2 | As B, but runs ahead and rewinds on wrong guesses |
| Feel | Stock PSP: digital stick, no right-stick camera; strict lockstep; TCP stalls on loss | Full port features (analog, right-stick camera, 60 fps display); input delay of a few frames | Most responsive |
| Biggest missing piece | `sceNetAdhocMatching` (10 functions), an unverified join path, the frame-timing trace | A lockstep layer, a desync test, identical start state, a full-screen player-2 view on the second PC | Save states of the whole runtime, re-simulation without drawing |
| Size (I) | Medium, with uncertain payoff | Medium | Large; months |
| Cross-play with PPSSPP | Possible in principle, but desync risk is high (different VFPU maths) | No | No |

**Why B over A.** A keeps the PSP's limits (digital stick, wireless-LAN timing) and needs a large missing library
built before anything can be tried, and the only reports of this game over emulated ad hoc are of desyncs. B builds on
the part of the port that already works and that the owner controls end to end: input delay, redundancy, desync
checks and the starting state.

## 6. Determinism: the first thing to prove

- **Same build, same floats.** Both players need the same build of the port. The setup ships a pinned compiler
  (llvm-mingw 20260922, `tools/port/package_installer.py:34`, `74`) and the clang build passes no `-march=native` or
  FMA flags (PortableKit's AVX options are MSVC-only, `CMakeLists.txt:62-63`), so **(I)** two players with the same
  setup version and the same disc should get identical code. Mixing an MSVC build with a clang build is not safe:
  VFPU square roots and similar go through each C library's maths ([gap analysis](portablekit-gap-analysis.md),
  section 3; [floating point determinism](https://gafferongames.com/post/floating_point_determinism/)). A lobby
  should exchange a build and disc hash and refuse mismatches.
- **Things to rule out** before trusting input-only sync:
  - "At 60 fps a battle's timeline sometimes drifts" ([graphics triage](graphics-triage.md), lines 81-82): is that
    only which frame a screenshot shows, or the simulation?
  - The battle camera's time step comes from a global ([battle camera](../research/battle-camera.md), 1.5).
    Movement is camera-relative, so each fighter's camera is part of the simulation.
  - The port's analog movement and right-stick camera write into game state from local input; on both copies they
    must come from the exchanged input, never straight from the local controller.
- **How to prove it (I).** Run two copies of local versus on one PC, fed the same recorded input script (the
  `D012_INPUT_SCRIPT` runs already used for testing), and compare a per-frame hash of the battle state, or start
  with the fighter fields `D012_TRACE_VERSUS` already prints (`d012_versus.cpp:321-371`).

## 7. Suggested phases

**Phase 0: determinism (no networking).** The two-copy local test above, until runs match for whole battles.
Decide how both copies reach the battle with the same state: the same save, or the host sends the characters,
builds, stage, rules, BGM and random seed.

**Phase 1: LAN and VPN netplay.**
- UDP; each packet carries the frame number and every input not yet acknowledged.
- A fixed input delay chosen from the measured ping; the game waits if an input is late.
- Exchange a state hash every few seconds and stop with a message on a mismatch.
- The second PC shows player 2's view full screen (the split-view code already renders it).
- Players connect by typed address, which already covers LAN and VPN.
- Windows Firewall will prompt the first time.

**Phase 2: internet without setup.** Room codes through a tiny rendezvous server with hole punching (libjuice), a
relay fallback on a cheap VPS, codes shared on Discord. Optional UPnP.

**Phase 3 (optional, large): rollback.** Only if delay-based feels bad at typical distances. Needs save states of the
whole runtime first.

**Side option: the game's own ad hoc.** Implementing `sceNetAdhocMatching` in PortableKit would let LAN or VPN
players use Communication Mode as on a PSP, and possibly PPSSPP's public relays. Worth it mainly as a PortableKit
contribution or for PPSSPP cross-play, which would likely desync anyway.

## 8. Open questions for the owner

- Is a VPN (Tailscale or ZeroTier) acceptable for a first online release, or should it work with no extra software
  from the start?
- Is running a small server (about €6 a month, or a free tier) acceptable, or should it be server-free?
- Does online play need the port's features (analog, right-stick camera), or would the stock PSP experience do for a
  first version?
- How should the two copies agree on characters and builds: both players' own saves, with the builds sent over, or
  fixed builds?

The owner's answer (30 October): true native online play, with room for rollback netcode later.

## 9. Rollback readiness (audit, 30 October 2026)

A read-only audit of PortableKit (`PortableKit-versus`) and the port, for what rollback would need. Paths in
PortableKit are relative to its root.

**Better than feared.**
- **One host thread runs every guest thread.** A guest thread's registers are one ~0.9 KB `AllegrexContext` kept in
  the kernel's thread table; blocking calls swap contexts in place (`host/kernel/kernel.hpp:18-27`,
  `kernel.cpp:315-322`). No guest state lives on host stacks at the runtime's outer dispatch boundary
  (`include/psprecomp/runtime.hpp:126-142`, `:296-299`). So the state can be copied, not just serialised.
- **Guest memory is two flat buffers**: 32 MiB of RAM from `0x08000000` (the kernel partition included) and 2 MiB of
  VRAM plus the scratchpad (`src/guest_memory.cpp:92-116`). Two memcpys capture everything the guest can address.
- **The game's clock is emulated**: it advances with dispatch counts and events, not real time
  (`kernel.cpp:502-506`, `:721-731`); the pad samples' time stamps use it too. Guest threads interleave only by
  emulated time and dispatch counts, in ordered tables. RAM starts zeroed and no host pointers leak into guest memory.
- **A natural save point**: the main thread's pad read (`sceCtrlReadBufferPositive`), once per 30 Hz logic frame.
  There every other thread is parked in the kernel's table.

**Real-time leaks to close first (for plain lockstep too).**
- The "busy clock" adds real elapsed time when one call stalls for 100 ms or more: a slow disc read, a shader
  compile (`kernel.cpp:700-719`). Off with `D012_NO_BUSY_CLOCK=1`.
- The PSP clock's wall time starts at the host's clock at boot, in the host's time zone (`host/hle/hle_system.cpp:25-33`,
  `:213-237`); the game reads it in battle. Phase 0 pins it from the port (`D012_SYNC_EPOCH`, `port/host/d012_sync.cpp`).

**What rollback still needs.**
- **Save and restore of the host-side state**, none of which has snapshot code today: the kernel singleton (threads,
  semaphores, event flags, callbacks, timers, the clock, the allocator), about 20 function-local statics in the HLE
  modules (display, GE state, pad counters, audio channels, SAS voices, ATRAC positions, file positions), and the
  port's own state (versus, split view, camera). The FFmpeg ATRAC decoder cannot be copied, but PortableKit's seek
  path re-warms it to identical output (`hle_atrac.cpp:291-302`).
- **A "simulate only" frame.** There is no frame-skip path: fast loading still draws, and the no-render switch is
  process-wide. Presenting, GE drawing and audio mixing must be skipped per frame. The game's own draw code is guest
  code and still runs: about half of its 6.5-7.3 ms per frame.
- **Size and cost (I):** about 34 MiB of memory plus under 1 MiB of host state per save; about 2-4 ms to copy. A
  desktop could re-run about 3 logic frames (about 100 ms) inside one frame; the Steam Deck is unmeasured.
- **Masking:** bytes that differ between machines must be left out of desync hashes: frames the renderer writes back
  into guest VRAM (and the EX Burst's copy of the screen at `0x095CD140`), FFmpeg-decoded music, OS font glyphs
  (Windows against Proton).
- **Shared view logic:** split-screen framing and the FOV slider change the lock-on camera's own update
  (`port/host/d012_split_view.cpp:1199-1215`) and movement is camera-relative, so both copies must run the same view
  logic, whatever each shows. Also every input source (both pads, R3, pad-loss pauses) must come from the exchanged
  record, and the pause menu must pause both.

**Effort, smallest first (I):** lockstep prerequisites (small); memory, CPU and port state save (small); kernel save
(medium); HLE module save plus a GGPO-style sync test (save, step, load, step, compare), where most of the risk is
(medium to large); simulate-only frames (medium, PortableKit patches); the rollback driver and netcode (medium to
large); performance work such as dirty-page tracking and Steam Deck measurement (medium).

**What it means for the plan.** Delay-based lockstep first is still right, and nothing it needs is wasted for
rollback: the same input record, the same determinism fixes and the same desync check. Rollback adds save states on
top.

## 10. Phase 0 results (30 October 2026)

**The tools.**
- `D012_SYNC_TRACE=FILE` (`port/host/d012_sync.cpp`, called from the versus pad-read wrapper): after every game pad
  read, one record holding the read's number, whether a battle is loaded and versus is on, both players' inputs as the
  game got them (the read's first sample; raw pad 1), and a hash of each of the 512 64 KiB pages of guest RAM.
  `D012_SYNC_DUMP=N[,N]` also writes the whole of guest RAM after those reads.
- `D012_SYNC_EPOCH=SECONDS` pins the PSP clock's wall time to that Unix time, in UTC, by rebinding
  `sceRtcGetCurrentClockLocalTime` and `sceRtcGetCurrentTick`, the game's only current-time imports.
- `tools/port/sync_compare.py A B [--align battle|boot]`: the first read whose inputs or memory differ, and every
  page that ever differs, with how often and from when.

**The runs.** All on the clang build (`tools/local/port/build-d012-online-clang`), the scripted route `r_p2nochase`
(boot, menus, a Quick Battle versus with player 2 chasing), each from its own copy of `build/port/data-versus-video`,
unthrottled. Traces and dumps in `build/port/online/`.

| Pair | Result |
| --- | --- |
| `D012_NO_BUSY_CLOCK=1`, `D012_SYNC_EPOCH`, data copied with its file times (`pinp_a`, `pinp_b`) | **Identical**: 10,799 pad reads, the same inputs at each, and all 512 pages the same at every read, from boot through the menus and 4,479 battle frames |
| The same, data copied with new file times (`pin_a`, `pin_b`, `dump_a`, `dump_b`) | 3 pages differ; the RAM dumps show 14 bytes at boot and 8 in battle, all a date and time (2026-10-30 14:43:03 against 14:45:48): the save folder's modification time, which PortableKit's save-data reads report (`host/hle/hle_savedata.cpp:578`). Data, not simulation |
| No pins (`free_a`, `free_b`) | Differs from read 0; by the battle 64 pages differ, spreading through it: the simulation diverges |
| Pinned, split-screen field of view 130% against 100% (`fov130` against `pinp_a`) | The same until read 8,275, when the fight begins; then 56 pages, spreading: the view settings change the simulation |

**What it shows.**
- **The port is deterministic on one PC once the two clock leaks are closed.** The same build, the same data and the
  same inputs give bit-identical guest RAM for a whole session. Lockstep from boot is possible; no save states are
  needed for delay-based play.
- **Both pins matter**, and they must be on in netplay: the busy clock off, and the PSP clock started from a time
  both copies agree on (the host's).
- **The camera is part of the simulation.** The split-screen framing and field of view change the lock-on camera's
  own update (section 9), so both copies must run the same view logic with the same values, whatever each player
  sees on screen.
- **The scripted inputs land on the same game frames in every run** (the same pad read count and the same inputs at
  each): render frames follow the game's own frames at a fixed frame rate.
- **Save data reaches guest RAM**, file times included. Two players' different saves will make their copies differ
  from boot.

**Not tested yet.**
- Another frame rate: since tested, with replayed input (section 11): it does not matter.
- Another PC, Windows against Proton (the C library's maths under Wine may round differently), clang against MSVC.
- Two different save files.

**Next (phase 1 groundwork).**
1. **One input record per game frame.** Both players' buttons and sticks, both right sticks and the extra buttons
   (R3), taken once at the pad read and used by every consumer (the pad buffer, raw pad 1, the cameras, the pad-loss
   pauses). `D012_SYNC_REPLAY=FILE` would feed a recorded run's inputs back per game frame. That is the netplay seam,
   and it allows the frame-rate test.
2. **The same start on both copies without save states.** The simplest way the results allow: the joining copy boots
   from the host's save (sent at connection, a few hundred KB, kept in a separate folder so the joiner's own save is
   untouched), with the host's clock epoch and view settings, and both run in lockstep from boot, menus included.
   Player 2 then fights with the host's builds, as local versus already does.
3. **Mask machine-dependent bytes** in the desync hash once two PCs are compared (renderer write-back, decoded music,
   font glyphs).

## 11. The per-frame input record (30 October 2026)

**What it is** (`port/host/d012_input.cpp`). One record per game pad read holding everything the game and the port's
features act on: player 1's buttons and both sticks as the read got them, the extra buttons (L2, R2, L3, R3) and
whether a gamepad is connected; the same for player 2's second gamepad; and the answers to the system dialogs. It is
taken by an innermost wrapper of `sceCtrlReadBufferPositive`, installed before every other, and the right-stick
camera (R3), local versus (player 2's pad, the pad-loss pauses) and the BGM selector (L2, R2) now read it instead of
PortableKit's live pads. Without the switches below it only mirrors the pads, so play is as before.

**Switches.**
- `D012_INPUT_RECORD=FILE` writes every frame's record to FILE (48 bytes each).
- `D012_INPUT_REPLAY=FILE` takes each frame's record from FILE instead of the pads.
- With either, every sample of the read is rewritten from the record, so the game sees exactly what it holds.

**The system dialogs were a second input channel.** PortableKit's own windows answer the PSP's dialogs on whatever
frame the player clicks, outside the pad read. A first replay stalled at the boot-time autosave notice, and a second
diverged at read 1,651, where the title screen's save list (`LISTLOAD`) took the player's pick. With the record in use:
- **The message dialog** (`sceUtilityMsgDialog*`) is run by the port, with PortableKit's life cycle and window: an
  answer waits for the next pad read, goes into that frame's record, and reaches the game at the dialog's next poll.
- **The save-data dialogs that ask** (the save lists, saving or deleting one save) stay PortableKit's, gated: once
  visible their polls report VISIBLE until a frame's record releases them, which while recording is the frame after
  the window closed with the answer. The choices (a save's name or "cancel") go to `FILE.saves`, and a replay hands
  them to PortableKit as `D012_SAVEDATA_ANSWERS`.

**Checked** (clang build, `build/port/online/`; the clock pins on throughout):
- A replay of a recorded run (`rec` from the scripted route, `rep` with only the screenshot steps left in the script):
  the same inputs and all 32 MiB of guest RAM the same at every one of the 10,799 reads, the message dialog answered
  at read 460 and the save list released at read 1,650 in both.
- **Frame rate:** the replay at 60 fps (`rep60p`, PortableKit's perf log reading "interpolation 60") and at 30 fps
  (`rep30p`, no interpolation) are identical for all 17,124 reads they share, and the 60 fps one matches the recording.
  The display frame rate does not touch the simulation, so two players may use different ones.

**Not done.** The OSK (name entry) is not in the record; it does not come up on the route to a versus battle. A
replay that runs past its last record goes back to the live pads.

**Where players will find it (owner, 30 October).** Online play belongs behind the game's own Communication Mode
online option, not the port's "Quick Battle (Player vs Player)" row. The phase 0 and record work uses the versus row
only as a test harness: it is where player 2's input already enters the game, and the scripted route reaches a battle
through it. Open question for later: two copies started in lockstep from boot share their state for free, but a
match started from a menu part-way through a session needs either the host's state sent to the joiner (save-state
transfer, which rollback also needs) or both copies restarted into a synchronised session.

## 12. Two copies in lockstep on one PC (31 October 2026)

**What it is** (`port/host/d012_net.cpp`). Delay-based lockstep over UDP. Each copy adds its player's input to the
frame `delay` reads ahead and sends it to the other. Each frame then waits until the other copy's input for it is in,
so both copies run the same frames with the same records (section 11). Player 1's copy contributes player 1's pad and
the system dialogs' answers. Player 2's copy contributes player 2's pad and draws no message dialog: like a replay, it
gets the answer from the record. Every packet carries all the frames the other copy has not confirmed (up to 64), so
a lost packet costs only a wait.

**Switches.**
- `D012_NET=host:PORT` waits for the other copy on `127.0.0.1:PORT`, as player 1.
- `D012_NET=join:ADDRESS:PORT` joins as player 2.
- `host:ADDRESS:PORT` listens elsewhere (`0.0.0.0` for a LAN). Windows asks to allow that; loopback asks nothing.
- `D012_NET_DELAY` sets the frames of input delay (3, about 50 ms) and `D012_NET_WAIT` how long to wait at start-up
  for the other copy (300 s).
- `D012_NET_PAD=2` gives this copy's player the second gamepad: two copies on one PC, a controller each (untried
  with real controllers; SDL may keep a window in the background from reading them).

**At the handshake** player 1's copy sends the PSP clock's epoch and the save answer for the title screen's list
(`D012_SAVEDATA_ANSWERS`, "first" by default). Both copies take them and turn the busy clock off, so the section 10
conditions hold without setting them by hand.

**In-band check.** Each read hashes an eighth of a MiB of guest RAM, covering all of it every 64 reads. The two
copies swap these hashes and log `[net] the two copies went apart ... at read N` on the first mismatch. That catches a
desync without trace files.

**Checked** (clang build, `build/port/online/`, two windows on this PC, each with its own copy of the versus test
data and its half of the scripted route: player 1's steps on one, player 2's on the other):
- `net1`: 6,940 reads, through boot, the autosave notice (answered on player 1's copy, read 463 on both) and the save
  list (released at read 1,107 on both). Same inputs and all 32 MiB of guest RAM the same at every read.
- `net2`: boot to a versus battle with player 2 fighting from the second copy, 10,799 reads. The battle started at
  read 6,323 on both, and the inputs and all of guest RAM were the same at every read (`sync_compare.py --align
  boot`). Screenshots of the two windows match. The in-band check agreed throughout.
- The longest waits for the other copy were 116 to 548 ms, during loads. Neither copy fell behind otherwise.

**Limits of this step.**
- It starts from boot: both copies must start together with the same data, the same build and the same view settings
  (field of view, framing; section 10). Nothing checks those yet beyond the RAM hash.
- Only the first save list is answered for both copies. A later save prompt opens player 1's window, and player 2's
  copy would wait for an answer it cannot get.
- Player 2's copy shows the local split screen. A single view per player, and the Communication Mode entry point, come
  later.
- A copy that quits leaves the other waiting 10 s. It then goes on with the other player idle.
- Only loopback has been tried, not a LAN, another PC or the Steam Deck.
