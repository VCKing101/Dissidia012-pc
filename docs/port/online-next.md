# Online play, what comes next: Group Battle, Communications, rollback

A read-only study on main 0094507 (v0.6.0), 7 November 2026, with PortableKit at `dissidia` e5a3fe8 (patches
0001-0088). Nothing was built or run for it. V = seen in code, logs or traces at the place given; I = inferred.
PK = `tools/local/port/PortableKit`. Background: [Communication Mode](communication-mode.md) (the game's network use,
sections 3 to 5 for the rows below) and [online play](online-play.md) (the netcode research, section 9 for rollback).

## 0. The short answer

| Row | Works online today? | What it takes |
| --- | --- | --- |
| Online Lobby (1-on-1) | Yes; **since 7 November also when the host is not a fighter** (the relay, [Communication Mode](communication-mode.md) 18.3) | - |
| Group Battle | **Works since 7 November** (18); **since 8 November every fight on the port's netcode**, watchers on the leading fighter's stream ([Communication Mode](communication-mode.md) 19) | Two machines |
| Communications | **Done on 7 November** (patch 0090, [Communication Mode](communication-mode.md) 17.2) | Was: Discover a stub, datagrams capped at 10 KiB (section 3) |
| Friend Cards | Local (ghost battles); nothing to do | One check run (section 4) |
| Artifacts | Local | Nothing |

Rollback: **feasible**, with the biggest risk (the threading model) already retired by reading the code. It is a
medium-to-large job of roughly 2,500 to 4,000 lines over PortableKit and the port, and the Steam Deck's headroom for
re-running frames is the open question (section 5).

## 1. What was read

- Port: `port/host/d012_comm_battle.cpp`, `d012_net.cpp/.hpp`, `d012_camera.cpp`, `d012_sync.cpp`, `d012_input.cpp`.
- PortableKit: `host/adhoc/{matching,client,server,protocol,session}`, `host/hle/hle_adhoc.cpp`,
  `host/hle/hle_media.cpp`, `host/kernel/kernel.{hpp,cpp}`, `src/guest_memory.cpp`, `src/runtime.cpp`,
  `tools/codegen_main.cpp`, `tests/adhoc_tests.cpp`.
- Game listings from the October static work (`main_net`, `menu_all`, the comm lobby and battle notes) and the
  `build/port/online/` run logs and page trace `pinp_a.bin`.

## 2. Group Battle

### 2.1 The room: probably works, untested

- PortableKit's matching engine has parent, child and P2P modes. Create takes 2 to 16 peers (PK
  `hle_adhoc.cpp:1028`; the game asks for 10). A parent advertises while it has room, takes requests only from
  children and turns them away when full (`matching.cpp:177-193`, `:323-331`). V
- `SetHelloOpt` replaces the advert and sends it at once (`matching.cpp:72-75`). `SendData` keeps a sequence per
  peer, retransmits and acks, which fits the host's "next member on DATA_ACK" (game 0x0890BC8C, 0x0890BDD8). V
- The game imports 10 matching functions and no `GetMembers`. The room's sizes (hello 0x970, request 0xF0) fit
  under the data limit. Adverts reach the whole `MLOBBY` group, and the server has no group-size limit
  (`client.cpp:1409-1411`, `server.cpp:261-271`, `:399-403`). V
- Risk: once a child joins, it stops hearing hellos, and a full parent stops sending them (`matching.cpp:326`,
  `:341`). If the guest's room screen updates from hellos rather than from `SendData`, it goes stale. I; a trace
  settles it. PortableKit has no matching tests.

### 2.2 The fights: three things break

1. **Nobody may listen for the battle link.** The copy that listens is the one hosting the PortableKit session
   (or whose server is 127.x); everyone else joins the session server's address (`d012_net.cpp:1044-1049`, V). In
   Group Battle the session host may watch or sit a fight out; then both fighters join an address where nothing
   listens, and the battle ends after 30 s with "did not answer". **The Online Lobby has the same gap today**: with
   three copies on one server, if A hosts and B challenges C, nobody listens. Only the host has forwarded ports
   (`d012_comm_battle.cpp:429`), so two fighters who are not the host cannot reach each other directly anyway.
