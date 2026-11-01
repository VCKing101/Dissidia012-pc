# Communication Mode: what the game asks of the network

Map of Dissidia 012's (ULUS10566) Communication Mode, for online play that keeps the game's own lobby and battle and
replaces the PSP ad hoc network under them (owner, 31 October 2026; [online play](online-play.md) has the research).
Static reading of main, menu and battle, plus offline runs of the port that open each menu row with ad hoc off
(`build/port/online/comm*/`). V = verified in code at the addresses given, I = inferred. "net" is the battle sync
object, `*(0x089D8364)+0x10D8`; "mgr" is `*(0x089D8364)`.

## 0. The short answer

- Communication Mode has five rows. Three use the network, each in its own way: **Online Lobby** (one-on-one
  challenges in 16 named lobbies), **Group Battle** (rooms of up to 10 PSPs for party battles) and **Communications**
  (Friend Card and quest exchange while the PSP sleeps). Friend Cards and Artifacts are local.
- **The battle is already a lockstep of inputs.** Each PSP simulates both fighters and both cameras from the two
  players' pads, and draws only its own player's view. The pads go in the same slots on both machines (`raw[0]` =
  player 0, `raw[1]` = player 1); only which slot is local differs. So the two machines run the same battle, much as
  the port's two copies did in [section 12](online-play.md#12-two-copies-in-lockstep-on-one-pc-31-october-2026).
- **The game's battle netcode is old but easy to swap out.** It sends a 16-bit word a frame (8 stick directions), has
  0 to 2 frames of input delay, and freezes the battle while it waits. All of it is one function, `0x0890D664`.
  Replacing it with the port's lockstep gives full analog, the right stick, chosen input delay and UDP redundancy,
  with the game's lobby, setup and view untouched.
- **Everything before the battle is ordinary PSP ad hoc**: adhocctl groups, `sceNetAdhocMatching`, three reliable PTP
  streams and PDP datagrams. That traffic is not timing-critical, so it can run over any modern connection (a room
  server, a relay, hole punching) as long as the port provides those PSP functions faithfully. Section 6 lists what
  they must do.
- Not known yet, and the next thing to trace: whether the lobby's setup really gives both machines an identical
  battle start (nothing in the battle stream syncs the random number generator or any state).

## 1. The five rows

| Row | What it is | Network | Section |
| --- | --- | --- | --- |
| Online Lobby | "Enter a room and engage in battles against friends." 16 lobbies: 1-8 "For customized characters" (Cornelia, Fynn, Palamecia, Sasune, Baron, Tycoon, Bal, Figaro), 9-16 "For preset characters" (Midgar, Balamb, Galbadia, Esthar, Alexandria, Lindblum, Besaid, Zanarkand) | adhocctl group `LOBBY%d`, matching P2P with 2 peers, then a battle session | 2 |
| Group Battle | "Team up with friends to take part in party battles." Create Room (Custom), Create Room (Preset), Search for a Room | adhocctl group `MLOBBY`, matching host/guest with up to 10, then a battle session with spectators | 3 |
| Friend Cards | "View your Friend Cards and battle against ghost opponents." The list reads "Welcome to the Offline lobby." | Opens the Online Lobby's matching object (Init and Create), no group joined (I) | 2 |
| Artifacts | Artifact history and renaming | None found | - |
| Communications | Send Friend Card, Send Original Quest, Import a Friend Card. "By putting your system into Sleep Mode, you can exchange your quests and Friend Card with people you pass... up to 16 players' data." | `sceNetAdhocDiscover` into group `DISCOVER`, then 27 KB PDP datagrams | 5 |

Offline (PortableKit's ad hoc off, so the PSP's WLAN switch reads off), lobby 1 and Create Room (Custom) end in the
game's "An error has occurred" with codes 0x80410805 and 0x80410803. Opening the Online Lobby, Friend Cards and
Communications logs `sceNetAdhocMatchingInit(0xAD4C)` and `sceNetAdhocMatchingCreate(3, 2, 100, 0x24B0, ...)`,
which PortableKit only stubs.

## 2. Online Lobby (one-on-one)

Driven by main 0x088F7834..0x088F8578 from menu 0x08A1CBC4..0x08A1E034; lobby object main 0x089096B0. V unless marked.

