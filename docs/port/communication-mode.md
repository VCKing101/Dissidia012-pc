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

## 14. The internet with nothing set up (1 November 2026)

Roadmap step 4: a player joins over the internet with no port forwarding, no server and no battle-link setting. The
owner chose to start without a server of the project's own. So the hosting copy opens its own router's ports, its
player gives the address it shows to the other, and the input delay sets itself.

**Additions.**
- **Router ports** (PortableKit patch 0071). Hosting a session asks the router to forward the session's ports. It
  tries NAT-PMP (RFC 6886) first, then UPnP (SSDP search, the gateway's description, `AddPortMapping`). The ports
  are TCP 27312 and 27313 for the lobby and its relay, and the port's UDP 27420 for the battle link, which
  `comm_battle::install()` adds with `adhoc_host_forward_also()`. Each keeps its number outside. The lease is an
  hour, renewed at half time, and given back when hosting stops or the program closes. The log says
  `[adhoc] router: NAT-PMP opened TCP 27312, 27313 and UDP 27420 on <address>`.
- **The Network page.** Under Play together, while hosting, a row gives the internet address the router reported,
  marked with the method, and copies it. If the router refused, the row says why. "Open router ports"
  (`network.router_ports`, on by default) turns the whole thing off; `D012_NO_ROUTER_PORTS=1` does for one run.
- **`D012_HOST_SESSION=1`** (patch 0072) hosts a session as the game starts, for test runs.
- **The battle address from the session.** With `online.battle_link` empty and ad hoc play on, the copy that hosts
  the session (or whose server is on this machine) listens on UDP 27420 on every network, and the other joins the
  session server's address at that port (`online.battle_port` changes it). The fighter each copy controls now comes
  from the game's own player number, not from which copy listens, so the host may be either player.
- **The input delay sets itself.** With `online.input_delay` empty or `auto` (the default), the listening copy times
  up to 8 round trips (PROBE and PROBE ECHO, every 40 ms, for up to 2 s) before its WELCOME. It takes the median:
  delay = ceil((round trip / 2 + 33 ms + 15 ms) / 33 ms), from 2 to 10 frames. The 33 ms is the frame an input can
  wait before it is sent, since INPUT goes once a frame; the 15 ms is jitter. That gives 2 frames on a home network
  and 3 at 75 to 100 ms. With no echo it uses 3. A number in `online.input_delay` still fixes it. The link's
  version is now 4.
- **A portable copy's log** goes to `data/logs/D012Native.log` whenever ad hoc play is on, not only with a battle
  link set.

**The owner's router (V).** NAT-PMP answered, with the right internet address. PCP did not (it answered as version
0). UPnP answered too, a MiniUPnPd, but only the Steam Deck saw its SSDP replies: Windows' firewall drops them for a
program it has no rule for. The game has a rule once Windows has asked about it, so this matters only for UPnP on
Windows the first time. The router's own mapping list (read over UPnP) showed the three ports pointed at the PC
while it hosted.

**Results (V).**

| Run | Setup | What happened |
| --- | --- | --- |
| `auto0` | One PC; battle link given; delay auto | Timed 0 to 1 ms, chose 2 frames; in step to the end |
| `auto1` | One PC; copy 1 hosts in the game (router ports off), copy 2 joins its LAN address; no battle link | Copy 1 listened as player 0; in step to the end, both left on frame 6,020 |
| `auto2` | As `auto1`, with the game's player 1 hosting | Player 1 listened, player 0 joined; in step to the run's time limit (frame 6,410) |
| Internet, first try | PC hosting with the router's forwards off; Deck on a phone hotspot | The lobby worked; the battle never linked. The Deck still had the old program: the deploy had gone to a local folder. Without a battle-link setting the old program never opens the port's link, so the PC waited and the Deck ended with the game's "Communications were disconnected". |
| Internet | As above, the Deck with the new program, nothing set on either | NAT-PMP opened the ports; the Deck joined the address the PC showed, from a mobile-data address; the battle linked with a 2-frame delay (75 ms timed, 74 to 78) and stayed in step to its end at frame 5,056 |
| `delay3rt` | One PC, both copies holding packets 37 ± 4 ms (`D012_NET_SIM`), the new formula | Timed 74 ms (68 to 81), chose 3 frames; in step to the run's time limit |

The internet battle, 900 frames at a time (30 s at full speed), on the PC:

| Frames | Took | Waited | Round trip |
| --- | --- | --- | --- |
| 0 to 899 | 31.6 s | on 892 frames, 6.4 s in all | 154 ms average, 67 to 1,647 |
| 900 to 1,799 | 30.2 s | on 403, 3.0 s | 92 ms, 66 to 188 |
| 1,800 to 4,499 | 30.0 s each | none | 91 ms, 65 to 136 |

- **The first minute ran short of delay.** 2 frames (67 ms) did not cover 37 ms each way plus the frame an input
  waits before it goes out. That cost 1.6 s in the first 900 frames and 0.2 s in the next. The formula above adds
  that frame. In `delay3rt` it chose 3 at the same round trip, and the first 1,800 frames took 30.0 s each with no
  waits. Its later windows had one copy waiting most frames, which runs at 0 ms (`auto1`, `auto2`) show too: two
  copies sharing one PC, one falling a little behind.
- **No delay hides the phone network's spikes** of 1.6 s in the first half minute.
- The owner chose the extra frame: one more frame of input lag, always, for fewer slowdowns.

**Limits.**
- **A provider that shares one address among customers** (carrier-grade NAT, as on most mobile data) leaves the
  router with an address that is not the internet's. Mapping then succeeds and no one can connect. The other player
  hosts, or both use a VPN. PortableKit does not detect this yet: the address it shows would be in 100.64.0.0/10 or
  a private range.
- **One router.** A second NAT behind the first, a router with NAT-PMP and UPnP off, or a firewall on the host that
  blocks the program, need the fallbacks in [Playing online](play-online.md).
- The joining player still types an address: the plain internet address, not an encoded code.

**Not verified.** UPnP mapping by the game (NAT-PMP answered first here), a carrier-shared address, the Deck hosting
(its copy has the old delay formula), Windows against Windows on two machines, Linux natively, Group Battle.