2. **The per-battle tag breaks in party battles.** The link's tag is the battle group's name plus how many battles
   this copy has begun as a fighter (`d012_comm_battle.cpp:107-120`, V). When A1 beats B1 and then meets B2, A1's
   count is 1 and B2's is 0: the two never link.
3. **Spectators get no inputs.** The port replaces only the fighters' handler (0x0890D664) and the leave call
   (`d012_comm_battle.cpp:366-369`, V). The game's own handler also pushed each pad word into the spectator relay
   (`net+0x380`), sent it to the spectator every 30 frames (0x0890C930), and drained the spectators' acks; the
   override does none of that (V). A spectator then waits 900 frames (30 s) and leaves. The session host checks
   every member and ends the session when one goes, and guests follow the host, so everyone drops (each step V, the
   chain I).

Feeding the game's relay from the port's link does not work: the relay carries the PSP's 16-bit word (8-way stick,
no right stick), so a spectator would simulate a different fight from the port's full-analog one, fail its own check
and leave, ending the session.

### 2.3 The two ways to fix the fights

- **The PSP's way.** When a battle session has spectators, both fighters keep the game's own exchange. That is
  about 10 lines (the game's spectator count is at `net+0x358`, read at 0x0890D810). It can be tried today with no
  code: `D012_NO_NET_BATTLE=1` on every copy. Play is PSP quality: 8-way stick, no right-stick camera, stalls above
  about 100 ms round trip.
- **The port's way.** Stream both players' full pads, camera settings and check values to every spectator, and
  replace the spectator handler (0x0890DF58) as the fighters' was replaced: block for the frame, inject both pads,
  compare the check value every 10 frames, never return 1. `comm_battle::active()`, the camera code and
  `net::battle::camera()` learn a third role. With fighters who need not be the host, the link also needs a relay
  on the hosting copy's UDP 27420 (it is already forwarded), and "who leads the protocol" separated from "whose
  socket listens". Medium-large.

The party battle's fight order, whether every member runs the battle start (0x08907000) for each fight, and the
session group's name and creator are not known yet (I). The fighters for each fight come from the team table at
`*(0x089D7F40)` (2 teams of 5) and the current fighter per team (V).

### 2.4 Order of work

| Step | What | Size | Test |
| --- | --- | --- | --- |
| 0 | No code: three copies on one PC with `D012_NO_NET_BATTLE=1` and `D012_TRACE_ADHOC=1` | none | Create a room, search, join, a fight with a spectator, the next fight, results; trace the session group, `SetHelloOpt` after joins, and the battle start on every copy |
| 1 | Keep the game's exchange when the session has spectators | small | Step 0 without the switch; a two-member Group Battle still uses the port's link |
| 2 | Count battle starts on every copy and in every role (a hook on 0x08907000) for the tag | small | A party battle where a member sits out |
| 3 | A relay on the hosting copy's 27420; the fighters need not be the host | medium (300-400 lines and a test) | Online Lobby: A hosts, B against C; PC against Deck through a third host |
| 4 | The spectator stream and the spectator handler's override; drop step 1's fallback | medium-large (600-900 lines) | Spectator check values agree every 10 frames; a lagging and a quitting spectator |
| 5 | PortableKit: listen backlog 16 (5 today, `client.cpp:874`), matching unit tests | small | `tools/port/pk_ctest.cmd` |

**Step 4, the spectator stream** (8 November; **built and tested on one PC**, [Communication Mode](communication-mode.md) 19, which also gives the handler's disassembly; the plan as written before it follows). The game's spectator handler
(0x0890DF58) is not a mirror of the fighters': it keeps a history of each player's 16-bit words per member (records
of 0xF4 bytes from `net+0x35C`, the words at +4, a count at +0xCC, a checksum over 100 words), unpacks them into
the raw pads (0x0890D090) and checks the battle (0x0890CBD4). Replacing it means: (1) the leading fighter streams
both players' full pads and the check values of frames whose inputs are certain, through the session host's relay,
to every copy that watches (a WATCH packet subscribes a copy to a battle's tag; the relay fans the stream out); (2)
an override of 0x0890DF58 that waits for a frame's pads, injects both, runs the check step every tenth frame and
compares, and never returns 1; (3) the battle's tag counted on every copy in every role, so watchers find it; (4)
`comm_battle::active()` and the camera code for the watcher's role, with both fighters' camera settings in the
stream. The fields the spectator handler advances besides the raw pads are the open question: a trace of one
watched fight's `net` before and after each handler call settles it.

Steps 0 to 2 give Group Battle as the PSP had it. Steps 3 and 4 give it the port's netcode. Step 3 also fixes the
Online Lobby gap above, so it is worth doing whichever way Group Battle goes. Every step with three copies needs
the owner present (ad hoc tests and the firewall), and three copies on one PC need `D012_NET_BATTLE=PORT`, since each
copy's server is 127.0.0.1 and every one would listen.

## 3. Communications

- **Discover is a stub** (PK `hle_adhoc.cpp:1147-1167`, V): `InitStart` reports "completed" at once, writes no
  result and joins no group. The game would then enter the exchange with no peers (I).
- **The relay refuses the game's datagrams.** The exchange sends 0x6B18 (27,416) bytes; PDP sends over
  `kPdpBlockMax` = 10 KiB fail (`protocol.hpp:202`, `hle_adhoc.cpp:666`, V), and client and server drop a
  connection that carries over 20 KiB (`client.cpp:747`, `server.cpp:390`).
- What already works: datagram boundaries are kept, and `GetPeerList` fills 16 entries with a fresh last-seen time
  (`client.cpp:765-769`, `hle_adhoc.cpp:523-549`, V).

**Smallest design**: "exchange with everyone in the same session who has Communications open". Discover joins group
`DISCOVER` the way `Connect` does, reports running while it joins and done when connected; `kPdpBlockMax` goes to
32 KiB (which also raises the matching data limit, `hle_adhoc.cpp:1002`). The game's own once-a-second round robin
does the rest. One PortableKit patch of about 80 to 150 lines plus a unit test with a 27,416-byte datagram. Open:
which result value the game reads as "found" (I; a trace or the table at 0x089D8250), and the "put the system into
Sleep Mode" screens.

## 4. Friend Cards

The row opens the Online Lobby's matching object (Init and Create, seen in a run), joins no group and never starts
matching; ghost battles are local (I). One thing to check: a second `MatchingInit` returns "already initialised"
(`hle_adhoc.cpp:1008`), which is harmless if each screen tears down with Delete and Term. One run settles it:
Friend Cards, a ghost battle, then the Online Lobby.

## 5. Rollback

### 5.1 The risk that is gone: threads

The fear in [online play](online-play.md) section 9 was that guest threads hold live state on host stacks, so a
snapshot could not be restored. They do not:

- **One host thread runs every guest thread.** A blocking import saves the caller's context and loads another's
  in place (PK `kernel.hpp:18-27`, V).
- **No guest register lives in a C++ local.** The register-cache passes are deliberately absent
  (`codegen_main.cpp:1757`); every MIPS register is in `AllegrexContext`, plain data of about 0.9 KB (V).
- **Native frames unwind at every import**, and the port's override of 0x0890D664 runs at the outer dispatch
  level (`runtime.cpp:458-464`, V).
- In a Communication Mode battle there are two guest threads: the game's and the audio mixer, which waits in a
  kernel delay (run logs `auto2-1.log`, `s5a-1.log`, V).

So the handler's call each frame is a clean point: all state is guest memory, CPU contexts, kernel objects and a
known list of HLE and port statics. Save just before it runs, restore by loading the state and pointing the
dispatcher back at it.

### 5.2 The state

- Guest RAM: 32 MiB in one vector that generated code points into (`guest_memory.cpp:105-116`), so a restore copies
  into it, never swaps it. VRAM and the scratchpad: 2 MiB + 16 KiB (V).
- CPU contexts, the kernel (threads, semaphores, event flags, callbacks, vtimers, the allocator, uid counters, the
  emulated clock and vblank schedule), and the starvation counter that adds emulated time every N dispatches
  (`corpus_abi.hpp:403`, V). The battle's clock is emulated time at a fixed step, so it rolls back with the kernel.
- HLE statics: display, controller, GE lists and `GeState`, audio channels, SAS, ATRAC contexts (the FFmpeg decoder
  re-warmed after a restore), file state. Ad hoc state is the outside world and stays live. Save dialogs, the
  keyboard and video must be idle (asserted).
- Port statics: `comm_battle`'s state, the linked cameras (mouse degrees carry across frames). Others to audit:
  split view, versus, the BGM playlist, voices.
- No host randomness reaches the guest (V).

### 5.3 Cost

- **Memory per frame**: in a recorded battle a frame changes a median of 29 of RAM's 512 pages of 64 KiB (p99 46),
  and 8 frames together only 30 (from `pinp_a.bin`, V). So dirty-page snapshots are about 2 MiB a frame; full
  copies 34 MiB (about 270 MiB for 8).
- **Re-running a frame**: the game's own work is 6.5 to 7.3 ms a frame on the PC ([performance](performance.md)
  section 1), drawing excluded. A headless GE walk already exists (no draw sink, `hle_media.cpp:193-205`).
- **Budget** at 30 game frames a second: about 33 ms per displayed frame for it and every re-run frame. Estimates (I):

| Machine, snapshot | Frames re-run per displayed frame |
| --- | --- |
| PC, full copies | about 2 |
| PC, dirty pages | about 3 |
| Steam Deck | 0 to 1 (its battle frame time is not measured) |

Rollback of 2 to 3 frames on top of 1 frame of delay covers about 100 to 130 ms round trip without a stall, where
delay alone needs 3 frames of felt lag. The Deck number decides how far that goes.

### 5.4 What must not repeat in a re-run frame

Presenting, drawing and the UI; mixing sound (a sound the corrected run starts joins part-way; the mispredicted
one was already heard); network sends and waits; logs and the stall watchdog; pacing to real time (a bypass that
leaves the pacing base alone). Disc reads are pure and run again. Pads come from the frame's input record. One
guest byte may differ: a block transfer that reads a render target (the EX Burst screen copy) cannot be redone
headless; visual only (I), masked in the hashes.

### 5.5 Prediction

"Repeat the last input" is how GGPO predicts, and it is often wrong here: full-resolution sticks and mouse deltas
change almost every frame. Predicting the stick within a dead band (send it quantised, as the PSP did) cuts the
misses. Measure it first from a human-played `D012_INPUT_RECORD`: how often each field changes frame to frame, raw
and quantised.

### 5.6 Plan

| # | Piece | Where | Size |
| --- | --- | --- | --- |
| 1 | Save and restore: memory, runtime, kernel (asserting no pending closures), HLE statics, ATRAC re-warm | PortableKit patches 0089 on | 800-1,200 lines |
| 2 | Simulate-only frames (no present, draw, mix, pacing) | PortableKit + port | 150-300 |
| 3 | An offline rollback test and a digest of host state | port + tools | 200-300 |
| 4 | The port's statics and the audit of others | port | 100-200 |
| 5 | The rollback driver in `comm_battle`: snapshot ring at the handler, re-run to the present, check value on confirmed frames only, wait beyond the window | port | 400-600 |
| 6 | The link: a non-blocking exchange (`trade` blocks today, `d012_net.cpp:861-932`), confirmed and predicted frames, time sync, delay and window agreed in WELCOME, link version 6 | port | 300-500 |
| 7 | Stick quantising for prediction | port | 100-200 |
| 8 | Dirty-page snapshots (VirtualAlloc write watch on Windows, soft-dirty or mprotect on Linux) and a Deck measurement | PortableKit + port | 300-500 |
| 9 | A Network page row, notices, docs | port | small |

Delay-based lockstep stays: as the off switch, and as what happens past the rollback window.

**The first experiment** (offline, no network, no firewall): in a recorded local versus replay with the clocks pinned
(`D012_INPUT_REPLAY`, `D012_NO_BUSY_CLOCK=1`, `D012_SYNC_EPOCH`), save at battle frame N, run to N+8, restore and re-run
the same 8 frames, first drawing and then headless, and compare all 512 page hashes (`D012_SYNC_TRACE`) at every
frame. Each page that differs names state the snapshot missed. Then the misprediction case: feed player 2 wrong
input, roll back, re-run with the right input; the whole run's trace must equal a plain replay's. That retires the
two biggest risks (hidden host state, and whether re-runs are exact) before any netcode is written.

**Cheaper first, and worth doing either way: balance the two copies' frame advantage.** The auto delay adds a whole
frame for the time an input waits before it is sent (`d012_net.cpp:706-712`). With the two copies' frame phases
aligned, as GGPO's time sync does, that frame may not be needed: 2 frames instead of 3 at 100 ms. Section 14's "one
copy waiting most frames" is the symptom. Small.

### 5.7 The first experiment (7 November 2026)

Save states exist (PortableKit patch 0091, `host/snapshot.hpp`; the port's `d012_rollback.cpp`) and the first
experiment above passed.

**The test.** `D012_ROLLBACK_TEST=K[,EVERY[,wrong]]`: in a battle, at every EVERY-th pad read save the whole state
(between two dispatches, after the read), run K more reads, restore, run those K again. With `wrong`, player 2's
input on the first run is wrong (stick mirrored, cross flipped), as a misprediction would be, and only the second
run gets the right input. Run on the recorded versus battle (`build/port/online/rec.inp`) replayed headless with the
clocks pinned, and `D012_SYNC_TRACE` compared with a plain replay read for read (the last run of a read counts).

**What a save holds.** Guest RAM and VRAM (copied back in place); the running thread's context, the runtime's
starvation phase and thread identity; the kernel (threads, objects, waits, clock and vblank schedule, memory blocks,
guest calls, interrupts); and participants: the display, controller latch, GE lists and state, audio channels and
SAS voices; threadman's pools and pipes; open files' positions; ATRAC contexts; and the port's input record, sync
trace counter, versus state, camera, split view and its popups. A restore restarts the hold to real time.

**What each run found** (V), in the order fixed:

| Run | Missing from the state | What showed it |
| --- | --- | --- |
| `test1` | The ATRAC music decoder | The music's buffers differed from the first restore on, then the game's own data |
| `test2` | The decoder's state after a seek: a warm-up is close to straight decoding but not to the bit, and the game's mixer keeps the difference | The music differed for 3 reads after each restore; later for good |
| `test4` | Open files' positions | The music stream's buffer (refilled from the disc) differed |
| `test5` | The decoder replay by frame number broke where the track loops | Short music differences after some restores |
| `test7` | The camera's and versus code's read counters | A HUD position for two reads after one restore |
| `soak2` | The split view's state (its "set up" flags) and its damage-number popups | The game's data parted for good: player 2's lock-on cursor was not set up again in the run after a restore |

The decoder is not copied (FFmpeg's state cannot be): it stays where it is, and after a restore the frames it made
since the save are handed out again in the order it made them, until it is back where it was.

**Results** (V), all against the same plain replay, every page of guest RAM at every pad read:

| Run | Test | Reads run twice | Reads that differ |
| --- | --- | --- | --- |
| `test10` | back 8 at every 60th read | 2,797 | 0 of 27,309 |
| `soak4` | back 8 at every 9th read (nearly the whole battle twice) | 17,800 | 0 of 26,356 |
| `wrong1` | as `soak4`, player 2's input wrong on each first run | 14,168 | 0 of 22,275 (3,976 reads differed on their wrong first run, by up to 29 pages) |

A save took 5.1 to 6.0 ms and a restore 2.2 to 2.4 ms on this PC (34 MiB copied each way, most of the save's time).

**Not verified.** Drawing on (the runs were headless, `D012_NO_RENDER=1`), sound on, a real two-copy link, the
Steam Deck's times, a state saved during a load or a dialog, a host file closed after a save (its handle does not
come back; none happened in a battle). Re-running a frame's cost was not measured (the runs were unthrottled).

**Next.** The rollback driver and the link (pieces 5 and 6 of 5.6): predict the other player's input, run ahead,
restore and re-run on a misprediction, with frames that are re-run drawing and mixing nothing (piece 2). Dirty-page
saves (piece 8) would take most of the 5 ms save away.

### 5.8 Rollback in online battles (8 November 2026)

Pieces 2, 5 and 6 of 5.6, on by a setting: `online.rollback` in settings.ini, or `D012_NET_ROLLBACK`, the frames a
copy may run past the other's input (1 to 12; empty or `off`, the default, keeps the lockstep).

**How it works.**
- **The link** (`net::battle::exchange_ahead`): this copy's input for a frame goes out as before; both inputs come
  back at once, the other's guessed as its latest when it has not come. A copy waits only once it is the rollback
  window past the other's latest input. When the real input for a guessed frame arrives and differs, the link notes
  the earliest such frame. The automatic delay takes the window off what the connection needs, down to 1 frame.
- **The ring** (`comm_battle::before_dispatch`): as the battle handler (`0x0890D664`) is about to run each frame, the
  whole state is saved into a ring of window + 4 states (PortableKit's runtime pre-dispatch hook), whose buffers are
  used again.
- **Going back** (`comm_battle::after_dispatch`): a live frame that finds a wrong guess asks for the frame to go
  back to and returns at once; right after it returns, before any more game code, that frame's state is restored
  (the hold to real time kept) and simulate-only turns on (PortableKit patch 0092). The game runs those frames
  again, each with this copy's input as it was sent and the other's as it is now known or guessed again, drawing
  and sounding nothing and waiting for nothing, until it is back at the frame it had reached, which runs live.
- **Checks**: the game's check value of every tenth frame is kept and compared with the other copy's only once
  every input up to that frame is certain.
- The port's own state goes back too: the handler's frame count, the camera's linked mouse degrees, and the rest of
  5.7's list.

**Results (V).** Two copies on this PC through the Online Lobby (the routes of section 18.3, copy 1 hosting), both
holding packets 50 ± 10 ms with 1% loss (`D012_NET_SIM`):

| Run | Netcode | Delay | 900 frames took | Went back (copy 1, copy 2) | Frames run again | In step |
| --- | --- | --- | --- | --- | --- | --- |
| `rb2` | rollback, 7 frames | 1 (auto) | | 285, 62 | 1,492, 80; at most 8 at once | yes, to the end |
| `rb3` | rollback, 7 frames | 1 (auto) | 30.0 to 31.0 s | 211, 58 (to frame 4,500) | 920, 69 | yes, to the end |
| `ls1` | lockstep (the same build) | 2 (auto) | 30.0 to 31.0 s | | | yes, to the end |

- **One frame of input delay instead of two** at 100 ms, at full speed. The windows of 30.4 to 31.0 s are the routes'
  screenshots, in both netcodes.
- A save takes 2.6 to 3.9 ms a frame on this PC (with the frames run again), a restore 2.2 to 2.5 ms.
- Copy 1 went back more: copy 2's route changes its stick more often. Every guess is "the same as last frame", so a
  moving stick is a wrong guess nearly every time it moves.

**Not verified.** Two machines; a real path's jitter; the Steam Deck's time for a save and for frames run again
(5.3); how a correction looks and sounds to a player (a sound the corrected frames start joins part-way, and a
mispredicted one already played); a rollback longer than the window (the copy waits, as the lockstep does); Group
Battle with rollback (a watched fight streams the certain frames, Communication Mode 19); Windows against Linux
with rollback.

**Next.** Fewer wrong guesses (a stick held within a small step counts as the same); the Network page row for the
setting; dirty-page saves; the Deck's numbers before it is on by default.

## 6. Suggested order for the next update

1. ~~Frame-advantage balancing in the lockstep~~: done 7 November ([Communication Mode](communication-mode.md) 17.1).
2. ~~Communications~~: done 7 November (17.2).
3. ~~Group Battle steps 0 to 3~~: steps 0, 1 and 3 done 7 November (Communication Mode 18); step 2 (the tag
   counted in every role) with step 4.
4. Rollback's first experiment: done and passed 7 November (5.7). Pieces 2, 5 and 6: done 8 November, off by
   default (5.8). Measure the Deck before promising a window.
5. ~~The spectator stream (Group Battle step 4)~~: done 8 November (Communication Mode 19). It takes rollback's
   certain frames when the fighters use rollback (not tested with a watcher yet).