1. **Join the lobby.** `sceNetAdhocctlConnect(sprintf("LOBBY%d", n))` (0x089098DC, string 0x089CDD30), n the menu's
   lobby number (0x09B6D848). Whether n counts from 0 or 1 is not settled (I).
2. **Advertise.** Matching in P2P mode (3), 2 peers, port 100 (0x08909758 -> 0x08903684). Every PSP's hello carries
   a 0x1258-byte block (built at 0x088F8578): a u64 id, its player card (I: the Friend Card) from the save, a list of
   its custom characters (12 bytes and two shorts each), challenge settings at +0x1240, a byte from
   `scePowerCheckWlanCoexistenceClock`, and **8 random ASCII digits** at +0x124D (seeded from the system clock). Each
   peer's hello fills one of 16 list slots (0x0890A358).
3. **Challenge.** A: `SelectTarget(B, its 0x1258 block)` (0x089099D8). B sees REQUEST with A's block; B accepts with
   `SelectTarget(A, nothing)` (0x08909B04) or refuses with `CancelTarget` (0x08909B60; A sees DENY). A can withdraw
   (0x08909ABC; B sees CANCEL).
4. **Matched.** On ESTABLISHED both compare MACs (0x0890A080, `sceWlanGetEtherAddr`): **the lower MAC hosts**, and
   the battle's group is named by **the lower-MAC player's 8 digits**.
5. **Leave the lobby** (`MatchingStop`, `AdhocctlDisconnect`, 0x08909984) and start the battle session (section 4)
   with the partner's MAC, hosting or joining (menu 0x08A1E034 -> 0x088F68E8).

## 3. Group Battle (rooms for party battles)

Comm session main 0x0890A470, driven from menu 0x08A6EB58..0x08A6F03C. V for calls and layouts, I for meanings.

- Both roles join adhocctl group `MLOBBY` (menu string 0x08A6F938). The room's host is a matching **parent** (mode
  1), guests are **children** (mode 2), up to **10** PSPs, port 100 or above, hello data 0x970 bytes.
- The host's hello is the room: 10 member slots of 0xF0 bytes (MAC and card each), the room type (custom or preset)
  and the room's 8 random digits. "Search for a Room" lists the parents' hellos (I). A guest asks to join with
  `SelectTarget` carrying its own 0xF0 member record.
- The host updates the advertised room with `SetHelloOpt` (0x08903FDC) and pushes data to members one at a time
  with `SendData`, sending the next on DATA_ACK (0x0890BC8C, 0x0890BDD8). Guests `SendData` to the host (0x0890C16C).
- When the room is ready, the menu starts the battle session (section 4) with the room's member MACs (menu
  0x08A6CE54, 0x08A77BF8). Each battle is two fighters; the other members watch as spectators (section 4.3). I: party
  battles are teams of up to five, fighting one-on-one in turn.

## 4. The battle session

Session object 0x089D8364 (made by 0x08906BDC), ticked once a frame by the network manager task (0x0890E794).

### 4.1 Setting up

- Both leave the lobby group. The host `sceNetAdhocctlCreate`s the 8-digit group; the guest `Scan`s and `Join`s it by
  name, rescanning up to 50 times (0x08906E08, 0x08907780). Then it waits until `GetPeerInfo` answers for every
  expected MAC (0x089078CC).
- It opens **three PTP streams** with buffers 0x5000, 0x400 and 4 bytes: the host listens and accepts, guests connect
  (0x08907608). It then opens **one PDP socket** with a 0x900-byte buffer for the battle's pads (0x0890C404). The four
  ports come from the game's port allocator (100 and up, 0x0890E9BC); both sides allocate the same numbers, so
  normally 100 to 103 (I). Nothing tells the other side which ports were chosen.
- **Pre-battle data** goes over PTP as framed messages (main 0x08904C08..0x089050F8, a 0x4000-byte ring): a 12-byte
  header `{"AMSG", type, length}` and a body, sent to every peer. Type 4 is a 2-byte player slot (menu 0x08A70270);
  types 0x13 to 0x18 are not identified yet (I: characters, rules, stage).
- A barrier before and after a fight: each guest sends the host a 4-byte `0x12345` message on the 4-byte channel
  (0x08907300). Results screens, rematches and Group Battle's next fight use it too.

### 4.2 The per-frame exchange

