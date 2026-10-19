"""Find and renumber the voice clips a fighter's moveset archive asks for.

A voice clip is asked for by number: the number of the voice bank it is in
(tools/sscf_bank.py) times 100, plus its slot. The source fighter's main bank
is number 24100, so his clips are 2410000 to 2410053.

A moveset archive (obj/p_xxxNNN.objx, obj/p_xxxNNN_a.objx for the assist) names
clips in one place only: the event tracks, the chunk of kind 8 in the archive's
directory. As the game reads it:

    chunk   u32 version, u32 count, u32 offset[count] of each track from the start of the chunk
    track   u16 animation, u8 ranged events, u8 instant events, u16 offset of the instant events,
            u16 size; the ranged events start at +8
    ranged  u16 first tick, u16 last tick, u8 type, u8 arguments, u8 condition, u8, u32,
            then one u32 per argument
    instant u16 tick, u8 type, u8 arguments, u32 condition, then one u32 per argument

Instant event type 0x15 is "say": entry 0x15 of the jump table at main
0x089C7B08 is the only caller in the main executable of battle 0x08B33F8C,
which adds 10000 to the first argument for a manikin, 30000 for the alternate
bank and 50000 for an assist, and hands it to the fighter's voice channel
(main 0x0884EA08). So the first argument of every such event is a voice
number, always written for the main bank, and nothing else in the archive is:
the other numbers of similar size (types 0x03, 0x11, 0x12) are sound effects
and effects of the fighter's own bank, which is numbered 100 less (24000).

`renumber` moves those arguments to another bank number and changes no other
byte. It goes by what the events hold, not by a fixed old number, so it can be
run again with the same number (nothing changes) or with the old one (the
archive is as it was).

The engine itself plays four more clips by numbers kept in the character's
chrp record (system.bin, +0x7C to +0x88), not in the archive: the defeat cry,
two EX Mode entry lines and the assist's call-in line.

Two tables of the game are keyed by these numbers, both in
general_archive/main/main.bin, both sorted rows of {first number, last number,
...} found by binary search, and neither has rows for a bank number the game
did not ship with:

    voice_priority_parameter.bin   main 0x088A4364(number) -> row or 0. The voice channel takes
                                   the priority from the row; with no row the priority is 0, the
                                   lowest, and the clip still plays (a stock assist's attack
                                   shouts have no rows). The source fighter's rows give hit
                                   grunts 9, the defeat cry 10, attack shouts 3 or 4.
    se_override.bin                main 0x088A0BC0(number) -> row or 0, kept with the playing
                                   sound; its flag word is tested bit by bit. Every voice range
                                   has flags 24. What bits 8 and 16 do was not traced.

A fighter on a bank number of his own therefore sounds, but with flat priority
and without those flags, unless the lookups are answered for his numbers with
the rows of the fighter he was copied from (same slot, the old bank number).
"""

import struct

ARCHIVE_MAGIC = b"ARC\x01"
TRACKS_KIND = 8
SAY = 0x15
SLOTS = 100


class VoiceError(ValueError):
    """The archive's event tracks are not laid out as expected."""


def find_tracks(archive):
    """Offset and size of the event-track chunk, from the archive's own directory
    (16-byte entries after a 16-byte header: kind, tag, offset, size)."""
    if archive[:4] != ARCHIVE_MAGIC or len(archive) < 16:
        raise VoiceError("Not a moveset archive")
    count = struct.unpack_from("<I", archive, 4)[0]
    if 16 + count * 16 > len(archive):
        raise VoiceError("The archive's directory is cut short")
    found = []
    for index in range(count):
        kind, _, offset, size = struct.unpack_from("<I4sII", archive, 16 + index * 16)
        if kind == TRACKS_KIND:
            if offset + size > len(archive):
                raise VoiceError("The event tracks lie outside the archive")
            found.append((offset, size))
    if len(found) != 1:
        raise VoiceError(f"The archive has {len(found)} event-track chunks, not one")
    return found[0]


def say_events(archive):
    """Every "say" event of the archive: [(offset in the archive of its voice number, the number,
    animation, tick)]. Walks each track's two event lists whole, and refuses a track whose
    lists do not end where its header says."""
    start, size = find_tracks(archive)
    chunk = archive[start:start + size]
    if size < 8:
        raise VoiceError("The event tracks are cut short")
    count = struct.unpack_from("<I", chunk, 4)[0]
    if 8 + count * 4 > size:
        raise VoiceError("The event tracks' offsets are cut short")
    events = []
    for track in struct.unpack_from(f"<{count}I", chunk, 8):
        if track + 8 > size:
            raise VoiceError("An event track lies outside the chunk")
        animation, ranged, instant, instant_offset, track_size = struct.unpack_from("<HBBHH", chunk, track)
        if track + track_size > size:
            raise VoiceError(f"The track of animation {animation} runs past the chunk")
        position = track + 8
        for _ in range(ranged):
            if position + 12 > track + track_size:
                raise VoiceError(f"The track of animation {animation} is cut short")
            position += 12 + chunk[position + 5] * 4
        if position != track + instant_offset:
            raise VoiceError(f"The ranged events of animation {animation} do not end where its instant events start")
        for _ in range(instant):
            if position + 8 > track + track_size:
                raise VoiceError(f"The track of animation {animation} is cut short")
            tick, kind, arguments = struct.unpack_from("<HBB", chunk, position)
            if kind == SAY:
                if arguments < 1:
                    raise VoiceError(f"A say event of animation {animation} names no clip")
                events.append((start + position + 8, struct.unpack_from("<I", chunk, position + 8)[0], animation, tick))
            position += 8 + arguments * 4
        if position != track + track_size:
            raise VoiceError(f"The instant events of animation {animation} do not end with its track")
    return events


def bank_number(archive):
    """The voice bank number the archive's say events are written for, or None if it has none.
    Refuses an archive whose events name more than one bank."""
    numbers = {number // SLOTS for _, number, _, _ in say_events(archive)}
    if len(numbers) > 1:
        raise VoiceError(f"The say events name clips of banks {sorted(numbers)}, not of one bank")
    return numbers.pop() if numbers else None


def renumber(archive, number):
    """The archive with every say event asking bank `number` for the slot it asked its old bank
    for. Returns the same object when nothing has to change."""
    if not 0 < number <= 0xFFFFFFFF // SLOTS:
        raise VoiceError(f"{number} cannot be a bank number")
    if bank_number(archive) in (None, number):
        return archive
    data = bytearray(archive)
    for offset, old, _, _ in say_events(archive):
        struct.pack_into("<I", data, offset, number * SLOTS + old % SLOTS)
    return bytes(data)