## 15. Player-facing pieces (1 November 2026)

Roadmap step 5, as settled with the owner: online 1-on-1 goes into the next release, marked experimental, with the
pieces a player needs when something goes wrong. Group Battle and Communications stay as the game has them, untested.
No installer firewall rule, no join code; PC against PC on two machines and the Deck hosting are left untested and
said so.

**Additions.**
- **The same program and the same rules on both sides.** As the battle links, each copy sends a list of what must
  match (`net::battle::must_match`), and the listening copy refuses a copy whose list differs, or whose link has
  another version (REFUSE, link version 5). Both players then read what differs before the game's own disconnect
  box. The list holds:
  - the port's version: `d012::source_hash()`, a SHA-256 of the port's own source, the PortableKit patches and
    `port/CMakeLists.txt`, line endings aside (130 files), made at build time by `port/cmake/source_hash.cmake` into
    one generated file, so a change recompiles only that file;
  - mods that change the game: empty for now, since the mods folder holds music only (which may differ); a kind of
    mod that changes the game sets it;
  - the test switches that change how a fighter or a camera moves (`D012_NO_RIGHT_STICK_CAMERA`,
    `D012_NO_SMOOTH_STICK`, `D012_SMOOTH_STICK_BATTLE`, `D012_CAMERA_FOLLOW_DEADZONE`).
  Saves and official DLC may differ. The split-screen field of view does not count: it scales only the split view.
- **Each player's own camera settings.** HELLO and WELCOME carry the sender's right-stick camera settings (dead zone,
  sensitivity, inverted axes); both copies turn each fighter's camera with that player's. Before, both used the
  defaults.
- **The waiting screen** (PortableKit patch 0073, `ui::wait_over_game`). A wait for the other copy longer than half a
  second, as the battle links or in it, becomes a band over the last frame: "Waiting for the other player... N s",
  and a hint to hold back (B, or Esc) to leave, with a bar that fills over a second. The game stands still behind it
  as behind the paused menu: its sound stops and the stall watchdog is not told. A battle link waits 30 s
  (`D012_NET_WAIT`, was 300); a frame that does not come gives up after 30 s; ten silent seconds still count as the
  other copy gone. Leaving sends BYE and ends the battle with the game's error.
- **Notices.** A battle that cannot link says why, with an OK, before the game's own box: the other game differs in
  what, or did not connect (the host), or did not answer (the joiner, with the battle port to check).
- **Input delay on the Network page** (Online battles: Auto, or 2 to 10 frames), kept as `online.input_delay`, through
  PortableKit's new `ui::add_network_rows`.
- **A shared router address** (patch 0073): when the router's own outside address is private or the range providers
  share among customers (100.64.0.0/10), the Network page says opened ports cannot help and that the other player
  should host, instead of offering that address.
- `D012_EXAMPLE_ADDRESSES=1` shows documentation addresses (RFC 5737) on the Network page, for the README's picture.
  PortableKit's Network page no longer uses Monster Hunter's words: no "Online Guild Hall" in the hosting rows, and
  rooms, not halls, in the Server status and the Ad hoc play help.
- The Network page (patch 0074): Hosting counts the players online; a joining player's row reads "Joined <address>
  connects when you go online"; a server typed into Server is remembered under Join; ImGui's "conflicting ID" box
  appears only with `<PREFIX>_UI_ID_CHECK=1`.

**Results (V; two copies on this PC, copy 1 hosting, copy 2 joining its LAN address, both with nothing set).**

| Run | Setup | What happened |
| --- | --- | --- |
| `s5a` | Copy 2: camera dead zone 0.3, sensitivity 2, X inverted | Linked with 2 frames; in step to the run's time limit (frame 6,410), the right stick turning cameras on both |
| `s5b` | Copy 2: `D012_SMOOTH_STICK_BATTLE=1` | Refused at once on both: "This battle cannot start: the other player's game differs in analog walking in battle (D012_SMOOTH_STICK_BATTLE)..." with OK; then the game's own box and back to the Online Lobby |
| `s5c` | Copy 2 joins the wrong battle port | Both showed the waiting band; after 30 s copy 1 said the other game did not connect, copy 2 that it did not answer, naming the port |
| `s5d` | As `s5c`; copy 1 holds Esc for 1.5 s | The bar filled and copy 1 left after a second, back to the lobby list |
| Captures | Hosting with example addresses | The Network page with 192.0.2.10 and 203.0.113.7 (NAT-PMP), and the Online battles row with Input delay Auto |

**Not verified.** A router that reports a shared address, the band and notices on the Deck, PC against PC on two
machines, the Deck hosting, Group Battle.

## 16. Windows against Linux (7 November 2026)

A Windows copy and a native Linux copy now battle each other. Before, the must-match list (section 15) refused every
such pair on "the compiler the port was built with", for two reasons. The Windows build is clang (llvm-mingw 23.1.2).
The Linux program is built in the Steam Runtime SDK with **GCC 14**, which set no compiler item at all, while its
game code (`game-code/main.so`, compiled by the Linux setup) is clang 23.1.4. What the item guards is the game's
maths, and `d012_math.cpp` gives clang and GCC builds alike the port's own `sinf`, `cosf`, `asinf`, `log2f` and
`exp2f` (it is left out only under MSVC). So the item is now "clang or GCC, with the port's own maths" on both, and
an MSVC build's version for MSVC. The refusal no longer says that Windows and Linux cannot battle.

**Without the network (V).** One recorded run (boot, menus, a save, a versus battle with a Bravery transfer)
replayed on the Windows build and on the Linux build in WSL (`D012_INPUT_REPLAY`, `D012_SYNC_TRACE`,
`D012_NO_BUSY_CLOCK=1`, `D012_SYNC_EPOCH`, the same data with its file times):
- With the same font file (`text.font`): only the decoded music differs, by one step in some samples (the two
  FFmpeg builds); the game reads none of it back.
- With each system's default font (MS Gothic, Noto Sans CJK JP): text layout and some heap block sizes differ too,
  and stop growing early in the battle. The fighters' records were the same at every point compared.

**Online, on one PC (V).** The Windows build against Linux copies in WSL, each copy with its own address (WSL's
network is a separate one behind this PC), through the Online Lobby with the routes of section 11. Router ports off.