- **Start** (0x08907000, from the battle's set-up at 0x0888DD18): the two fighters' lobby members become player 0 and
  player 1. Both raw pads switch to injected mode (0x0890C640, 0x0890C784). A PSP that is one of them is role 0, with
  `net+0x44` = its player index; any other is a spectator (role 1).
- **Each frame** (frame pump 0x088EC010 -> 0x0890E794 -> 0x08907F14 -> 0x0890C5BC -> role 0 handler 0x0890D664):
  1. If the local sample may run ahead (the gate at 0x0890D6C8 allows up to 2 frames ahead of the frame being
     applied), read the pad (`sceCtrlReadBufferPositive` at 0x0890D6EC), pack it into 16 bits (0x0890CE58), and send
     a 0x24-byte packet to the other player. Otherwise resend the last packet.
  2. Drain the PDP socket: keep the other player's latest packet, spectator acks, and quit notices.
  3. If both players' words for the current frame are in, unpack them into `raw[0]` and `raw[1]` (+0x40..+0x4F,
     loop 0x0890DE30) and return 0. The pad read that follows (0x08872F08) feeds them to the battle on the same
     frame. If either is missing, return 1.
- **The packet** (0x24 bytes): checksum, the last **8** pad words (newest first), the frame of the newest, a desync
  hash and its frame, the word count. No acks; the 8-word history covers lost packets. Sent unicast to each peer.
- **The word**: bits 0-11 the buttons (left, right, up, down, circle, cross, square, triangle, L, R, START, SELECT),
  bits 12-15 the left stick as four direction bits after the game's dead zone. Unpacking writes the stick as 0x00,
  0x80 or 0xFF and leaves the six spare bytes of each sample with stack garbage.
- **Input delay**: 0 to 2 frames. The lead grows on each frame that waits and never shrinks, so it settles at 2
  after the first hiccup. Beyond a round trip of about 3 game frames (~100 ms at 30 Hz) the battle stalls every
  frame. No setting changes it.
- **Waiting**: return 1 freezes the battle (pause kind 0x10, battle task groups 0 to 2 off, 0x08889138) while the
  screen keeps drawing. After **900** waiting frames (about 30 s) the battle ends the session.

### 4.3 Spectators (Group Battle)

Up to 8 spectators. Each player relays its pad history to one spectator every 30 frames (0x1B8 bytes), spectators
relay down a chain, and acks come back up (0x10 bytes). A player stops sampling while its spectator lags 90 frames
behind (0x0890C810). The spectator handler (0x0890DF58) never reads a pad; it simulates the battle from the relay.

### 4.4 Stick, cameras and the two machines

- The word holds raw directions, not camera-relative ones. Each fighter moves relative to **its own camera**
  (battle 0x08B36D2C -> 0x08B38A0C), and each machine updates both fighters' cameras from their own pads
  (battle 0x08B0E200; [local versus](local-versus.md) 1.1, 1.4). So each machine simulates the other player's camera
  from the input it received, and the battle is the same on both.
- The machines differ in `net+0x44`, the lobby and MAC data, and which view and HUD are drawn (I: FIELD+0x54).
- **Full analog can go in place of the word** as long as both machines inject the same bytes, as the port's local
  versus already does for player 2.

### 4.5 Checks and endings

- **Desync check** (V): every 10 frames the battle hashes both fighters' positions, some status fields and FIELD+0x60
  (callback battle 0x08B136D4, installed at battle 0x08B132A0). The newest hash rides in every packet; a mismatch sends
  quit notices and shows message box 1 on both (battle 0x08B135E4).
- **Quit**: an 8-byte notice to all. L+R+START+SELECT leaves without telling the other side (battle 0x08B13484);
  the other side times out.
- **Session lost** (adhocctl group gone): message box 0. **Ending** (0x08906F6C): close the three streams, delete the
  PDP socket, leave the group.
- Nothing else is exchanged in battle: no random seed, state, pause, EX or assist events.

## 5. Communications (pass-by exchange)

Discover object main 0x08908960, menu 0x08A28B74..0x08A292A8. V for calls, I for meanings.

- `sceNetAdhocDiscoverInitStart` with group `DISCOVER` (menu string 0x08A294C0), then `DiscoverUpdate` and
  `DiscoverGetStatus` each frame until it finishes, then `DiscoverTerm`.
- Exchange: one PDP socket on port 1000 with a 0x6B18-byte buffer. Every second, send the 0x6B18-byte (27,416 bytes)
  payload (own MAC, card, a 0x123C and a 0x6B0C block: I, Friend Card and original quest) to the next peer from
  `GetPeerList`, round-robin, up to 16 peers; receive by sender MAC; drop peers unseen for 20 s.
- Online, this becomes "exchange cards and quests with whoever is in the same room", or a small card server. It
  needs datagrams of 27 KB delivered whole. Lowest priority of the three.

## 6. What a replacement network layer must provide

Per PSP function, as the game uses it (from the lobby report's spec; addresses above).

| Function | The game's use | Must |
| --- | --- | --- |
| `sceNetGetLocalEtherAddr`, `sceWlanGetEtherAddr` | Own MAC; the lower MAC hosts | Same stable, unique 6 bytes from both, per player |
| `sceNetAdhocctlInit`, `AddHandler`, `DelHandler`, `Term` | Product `{0, "ULUS10566"}`; one handler | Handler events (event, error): 0 ERROR, 1 CONNECT, 2 DISCONNECT, 3 SCAN |
| `sceNetAdhocctlConnect` | `LOBBY<n>`, `MLOBBY`, `DISCOVER` | Join or create the group; one CONNECT event later |
| `Create`, `Scan` + `GetScanInfo`, `Join`, `Disconnect` | The 8-digit battle group | Exactly one CONNECT, SCAN or DISCONNECT event (or ERROR) per call; the game does not check which kind |
| `sceNetAdhocctlGetPeerInfo` | Every known peer, every frame | Fail when gone; else the MAC at +0x84 and a **fresh** last-seen time at +0x90 (peers older than 20 s in a lobby, 60 s in a session are dropped) |
| `sceNetAdhocctlGetPeerList` | Communications | Up to 16 peers in the group |
| `sceNetAdhocMatching*` (all 10) | P2P with 2 peers (Online Lobby), parent and children up to 10 (Group Battle), hello 5 s, keepalive 10 s, retry 2 s | HELLO events with the peer's hello data, REQUEST with the requester's data, ACCEPT then ESTABLISHED on both sides, DENY, CANCEL, LEAVE, TIMEOUT, BYE; `SendData` with DATA at the peer and DATA_ACK back |
| `sceNetAdhocPtp*` | 3 streams per session, host listens | Reliable in-order, non-blocking; partial sends and receives counted; 0x80410709 (would block) when there is nothing; 0x80410718 if refused |
| `sceNetAdhocPdp*` | The battle's pads (replaced, 4.2), Communications | Whole datagrams up to 0x6B18 bytes; non-blocking receive with the sender's MAC |
| `sceNetAdhocDiscover*` | Communications | Running, then finished with "peer found" or "none" |

**What PortableKit already has** ([online play](online-play.md) 2.1): adhocctl, PDP and PTP over a TCP relay with
its own server. Missing: all of `sceNetAdhocMatching`, `sceNetAdhocctlGetPeerInfo`, and `sceNetAdhocDiscover*`.
Its server listens on every interface, which brings up the Windows Firewall prompt. Section 10 adds the first two and
a loopback-only server.

## 7. What this means for the port

- **The battle**: replace `0x0890D664` with the port's lockstep (`port/host/d012_net.cpp`). It must:
  - block for the other player's input itself and **never return 1**, so the game's pause and its 900-frame count
    never run;
  - write both players' full 16-byte samples into `raw[0]` and `raw[1]` (+0x40), the same on both machines;
  - take the port's per-frame input record here, because in comm mode the normal pad read never calls
    `sceCtrlReadBufferPositive`;
  - keep the game's desync hash, or use the port's RAM hash on the battle's state and set the quit flag on a
    mismatch so the game's own error box appears;
  - apply each player's right stick to that player's camera on both machines (the port's right-stick camera works
    on logical pad 0 only today).
  - Spectators: either replace `0x0890DF58` the same way or relay the port's records to them.
- **The lobby and setup**: provide the PSP functions in section 6 over a modern connection. Their traffic is small and
  can wait a few hundred milliseconds, so a relay or a room server is fine for it; only the battle needs the fast path.
- **The start state** is the open risk. The port's determinism was proven from boot with the same save. Here each
  machine starts from its own save and menus, and the game trusts the setup messages to make the battle identical.
  The first trace should compare the random generator and both fighters' set-up at `0x08907000` on both machines.

## 8. Next steps

1. ~~**Run two copies through the Online Lobby on one PC**~~: done, section 10.
2. ~~**Swap the battle exchange**~~: done, section 11.
3. Then the connection itself: room codes, hole punching and a relay fallback ([online play](online-play.md) 4).
4. Group Battle with spectators, then Communications.

## 9. Not settled statically (for the first runtime trace)

1. ~~The lobby number in `LOBBY%d`~~: from 0; Cornelia (lobby 1) is `LOBBY0` (section 10).
2. Group Battle: the room's group name after it fills, and who creates it.
3. The pre-battle message types 0x13 to 0x18 (log type, length and the first bytes of each).
4. Which stream carries the pre-battle messages (the second, 0x400 buffer: section 10); what the barrier states
   exchange.
5. The error codes 0x80410805 and 0x80410803 seen offline: log every network call's return value and every
   adhocctl event.
6. Per frame in battle: the consume and sample frames, the handler's return, and which task groups run while paused.
7. The random generator's state and the fighters' set-up at `0x08907000` on both machines (is the start identical?).
8. ~~The battle's PDP port~~ (104 in the run, section 10); the texts of message boxes 0 and 1.

## 10. Two copies through the Online Lobby on one PC (31 October 2026)

Two copies of the port, each with its own copy of the same save, went from the Communication Mode menu through Online
Lobby 1, a challenge, fighter select and the host's Enter Battle, into a 1-on-1 online battle. The battle ran on the
game's own netcode over PortableKit's relay. Screenshots: `build/port/online/online_lobby_flow.png`; runs
`build/port/online/lob1` to `lob10` (logs traced with `D012_TRACE_ADHOC=1`).

**Set-up.** PortableKit's ad hoc server on 127.0.0.1 alone (`D012Native --adhoc-server 27412 --loopback`; nothing
listens on another address, so Windows asked nothing), and each copy with `D012_ADHOC=1`,
`D012_ADHOC_SERVER=127.0.0.1:27412`, its own `D012_ADHOC_MAC` (`02:00:00:00:00:01` and `:02`) and nickname.