| Run | Hosting | Linux copy | What happened |
| --- | --- | --- | --- |
| `x1` | Linux | the GCC build | Lobby, challenge, streams and battle link worked; refused on the compiler item (the old list) |
| `x2` | Linux | the GCC build | Linked with 2 frames; in step to the run's time limit (frame 6,410) |
| `x3` | Windows | the GCC build | The same |
| `x4` | Linux | installed by the Linux setup (game code clang 23.1.4) | The same |
| `x5` | Windows | installed by the Linux setup | Longer routes: the battle ran to its end; both left on frame 15,805 and saved |
| `x6` | Linux | the GCC build | This change, nothing else altered: linked with 2 frames; in step to the time limit (frame 6,410) |

The copies used different fonts throughout (MS Gothic and the setup's Noto Sans CJK JP). In `x2` to `x5` the
compiler item was made the same by hand; `x6` is the change above.

**Not verified.** Two machines (WSL shares this PC), the Windows setup's copy against Linux (the Windows build here
compiles like setup.exe, with llvm-mingw), the Steam Deck on the native Linux build, Group Battle.

## 17. The frames in balance, and Communications (7 November 2026)

Two pieces from [what comes next](online-next.md): the battle link keeps the two copies' frames in step with each
other, which saves a frame of input delay, and Communications (section 5) works over PortableKit's server.

### 17.1 The frames in balance