**What was added to PortableKit** (branch `adhoc-matching-012` of `tools/local/port/PortableKit-versus`, the
checkout the online build uses; not committed):
- `host/adhoc/matching.{hpp,cpp}`: `sceNetAdhocMatching` as a protocol engine of its own over a datagram socket on
  the matching port: hellos, requests, accept, cancel and deny, leave, bye, keepalives and timeouts, and data with
  acknowledgements, timed by the intervals the game passes to Create. `hle_adhoc.cpp` binds the 10 functions the game
  imports (plus `CancelTargetWithOpt` and `AbortSendData`) and calls the game's handler with the event, the peer's MAC
  and its data in a block of guest memory.
- `sceNetAdhocctlGetPeerInfo`: the peer's entry with a last-seen time of now while it is in the group, failing once it
  has left.
- `PtpOpen` fails with "port in use" when a stream on that port to that peer is already open.
- `PtpSend` and `PtpRecv` write 0 to the length when nothing was sent or received.
- `--adhoc-server [port] --loopback`.

**What the runs showed (V = seen in the traces).**
- Lobby 1 (Cornelia) is `sceNetAdhocctlConnect("LOBBY0")`: the lobby number counts from 0. V
- Hellos are 0x1258 bytes every 5 s; each copy listed the other's Friend Card as Online. V
- Challenge: A `SelectTarget(B, 0x1258 bytes)`; B gets REQUEST and its screen asks "Accept challenge and begin online
  match?"; YES is `SelectTarget(A, nothing)`. V
- **The challenger leaves at once.** A gets ACCEPT and ESTABLISHED, and within the same frame stops matching and
  leaves the lobby. A first version that gave the accepter its ESTABLISHED only on a reply from A never delivered it:
  A was gone. The accepter now gets ESTABLISHED when it accepts. V
- A (the lower MAC) created the 8-digit group and B scanned and joined it, as section 4.1 says. Ports: three streams
  on **101, 102 and 103** and the pad datagrams on **104** (the matching object still held 100 when they were
  allocated). V
- **The game reopens a stream every frame until its connect succeeds** and counts on the second open failing. Without
  the "port in use" check each retry made a new stream. V (main 0x089063A0, 0x089064B0)
- The pre-battle messages ("AMSG") travel on the second stream (port 102, buffer 0x400), and one is 3,744 bytes, so
  sends are partial. **The game's send pump adds the length field to its sent count even when PtpSend says it would
  block** (main 0x08905A48). PortableKit left the asked length there, so the game thought the rest of that message had
  gone and the guest waited for it forever on a white screen. With the length zeroed, both reached fighter select. V
- In battle each copy sent 0x24-byte pad datagrams at about 28 a second (30 is full speed) and drew its own player's
  view; player 1's stick moved its fighter; the game's desync check raised no error in about a minute of battle. V
- Both HUDs label both fighters "COM". I: the game's label for a communication battle; not checked against a PSP.

**Not done or not verified.**
- The battle used the game's netcode (section 4.2) over the TCP relay; section 11 replaces it.
- Whether the two copies' battles stay identical was not compared beyond the game's own check (section 9, item 7).
- Group Battle, Communications, rematch and results screens, leaving mid-battle, a lost connection.
- The PortableKit changes are not committed, not in a patch under `port/portablekit-patches`, and have no unit test.

## 11. The battle on the port's lockstep (1 November 2026)

The game's per-frame exchange (`0x0890D664`, section 4.2) is replaced by the port's own link
(`port/host/d012_comm_battle.cpp`), as section 7 planned. Everything before and after the battle stays the game's
over PortableKit's ad hoc layer. Runs `build/port/online/bat1` and `bat2`, screenshots `bat1.png` and `bat2.png`.

**How it works.**
- The override takes the handler's calls while a battle runs (session open and running, no quit, role 0) and leaves
  every other call, and a spectator's handler, to the game.
- At a battle's first frame it links the two fighters' copies over UDP (`net::battle` in `d012_net.cpp`, the same
  lockstep as [online play](online-play.md) section 12): player 0's copy listens, player 1's joins. Each battle's
  link has its own tag (from the battle group's name and how many battles the copy has begun in it), so packets left
  over from an earlier battle are ignored.
- Each frame it samples this copy's pad without waiting (PortableKit's new `hle_extension::first_pad_now()`; the
  game's own read may wait for a vblank, which a native override cannot), trades it for the other copy's, and
  writes both players' whole pads into the raw pads' injected samples: buttons, the left stick at full resolution,
  and the right stick in bytes 10 and 11. R3 is sent as SELECT, the camera's recentre.
- It never returns 1, so the game's own pause and its 900-frame timeout never run. Input delay is the link's
  (`D012_NET_DELAY`, 3 frames by default).
- Every tenth frame it runs the game's own check value (`0x0890CD90`) and compares it with the other copy's over the
  link. A difference, or the other copy unheard for 10 s, ends the battle with the game's own error (quit flag,
  return 3).
- **Cameras**: both copies turn both fighters' cameras, each from its own player's injected right stick
  (`d012_camera.cpp`), with the game's per-player invert options and the default dead zone and speed. This copy's
  own camera settings are left out, since the other copy cannot know them.
- Switches: `D012_NET_BATTLE=PORT` or `ADDRESS:PORT` turns it on (both copies are given it until the lobby can
  hand the address over); `D012_NO_NET_BATTLE=1` keeps the game's exchange.

**Results (V).**
- `bat1`: both copies linked at the battle's start (3 frames of input delay). Player 1 ran and attacked, and player
  2 turned its camera with the right stick and then moved. Each copy shows the same fight from its own player's
  camera, and the check values agreed every 10 frames through frame 1,930.
- `bat2`: both players pressed random moves, jumps, attacks, dashes, guards and camera turns throughout (scratchpad
  `fight_route.py`). The battle ran about 8,400 frames: Brave and HP changed and matched on both copies, with EX Mode
  and a Break. The check values agreed every 10 frames through frame 8,330. The battle ended on its own, and both
  copies went back to the Online Battle screen.
- No wait of 100 ms or more on either copy (one PC, loopback).

**Not verified.**
- That the check value covers enough: it is the game's own (positions, a few status fields, FIELD+0x60). The copies'
  full RAM differs by design (MACs, lobby data, the drawn view), so the port's whole-RAM check does not apply here.