**Why a frame was spent.** Each copy holds its own frames to its own clock, and drops what it owes when it falls
more than 100 ms behind (PortableKit `Kernel::pace_to_real_time`). So where the two copies' frames fall against each
other is set by when each started and by every stall since, not by the link. A copy's slack on a frame is how long
before it needed the other's input for that frame the input came. The two slacks add up to twice (delay - one way): 29 + 30 ms at 2 frames and 37 ms each way,
but nothing shared them out: one copy usually had nearly all of it and the other none, and the one with none
waited on almost every frame. The auto delay paid for that with a frame (section 14's "+ 33 ms").

**What changed.**
- The link reads its socket on a thread of its own and stamps each datagram with when it came, so a copy knows the
  slack it had on every frame. (The game's thread reads only once a frame, which hid when an input arrived; the
  round-trip figures were a frame high for the same reason, and are now taken from the arrival too.)
- Each INPUT carries the sender's average slack over its latest 16 frames (link version 6). The copy with less,
  by more than 2 ms, holds its game back by a sixty-fourth of the difference each frame, at most 1 ms
  (`hle_extension::hold_back`, PortableKit patch 0089, which moves the kernel's hold to real time). The two meet in
  the middle within a second or two, and stay there unless a stall moves one.
- They start in balance: the listening copy waits half the timed round trip after its WELCOME, the time the WELCOME
  takes to reach the other copy, which starts its frames when it comes.
- The auto delay drops the frame: ceil((round trip / 2 + 15 ms) / 33 ms), from 2 to 10. That is 2 frames up to a
  round trip of about 100 ms, where it was 3 from 75 ms.
- Every 900 frames the link reports both slacks and how long this copy was held back.
- `D012_NO_NET_BALANCE=1` turns it off (and puts the frame back into the auto delay).

**Results (V).** Two copies on this PC through the Online Lobby with the routes of section 12 (random busy input
from both players), the battle link through `D012_NET_SIM`. Per 900 frames (30 s at full speed):

| Run | Path (each way) | Balance | Delay | Slack, the two copies | Waited, the worse copy | 900 frames took |
| --- | --- | --- | --- | --- | --- | --- |
| `bal1` | 37 ± 4 ms | on, no start offset | 2 (auto) | 28 to 30 ms each | 0.6 s in the first 30 s, then 2 to 36 ms | 30.0 s |
| `bal2off` | 50 ± 10 ms, 1% loss | off | 2 (fixed) | about 31 and -1 ms | 2.0 to 3.5 s, waits up to 65 ms | 29.9 to 30.0 s |
| `bal3` | 50 ± 10 ms, 1% loss | on, no start offset | 2 (auto) | 14 to 18 ms each | 1.4 s in the first 30 s, then 0.13 to 0.30 s, waits up to 47 ms | 30.0 s |
| `bal4` | 50 ± 10 ms, 1% loss | on | 2 (auto) | 14 to 19 ms each | 0.07 to 0.31 s, waits up to 47 ms | 30.0 s |
| `bal5` | 37 ± 4 ms | on | 2 (auto) | 28 to 31 ms each | 1 to 2 ms | 30.0 s |

- With the balance on, the two slacks stay within 3 ms of each other, and the copies hold themselves back by 0 to
  35 ms per 30 s. Without the start offset (`bal1`, `bal3`) the first 30 s went on settling: the copy that started
  ahead waited until it had held itself back enough.
- With it off, one copy kept nearly all the slack and the other waited on 890 of 900 frames. The battle kept its
  speed (the waits came out of the time the copy would have slept), but every frame of that copy came late by up to
  65 ms, and any spike in the other copy's direction stalled it.
- Every check value agreed in every run. Windows of 30.5 to 31 s are the routes' screenshots (both copies stall);
  the longest waits of 430 to 510 ms come from them too.
- The round trip read 74 and 100 ms in `bal4` and `bal5`, as simulated; `bal1` to `bal3` read it a frame high
  (the fix above came after them).
- One `bal2` run (the same as `bal3`) lost 3.3 s in its first 30 s on both copies at once, with neither waiting on
  the link and 49 ms held back; its rerun `bal3` did not. I: this PC (its slow disk); not looked into further.

**Not verified.** Two machines, a real path's jitter, the Deck, a delay of 2 over the internet at about 100 ms.

### 17.2 Communications

**What changed** (PortableKit patch 0090).
- `sceNetAdhocDiscoverInitStart` joins the group its parameters name (`DISCOVER`) on the session's server, and the
  search ends with result 0 once another player is in the group. With nobody there in its time (the game gives
  20 s) it ends with 2, which makes the game search again; Stop ends it with 3. What the game does with each
  result is in main 0x089034D0 (result 0 -> 1, 1 -> 0, 2 -> 2, 3 -> 3) and 0x08908E20 (1: the exchange; 2: search
  again; 3: close; 4: an error). The group stays joined for the exchange.
- The relay forwards datagrams of up to 32 KiB (it was 10 KiB): the exchange's are 27,416 bytes. A unit test sends
  one each way.

**Results (V).** Two copies on this PC on the loopback server, routes `xchg3` to `xchg5` (Communications Mode,
Communications, Send Friend Card, the two notes, YES):
- Each copy joined `DISCOVER`, saw the other, and ended the search with result 0 within 35 ms. Both then opened
  their exchange socket on port 1000 and sent their 27,416 bytes to the other once a second; every datagram came
  whole.
- Both showed "Searching for players to exchange with... Receiving data from 1 players".
- `xchg5`, player 2 on another save (`data-versus-user`): player 1 pressed circle, "Data received: 1. Continue data
  transfer?", NO. It listed the other's card under Received Friend Card and nothing under Received Original Quest
  (player 2 has none), and its Friend Card list then had the new card marked NEW beside its own. Player 2's circle
  presses fell outside the screen's window in every run (the route's timing), so its side was seen only up to
  "Receiving data".
- Closing the exchange deleted the socket and left `DISCOVER`.

**Not verified.** Sending an Original Quest, Import a Friend Card, three or more players, two machines, and whether
the received card is kept after saving. The new unit test is one of the ad hoc tests, which need someone at the PC
(they raise the firewall prompt) and were not run.

### 17.3 Windows against Linux

The Windows build against the Linux build in WSL (the Linux copy hosting the session, so the Windows copy only
connects out; router ports off), both from this change, as in section 16.

- **A battle** (`xb2`, both copies holding packets 50 ± 10 ms with 1% loss): linked with 2 frames and stayed in step
  to the run's time limit (frame 6,300). The first try (`xb1`) was refused on "the port's version": the Windows
  program had been built before the two new patch files were added, and they are part of the fingerprint.
- **The balance does not settle there, because WSL's clock is slow.** Over a minute, WSL's monotonic clock counted
  60.0 s while Windows counted 63.6 s: 94.35% of the rate. A Linux copy in WSL paces itself to that clock, so it
  plays about 5.7% slower in real time. The Windows copy held itself back by the most the balance allows
  (about 0.8 s per 30 s) and still waited for the Linux copy on most frames, as it would have without the balance.
  The battle still kept in step; only the waiting was not shared. Two real machines' clocks differ by parts per
  million, which the balance takes up easily, so WSL cannot test this part; a real Linux machine or the Deck can.
- **Communications** (`xc1`, the Linux copy on another save): both joined `DISCOVER`, found each other and traded
  their 27,416-byte datagrams every second through the Linux copy's server. The Windows copy listed the Linux
  copy's card under Received Friend Card and then in its Friend Card list, marked NEW. The Linux copy showed
  "Receiving data from 1 players" (its circle press missed, as in the runs on one PC).

## 18. Group Battle (7 November 2026)

Steps 0, 1 and 3 of [what comes next](online-next.md) section 2.4. Three copies on this PC, each with its own copy of
the same save (so every card reads "PortableKit"), driven partly by input routes and partly live
(`D012_INPUT_LIVE`): the Group Battle menus wait on one another, and fixed frame numbers drifted between runs.

### 18.1 The game's own netcode, with a watcher (step 0)

`D012_NO_NET_BATTLE=1` on all three, the loopback server, ad hoc tracing on (runs `gb1` to `gb7`, `gl1`). V:

- **The room.** Copy 1 chose Group Battle, Create Room (Preset), Lv 20: `Connect("MLOBBY")`, matching as parent (mode
  1), 10 members, port 104, hello 0x970 bytes, `SetHelloOpt` each time the room changed. Copies 2 and 3 chose Search
  for a Room, saw "PortableKit Lv 20 1" and asked to enter (REQUEST with 240 bytes each). The host accepted both
  (`SelectTarget`, ESTABLISHED), sent each an 8-byte `SendData` and got its DATA_ACK, then all left `MLOBBY`.
- **The battle group.** The room's host created the 8-digit group (`Create("86274407")`), listened on the three
  streams (ports 100 to 102) and opened the pads' datagram socket (103); the others scanned and joined it.
- **Party Select.** Each member picks Party A or B, or Watch match (SELECT): copy 1 Party A, copy 2 Party B, copy 3
  watched. After a countdown the host picks the format (Tournament, Round-Robin 3 or 5); each fighter presses on its
  own slot, picks a character and answers YES; then both parties press Enter Battle. A slot left empty gives "A
  character has not been set. Each player must have a character set to a slot."
- **The fight.** All three showed Round 1, the opponents' info and the fight; the watcher showed the same fight
  from player 1's side, a few frames behind (the game relays the pads to it every 30 frames, 440 bytes, and it acks
  each with 16). Brave and HP matched on all three at every look, through EX Mode to the final blow, and the result
  (Tournament 1 - 0: VICTORY on Party A and the watcher, DEFEATED on Party B). Then all went back to the Tournament
  screen for the next match in the same room.
- A first reading that the fight had frozen was the two fighters standing idle once their routes ended.

### 18.2 Battles with watchers stay on the game's exchange (step 1)

Since section 19 this holds only with no relay to stream through (`online.battle_link` set).

The port's link carries two fighters' pads and nothing to watchers, so a watcher heard nothing, gave up after 30 s and
took the session down with it. Now, at a battle's first frame, if the game lists watchers (`net+0x358`) both fighters
leave that battle to the game's own exchange, as with `D012_NO_NET_BATTLE=1` (both see the same watchers, so both
choose alike). The log says "1 copy watches this battle: it runs on the game's own exchange". Such a battle plays as
on a PSP: the stick in eight directions, no right-stick camera, and stalls beyond a round trip of about 100 ms.
Battles without watchers (a room of two, and the Online Lobby) keep the port's link.

`gl2`: the same party battle as 18.1 with the port's link on: both fighters chose the game's exchange, and the watcher
followed the fight in step.

### 18.3 The session host's relay (step 3)