- A rematch, quitting, a lost copy, different saves, lag and loss: section 12.
- Different camera settings on the two copies (each copy's own are left out of the battle, as above).
- Any real network or two machines. The battle link still needs its address given by hand.
- Group Battle spectators (role 1 still uses the game's relay, which no longer gets the fighters' pads).

## 12. Robustness on one PC (1 November 2026)

Two copies on this PC, each test a full run from the title screen (`build/port/online/`, logs with ad hoc tracing
off unless named). Every run below stayed in step: the game's check value agreed every 10 frames to the end, and no
run reported a mismatch.

**Additions.**
- A quit notice on the battle link (BYE). The port sends it when the game takes this copy out of a battle (main
  `0x089072AC`, overridden), after resending its last frames, so that a battle that ends normally always ends
  normally on the other copy too. A copy that hears it, and then lacks a frame for a second, ends its battle with
  the game's error instead of waiting 10 s.
- `D012_NET_SIM=DELAY_MS,JITTER_MS,LOSS_PERCENT` holds or drops every packet a copy sends (a test switch; a thread of
  its own sends them when due). Both copies set it, so the round trip is twice DELAY_MS.
- The link reports every 900 frames how long they took and how long it waited.
- `comm_battle::active()` now asks the game whether its battle is still running, so the right-stick camera goes back
  to normal when the game ends a battle without the port's hook seeing it.

**Results (V).**

| Test | Run | What happened |
| --- | --- | --- |
| Pause | `quit2` | START opens PAUSED (Return to Game, Forfeit) on that copy alone. The battle does not stop: player 2 moved while player 1 was paused and the copies stayed in step. |
| Forfeit | `quit3` | Forfeit, YES: DEFEATED on player 1, VICTORY on player 2, both copies leave the battle on the same frame (2,270) and go back to the Online Battle screen. The forfeit travels with the inputs. |
| Rematch | `rematch1` | After a forfeit both press Enter Battle; battle 2 opens a link with a new tag on both copies, stays in step, and player 2's forfeit ends it on the same frame (1,970) on both. |
| A copy vanishes | `vanish1` | Player 2's program closed mid-battle. The lobby server saw it gone at once, and player 1's game showed its own "Communications were disconnected. Returning to the lobby." The link's 10 s timeout was not needed. |
| Different saves | `diffsave1` | Player 2 on a copy of the owner's port save (`data-versus-user`): its Lightning is Lv5 with 1,242 HP against player 1's Lv1 with 1,000. Each copy shows the other's build, Brave and HP match, in step through frame 4,490, 900 frames in 30.0 to 30.5 s. |
| The L+R+START+SELECT combo | `quit1` | Opened the pause menu instead; not looked into further, since Forfeit is the way out the game offers. |

**Lag and loss** (`lag1` to `lag8`; random busy input from both players; 900 battle frames take 30 s at full
speed):

| Round trip | Jitter | Loss | Input delay | 900 frames took | Longest wait |
| --- | --- | --- | --- | --- | --- |
| 60 ms | ±10 ms | 1% | 3 frames | (not timed; waits under 3 ms in all) | 1 ms |
| 120 ms | ±20 ms | 2% | 3 frames | 30.0 to 30.5 s | 26 ms |
| 200 ms | ±30 ms | 3% | 3 frames | 30.1 to 31.3 s | 523 ms |
| 200 ms | ±30 ms | 3% | 6 frames | 30.0 to 30.6 s | 87 ms |

- Up to about 120 ms of round trip, 3 frames (100 ms) of input delay plays at full speed. At 200 ms it falls about
  3% behind with hitches of up to half a second, and 6 frames fixes that. So the delay should follow the measured
  round trip: about round trip / 33 ms, plus a frame or two for jitter.
- A copy's reported waiting can be large with no slowdown at all: a copy running a fraction of a frame ahead of the
  other waits a little every frame, in place of its own wait for the next vblank. The time per 900 frames is the
  measure that matters.
- The lag runs simulate the battle link only; the lobby still runs over PortableKit's relay on loopback.

**Seen, not fixed.**
- Opening the pause menu once stalled the pausing copy for up to 2 s (`quit1`, `rematch1`), and the other copy waited
  with it. Not seen in `quit2` or `quit3`. I: a disc read for the menu on this PC's slow disk.
- While the port waits for the other copy, the whole game waits, so the window does not redraw. Short waits do not
  show; a 10 s wait for a lost copy would look like a frozen window.
- One run (`lag7`) never reached the battle: the scripted challenge missed player 2's window. A test-script timing
  slip, not the game or the link.

**Not verified.** Two machines, a real internet path, Windows against the Steam Deck, the lobby over a real
network, different camera settings, and Group Battle.

## 13. This PC against the Steam Deck (1 November 2026)

This PC (Windows 11) against the Steam Deck (Proton), each a release build of the same program. The PC runs the
lobby server and listens for the battle link; the owner forwarded TCP 27312-27313 and UDP 27420 to it. Each copy keeps
its settings in its own `settings.ini` (`network.server`, `network.mac`, `online.battle_link`, `online.input_delay`),
so the Deck needs nothing typed into its launch options.

**The Deck's package** needs `libc++.dll` and `libunwind.dll` beside the program (llvm-mingw's runtime); without them
it closes at once under Proton.

**The first LAN battle went out of step** at frame 3,350. On one PC two copies stayed in step through everything the
two machines could differ in: clocks started 90 s apart (`DELAY1`), the PC's settings against the Deck's, one copy on
a single CPU core, one with no audio. Every clock read during a battle feeds only frame pacing and the lobby's "is the
peer there" check, and network battles run at a fixed step of 2 vblanks. What was left was the C runtime: the game's
VFPU code (`vsin`, `vcos`, `vrot`, `vasin`, `vlog2`, `vexp2`) reached `sinf`, `cosf`, `asinf`, `log2f` and `exp2f` in
Microsoft's UCRT on Windows and in Wine's under Proton.

**The fix**: `port/host/d012_math.cpp` defines those five functions in the program itself, in double precision with
nothing but IEEE additions, multiplications, divisions and square roots, so they give the same bits on every machine,
and the program no longer imports them. The boot regression and a full battle on one PC were unchanged in play.

**The check** (`port/host/d012_math_check.cpp`): a copy set up for online play prints a fingerprint of the port's
five functions and of the C runtime's; `online.math_check=full` (or `D012_MATH_CHECK=full`) sweeps every input of the
C runtime's (V):

| | sin | cos | asin | log2 | exp2 |
| --- | --- | --- | --- | --- | --- |
| The port's, PC and Deck | `e4edcb12` | `72892450` | `c58563e6` | `7dfed707` | `dbb76461` |
| UCRT (Windows), every input | `03df4106` | `7529f0cf` | `9d3ec99f` | `0fa7f818` | `c0ace263` |
| Wine (Deck), every input | `01ec3dee` | `7c584f61` | `fbef3816` | `0fa7f818` | `c0ace263` |

Wine's `sinf`, `cosf` and `asinf` differ from Microsoft's for some inputs; the sampled fingerprint (every 1,000th
input or so) had matched, which is why the first check missed it.

**Logs.** Under Proton a console program prints into a console of Wine's own, which a launch option's `> file` never
sees. A portable copy set up for online play now keeps its own log in `data/logs/D012Native.log` (the previous run's in
`D012Native-previous.log`), unbuffered; `D012_LOG_FILE=<path>` names another file.

**Results (V).**

| Path | Input delay | Battle | 900 frames took | Waiting | Round trip |
| --- | --- | --- | --- | --- | --- |
| LAN, before the fix | 3 frames | out of step at frame 3,350 | | | |
| LAN, Deck on 5 GHz Wi-Fi at -79 dBm | 2 frames | in step to its end, frame 5,398 | 30.0 to 32.1 s | PC: up to 7.9 s per 900 frames, in runs of 180 ms waits | 23 to 45 ms average, up to 194 ms |
| Internet: Deck on a phone hotspot (mobile data), PC behind the forwarded ports | 3 frames | in step to its end, frame 5,724 | 29.9 to 30.0 s | PC: 0.28 s in all; Deck: none | 100 to 103 ms average, 63 to 179 ms |

- Over the internet the battle played at full speed with no stalls the owner noticed. 3 frames suits a round trip of
  about 100 ms, as section 12 predicted.
- The Deck kept full speed through the whole internet battle, so its frame rate is not what stalled the LAN one. I:
  the Deck's weak home Wi-Fi signal, whose latency spikes came and went (a 5 s ping burst afterwards was clean at
  2 to 8 ms). The Deck's own log of that battle was lost to the console problem above.

**Not verified.** A player behind a router with no forwarded ports (roadmap step 4), Windows against Windows on two
machines, Group Battle.