The copy that listened for the battle link was the one hosting the session. When the host was not one of the two
fighters, both fighters sent to an address where nothing listened, and the battle ended after 30 s with "did not
answer": a Group Battle the host sat out, and the Online Lobby whenever two players other than the host fought.

Now the copy hosting the session runs a relay on the battle port (UDP 27420, the port its router already forwards)
while it hosts: it passes each link's datagrams between the two copies that send that link's tag, at most two per
tag, and forgets an address unheard for 60 s. Both fighters send to it, at the session's address (the host's own
fighter on loopback). The game's player 0 leads the link (times the round trip, sets the delay, sends WELCOME), and
while it waits it tells the relay it is there (HERE), so the other's HELLO can reach it. Link version 7.

The input delay used to be the listening copy's setting. Now each copy sends its own (a number, or auto), and the
higher counts, auto as the round trip needs.

Results (V), the hosting copy (`D012_HOST_SESSION=1`) as the session's server, router ports off:

| Run | Who | What happened |
| --- | --- | --- |
| `rl1` | Copy 1 hosts and stays at the menu; copies 2 and 3 fight in the Online Lobby | Both linked through copy 1's relay, 2 frames; in step to the run's end (frame 5,781) |
| `rl2` | Copy 1 hosts and fights copy 2 | Copy 1 linked through its own relay on loopback; in step to the end |
| `gl3` | A Group Battle room of two, copy 1 hosting | No watchers, so the port's link, through the relay; in step to the run's end |
| `rl3` | As `rl1`, copy 3 with input delay 5, copy 2 auto | 5 frames ("this copy's setting auto, the other's 5"); in step |
| `xr1` | The Linux build in WSL hosts and fights the Windows build in the Online Lobby | The Windows copy joined through the Linux copy's relay, 2 frames; in step to the run's end (the Windows copy waited on the slower WSL clock, as in 17.3) |

**Not verified.** Any of this on two machines; the relay over the internet (the extra hop adds the path from the host
to each fighter); a party battle of more than one fight (the per-battle tag counts battles per copy, which agrees
whenever the same two copies fight every battle in a group, as they do without watchers); Round-Robin; a watcher on a
slow connection; a server run on its own (`--adhoc-server`, as on a VPS), which has no relay: its battles still need
`online.battle_link`.

### 18.4 Party battles of several fights (7 and 8 November 2026)

Three copies on this PC, the copy hosting the session as its server (`D012_HOST_SESSION=1`), driven live
(`D012_INPUT_LIVE`) through the menus with screenshots at each step, and the fighters on random or attack-spam
input. V:

| Run | Parties | Format | What happened |
| --- | --- | --- | --- |
| `gp1` | Party A: copy 1 (the room's and session's host) and copy 3; Party B: copy 2 | Tournament | Fight 1, copy 1 (Lightning) against copy 2 (Lightning), with copy 3 watching; copy 1 lost. Fight 2, copy 3 (Vaan) against copy 2, with copy 1 now watching. Copy 2 won both: 0 - 2 DEFEATED on copies 1 and 3, 2 - 0 VICTORY on copy 2. Brave and HP agreed on all three at every look |
| `gr4` | Party A: copy 2; Party B: copy 3; copy 1 (the host) watched | Round-Robin (3 Rounds), three characters a party | Rounds 1 and 2 (Lightning, then Vaan, against their counterparts) were fought to their end, and round 3 (Laguna against Laguna) began on all three copies, the watcher following each. Round 3 was stopped after 15 minutes with neither fighter hit (the attack-spam input's shots missed), so the final result was not reached; all three copies agreed at every look |

- Every fight had a watcher, so each ran on the game's own exchange (18.2); the log says so at each fight's start
  on both fighters.
- A party's members each press Enter Battle; one player's press does not start the battle. In Round-Robin each
  fighter fills one slot a round. A slot's "Random" column opens the round's stage and rules.
- A note for tests: two test benches must not run at once on one PC. A second bench's copy with the same address
  joined the first bench's session, pushed its player 2 out and broke the room (`gr2`).

## 19. Watched fights on the port's netcode: the spectator stream (8 November 2026)

Group Battle step 4 ([online next](online-next.md), section 2.4). Until now a fight others watched ran on the game's
own exchange (18.2), as on a PSP. Now it runs on the port's link like any other, and the watchers get it from the
leading fighter.

### 19.1 The game's spectator handler

Disassembled from main 0x0890DF58 (V). Each call it:

- takes one relay packet of 0x1B8 bytes (two records of 0xDC, one per player: a checksum, 100 pad words, the newest
  frame at +0xCC, a check value pair at +0xD0 and +0xD4, the frames the record covers at +0xD8), checks each record's
  sum, and pushes the new words into that player's history at `net+0x35C + player * 0xF4` (count and newest frame
  at +0xF0, room for 0x76 words, 0x0890D5B4); the check value pairs go to the player's list at `net+0x78 + player *
  0x54` (up to 4);
- passes the packet on to the next spectator down the chain (0x0890C930) or acks it upstream (0x0890CB48), and
  passes acks of 0x10 bytes from downstream on up; an 8-byte quit notice sets the quit flag and frame;
- once running, compares its own check values with the fighters' (0x0890CBD4; a difference ends the battle with
  return 3), then looks up the frame `net+0x48` in both players' histories (0x0890D638), unpacks both words into the
  raw pads' injected samples (0x0890D090, then raw pad i + 0x40), runs the check step (0x0890CD90) and advances
  `net+0x48`; with either word missing it returns 1, the game's wait.

So besides the raw pads it advances only `net+0x48` and its own check list (`net+0x1C`, which the check step keeps
on every role). The histories, the per-player check lists and the chain are its own bookkeeping, which nothing else
reads (I: no other reader was found; the battle has run on without them on the fighters since section 11).

### 19.2 What the port does

- **The tag.** Every copy counts the battles it begins in a battle group, fighting or watching, as the handler of
  either role first runs (`tag_for`). The fighters of a party battle change from fight to fight, so a count of only
  the battles fought gave two fighters different tags from the second fight on.
- **The leading fighter** (the game's player 0) keeps both players' pads of every certain frame from the battle's
  first: in the lockstep each frame as the exchange returns it; with rollback (5.8 in online next) each frame once the
  other's real input for it is in, with the inputs the frames run again used. It keeps the check values of the
  frames whose inputs are certain. A STREAM packet carries up to 36 frames (32 bytes each) from the first frame any
  watcher lacks, the 4 newest check values, both players' camera settings and a hash of the must-match list
  (d012_net.cpp's header). It goes every frame while a watcher lacks frames, else every 100 ms.
- **Watchers** send WATCH (an id and how many frames they hold) every 30 ms through the session host's relay. The
  relay keeps them apart from the two fighters (up to 16 a battle, forgotten after 10 s unheard), passes their WATCH
  to both fighters and every STREAM to them. Each frame sent once reaches each watcher at the latest when its next
  WATCH says it lacks it.
- **A watcher's frame** (the override of 0x0890DF58): both pads for the frame from the stream, injected as the
  fighters inject them; the check step every tenth frame, compared with the leader's whenever both are in; never the
  game's wait. It keeps a buffer of frames in hand, sized to its path (19.4). The wait is the fighters' screen,
  "Waiting for the fighters…"; holding B leaves.
- **The camera.** `comm_battle::active()` holds for a watcher too, and `net::battle::camera()` gives both fighters'
  camera settings from the stream, so the watcher turns each fighter's camera with that player's right stick and
  mouse exactly as the fighters do. A watcher has no local player: its own sticks do nothing in the fight.
- **The end.** As the leader's battle ends (the game's leave, 0x089072AC, or a quit), it keeps sending for up to
  1.5 s until every watcher holds the whole battle, then marks the stream ended at its last frame. A battle that ends
  by itself ends on that frame on every copy, the watcher's included. If the watcher's handler is called for a frame
  past the stream's end, the battle was left there, and the watcher's ends with the game's error too.
- **No relay.** With `online.battle_link` set (a direct link, or a server run on its own) there is nothing to stream
  through, and a watched fight stays on the game's exchange on every copy, as in 18.2.
- **What must match.** A watcher whose must-match list differs from the leader's is told "This battle cannot be
  watched" and its battle ends.
- Link version 8 (WATCH and STREAM are new).

### 19.3 Results (V)

Three copies on this PC, the build of the rollback branch, copy 1 hosting the session as its server with its relay
(`D012_HOST_SESSION=1`), driven live through the menus as in 18.4, the fighters on random fight input:

| Run | What | What happened |
| --- | --- | --- |
| `gw1` | The 2-against-1 Tournament of `gp1` (18.4): fight 1 copy 1 (Lightning, leading) against copy 2 (Lightning) with copy 3 watching; fight 2 copy 3 (Vaan, leading) against copy 2 with copy 1, the session host, watching through its own relay on loopback | Fight 1 ran 11,330 frames to its end by itself; the watcher played all of them and left at frame 11,330 with both fighters, through the game's own end. Fight 2: the tag agreed on all three (copy 3 had only watched before); copy 2 forfeited from the pause menu at frame 12,742, and all three left at 12,742. Result 1 - 1 by rounds, then VICTORY on copies 1 and 3, DEFEATED on copy 2. Every check value the watchers compared agreed, and every screenshot showed the same Brave, HP and camera on all three, the watcher a few frames behind |
| `gw2` | As `gw1`, with the refill and the hold back below, copy 1 fighting too, and copy 3 sending over a simulated path of 60 ± 20 ms each way losing 2% (`D012_NET_SIM`): in fight 1 its WATCH went that way, in fight 2 (copy 3 leading) the stream itself | Fight 1 ran 13,803 frames to its end by itself, all three leaving at 13,803. Fight 2: copy 3, the game's player 0, forfeited from the pause menu at frame 4,745, and all three left at 4,745. Result 0 - 2: DEFEATED on copies 1 and 3, VICTORY on copy 2. Over the 18,548 frames watched, 10 waits (3.6 s in all, at the fights' starts and the fighters' own stalls), 1.8 s held back, 6 or 7 frames in hand at every report, and no difference |

- In `gw1` the watcher ran at the leader's pace (900 frames in 30.0 to 31.0 s) but at the head of the stream,
  waiting a little on most frames (239 to 900 a report): its frames took less than a frame's time, and the waits
  stood in for its own pacing. On loopback that looks the same; over a real path each late packet would show. The
  refill of 3 frames and the hold back to 6 in hand came from it; in `gw2` a report had 0 to 3 waits.
- The game shows a watcher the pause menu of player 0 (not player 1's), and the menu follows player 0's inputs on
  the watcher as on player 0's own copy. It is drawn only: player 0's Forfeit ended the battle on the same frame on
  every copy, the watcher's by the game's own end. The game's own spectator handler injects the same pads, so a PSP
  spectator saw the same (I).
- The quit combo (L + R + START + SELECT) from the test input did not end the fight; the pause menu's Forfeit did.
- The pause menu does not stop an online battle: the other copies played on while copy 2's was open.

**Not verified.** Watchers on other machines; more than one watcher at once (the room had three members); a watcher
that leaves mid-fight (the session then, as on a PSP: I); rollback fighters with a watcher; the Steam Deck or Linux
as a watcher (GCC 13 compiles the changed files); a watcher whose game differs; Round-Robin with the stream.

### 19.4 A watcher's buffer sized to its path, and skipping ahead (8 November 2026)

19.2's watcher kept 6 frames in hand (0.2 s): enough on loopback, not on a path whose packets come late by more than
that, where each late packet stopped the watcher's battle. A watcher does not need to be close to live, so its
buffer now grows with the path, and a watcher left behind skips ahead instead of staying behind for the rest of the
fight.

**The constraint: sound.** The game's sound is timed by its frames: `sceAudioOutput*Blocking` returns once the
previous buffer has drained in emulated time (PortableKit `host/hle/hle_media.cpp`, V), and the sink keeps a ring of
about 186 ms ahead of the device (`host/audio/audio_sink.cpp`, V). Frames run a few percent faster fill the ring
until it drops its oldest samples on every buffer; a few percent slower empty it until it starts again with a gap
(both read from the code, I). So the watcher never changes its pace to grow or shrink its buffer. It does so with a
single stop or a single skip, each heard once, as a gap (19.5).

**What it does** (`net::battle::watch_frame`, the constants at `kWatchStart`):

- **The buffer** starts at 12 frames (0.4 s), at least 6, at most 45 (1.5 s); `D012_WATCH_BUFFER=N` fixes it at N.
- **A frame that has not come** waits, with the fighters' wait screen after half a second, until the whole buffer is
  in again: one stop, as long as the gap, instead of a short one on each frame after it. The fighters' own stall
  costs the watcher as long as it costs them, and no more.
- **Larger:** a wait the path caused makes the buffer larger by the frames the wait lasted, plus 3. The frames that
  came in the wait then are the buffer, so nothing is skipped after it. A wait is the fighters' own when a STREAM in
  it said the leader held no frame the watcher lacked; those do not change the buffer. STREAM's u32 at +12 is now
  always the frames the leader holds (not only once the battle has ended), for this.
- **Smaller:** 900 frames (30 s) with no wait the path caused, in which a quarter of the buffer (at least 4 frames)
  more than needed was always in hand, make it up to 3 frames smaller.
- **Skipping ahead:** a quarter of the buffer (at least 8 frames) more than it in hand, for 15 frames on end (not a
  moment's bunch of late packets) after a smaller buffer, or at once after a slow moment of this copy's own (more
  than 0.1 s between two frames: a window dragged, a disk stall; the kernel drops a debt of more than 0.1 s rather
  than racing to make it up, so the copy stays behind), so its sound stops once for both (19.5):
  the extra frames run unseen, as rollback runs frames again (`set_simulate_only`, patch 0092: nothing drawn or
  played, nothing waits for real time), and a skip that ends with as many again in hand goes on. After it the hold
  to real time starts again from that moment (`hle_extension::resync_real_time()`, PortableKit patch 0093), or the
  copy would wait for real time to catch up with the frames it skipped. `D012_WATCH_SKIP=0` never skips.
- **Following the fighters' pace.** The fighters' frames come a little slower at times as they keep in step with
  each other (17.1), so a watcher on its own clock creeps up on the head of the stream. With more than
  2 frames short of the buffer all through the last 90 to 180 frames (a lasting shortage, not a gap in the path,
  which the buffer is for), each frame is held back 1 ms, as little as the fighters' own hold.
- Every 900 frames the log gives the waits (and how many were the fighters' stalls), the time held back, the frames
  skipped, the buffer and the frames in hand.

Test switches: `D012_NET_SIM` takes an outage, `DELAY,JITTER,LOSS,EVERY_S,OUTAGE_MS` (every packet dropped for the
last OUTAGE_MS of every EVERY_S seconds), and `D012_NET_SIM_STREAM=1` keeps the simulated path to the STREAM packets;
`D012_WATCH_PAUSE=FRAME:MS[,...]` stops a watcher for MS at FRAME, as a slow moment of its own.

**Results (V).** Three copies on this PC, set up as 19.3, a 2-against-1 Tournament, the fighters on random fight
input:

| Run | Path | What happened |
| --- | --- | --- |
| `gw3` | Copies 1 and 3 on 120 ± 90 ms losing 6% (`D012_NET_SIM`); this run had no hold back | Fight 1 ended at frame 6,893 on all three. No wait the path caused, but the frames in hand fell by about 3 a minute as the fighters held back, and the watcher stopped about 0.25 s once a minute at the head of the stream: the hold back to follow the fighters came from it |
| `gw4` | Copy 1 on 100 ± 60 ms losing 3% with an outage of 1.5 s every 40 s; the watcher stopped 1.5 s at frame 2,000 and 0.8 s at 5,000 (`D012_WATCH_PAUSE`) | Fight 1 ended at 7,044 on all three, fight 2 (copy 1 watching through its own relay, stopped 1.2 s at 3,000) at 3,930. The outages stalled the fighters too, and were taken as the fighters' stalls: the buffer stayed. Each stop was skipped: 46, 22 and 34 frames. The buffer went from 12 to 9 after a steady 30 s. Two skips of 8 and 9 frames came from a moment's bunch of late packets: the 15 frames on end came from them |
| `gw5` | The outage on the stream only (`D012_NET_SIM_STREAM=1`), every 30 s; the buffer grew 3 frames plus at most 9 a wait | Fight 1 ended at 10,532 on all three. The buffer took three outages to reach 45, each followed by a skip; after it a smaller buffer by 3 and a skip of 8 to 9 frames, then a wait on the next outage. Growing by the whole wait and the quarter-of-the-buffer margins came from it |
| `gw6` | As `gw5`, with those | Fight 1 ended at 9,845 on all three: one wait of 1.1 s, the buffer straight to 45 with no skip, then one wait of 0.6 s in 5 minutes of outages; the stop skipped 58 frames. A second watched fight (a new Tournament) ran 56,700 frames (31 minutes, ended by the run's time limit) with no wait after the first, but a skip of 11 frames every 4 minutes: each outage held the watcher back 40 ms as if the fighters had slowed. Following the fighters only on a lasting shortage came from it |
| `gw7` | As `gw6`, with that | 33,000 frames watched (17 minutes, ended by the time limit): two waits of 0.3 s after the first (an outage and jitter beyond 1.5 s), nothing held back once the buffer was 45, no skip but the stop's (50 frames) |

Every check value compared in these runs agreed (no copy logged a difference). Starting the drift record again
when the buffer grows (the 0.3 s held back for nothing just after it in `gw7`) came after `gw7`; the runs of 19.5
have it, and held nothing back once the buffer was 45.

**Not verified.** A real path; a watcher on another machine, the Steam Deck or Linux (GCC 13 compiles the changed
files).

### 19.5 How a watcher's stops and skips sound (8 November 2026)

**How it was heard.** `D012_AUDIO_DUMP=FILE.wav` writes what the sink hands the device, before the volume setting
(the test data is muted; the dump is not). `D012_TRACE_AUDIO=1` logs, each second of it, the frames the device took
past what the game had written (`silent=`), so each `[battle-watch]` line falls in a known second of the dump. Each
run is the `gw6` set-up with the watcher stopped by `D012_WATCH_PAUSE`; the script that lines the two up and measures
each gap's edges is not in the repository (the measure: the largest change from one sample to the next at the edge,
against the music's own largest in the half second before).

**What a stop and a skip were.** The game stops writing sound for both: a stop holds the whole game, and frames run
unseen mix none (patch 0092). The ring ran dry 0.35 to 0.4 s into a stop, so a stop of 0.8 s was 0.46 s of silence;
a skip of 15 to 70 frames, 36 to 188 ms; a skip of 8, none (it ran before the ring was dry). Two faults:

- **A click at each end of every gap.** The sound went from its level to nothing between two samples and back the
  same way: a step at the edge of up to 15,376, three times the music's own largest (about 5,000).
- **Two gaps for one slow moment.** The stop was skipped 15 frames later (the look of 19.4), when the sound had come
  back: a second gap of 36 to 188 ms about a second after the first.

**What changed.**

- PortableKit patch 0094 (`host/audio/audio_sink.cpp`): the last 256 frames (6 ms) before the ring runs dry fade out,
  and a producer's first 256 after it fade in. The fade out has to go on the ring before the device takes those
  frames: it does when a hand-over leaves fewer than 256 behind. If the game then writes past them after all, the
  fade is taken off again while none of it has played (what it took off each sample is put back), or the new frames
  fade in if some has; a near miss is then a dip of 12 ms at most, never a click. A producer the sink starts again
  ahead of a ring that had not run dry (its sound fell within a quarter of the prebuffer of the device, as after a
  short skip) fades out where the ring ends too. `D012_AUDIO_NO_FADE=1` turns it off; the trace line gains `fades=`
  and `undone=`.
- `watch_frame` skips at once after a slow moment of this copy's own (19.4), inside the same gap.

**Results (V).**

| Run | What changed | Gaps in the fight (stop or skip: gap, step at the cut and at the return) |
| --- | --- | --- |
| `gw8` | Neither | Stop 0.8 s: 464 ms, 4,225 and 6,162, then its skip 0.9 s later: 36 ms, 7,370 and 15,376. Stop 1.5 s: 1,088 ms, 6,126 and 5,694, then its skip: 188 ms, 8,712 and 8,857. Skips of 22 and 15 frames: 41 and 64 ms, steps up to 6,987 |
| `gw9` | The fade, without looking ahead | Steps of 2 to 77 at every edge but one skip's cut (1,246) |
| `gw10` | And the skip at once | Stop and skip one gap: 911 ms for 0.8 s, 1,551 ms for 1.5 s. A second skip 2 s after the 1.5 s stop cut at 14,521: the hand-over before it had left a few frames |
| `gw11` | And the fade put on ahead | The stops clean (5 to 80); the same second skip cut at 13,174, with no dry ring: the sink had started that sound again ahead of the device |
| `gw12` | And the fade where the sink starts a producer again | Six stops (0.2 to 1.5 s) and five skips (11 to 44 frames): eight gaps, every edge at most 75 against the music's 3,000 to 10,000. The fade fired 31 times in the run, each before a real gap, and was never undone |

A picture of the edges and clips of each case before and after are in `build/port/online/sound-check/` (local, not
in the repository). Every check value compared in these runs agreed. Heard through headphones on 8 November: the
clips after the change sound clean.

**Not verified.** The device's own resampling after the sink (SDL); the
fade in another game; a watcher on another machine, the Steam Deck or Linux (GCC 14 compiles both changed files).
Section 19.6 has a watcher on the Steam Deck.

### 19.6 The Steam Deck hosting, watching and leading (8 November 2026)

The build of this section on the Steam Deck (native Linux, GCC 14, Wi-Fi) and this PC (llvm-mingw clang) on the same
home network; one player at the Deck, the PC's copies driven by test input. The Deck hosted the session in every run,
so its relay carried every battle. V unless marked.

**Two faults, found on the Deck.**

- **A watcher that went online before hosting saw a white screen.** Whether Communication Mode battles use the port's
  link was read once, at the game's first call of a battle handler, and kept. The Deck had tried Group Battle before
  hosting (ad hoc off: the game's switch-off error 0x80410B03), so it kept the game's own exchange. Its fighters
  streamed on the port's link; it waited for the game's exchange, which never came, until the game gave up:
  "Communications were disconnected", for the fighters too. Now `comm_battle::enabled()` is read again as each
  battle starts, and the log says which way it went each time it changes.
- **Create Room waited for ever after a failed try.** With ad hoc play on but no session hosted or joined, PortableKit
  read the PSP's wireless switch as on, so the game tried to join and PortableKit failed it with an error no PSP gives
  (0x80410B05). After the player then hosted, Create Room showed "Creating room..." for ever, with no way back. Patch
  0095: the switch reads on only with ad hoc play on and a server, hosted or joined; before that the game says the
  switch is off (0x80410B03) and starts cleanly once the player hosts.

Both were reproduced on this PC first (the Deck's own steps, three copies on loopback) and fixed there, then on the
Deck.

**Results.**

| Test | Who | Result |
| --- | --- | --- |
| 1-on-1, the Deck hosting | Deck against PC copy 1 (Online Lobby) | In step to the end (frame 4,867), 8 checks agreed; input delay 2 (auto), round trip 4 to 5 ms; one burst of 41 waits (1.6 s) when the round trip reached 177 ms |
| Watching, before the first fix | PC copies fight, the Deck watches | White screen on the Deck; all three "Communications were disconnected" at frame 845 |
| Watching | PC copies fight, the Deck watches (Tournament) | 9 minutes, all three left on frame 16,380 (a forfeit); 26 checks agreed; no skip; buffer 12, 9, then 6 frames; 18 stops, 3.8 s, each a stall of the fighters' own (161 and 268 waits between them through the Deck's relay). The player: smooth, no clicks, a delay only seen sitting next to a fighter |
| Leading with a watcher | The Deck fights PC copy 1, PC copy 2 watches | All three left on frame 4,895; 8 checks agreed on each; one 0.4 s wait on the Deck at frame 374, the watcher's one stop the same moment; buffer down to 6 |
| Create Room after a failed try, after the second fix | The Deck | The switch-off error, then Host a session, then the room made and both PC copies entered it |

**Not verified.** Rollback on the Deck; Communications between machines; a party battle of more than one fight
across machines; Round-Robin; more than one watcher at once; a path beyond one home network.
