"""Read and write the game's sound banks of the kind its battle voices are kept in (magic `SSCF`).

This is the PSP form of Square Enix's .scd, older than the `SEDBSSCF` form
other tools know. A fighter's voice is voice/EN/battle/voice_<CODE8>_00.scd,
with a manikin bank (_zako) and an assist bank (_a_00) beside it. Layout, as
measured on the game's 91 battle voice banks (all 91 were written back byte
for byte when this was written):

    0x00  "SSCF"
    0x04  u32 0x02070210
    0x08  u32 size of the file
    0x0C  u32 number of slots
    0x10  u16 1
    0x12  u16 the bank's number (the fighters of the first game), else 0
    0x14  u32 the bank's number (the fighters added by this game), else 0
    0x18  8 zero bytes
    0x20  one 32-byte entry per slot
          the clips, back to back, in slot order; the file ends with the last one

    entry +0x00  u32 kind: 0 a clip, 0x00200000 an empty slot, 0x10000000 a group
          +0x04  u32 clip: file offset of its bytes     group: its first slot
          +0x08  u32 clip: byte size                    group: how many slots
          +0x0C  u16 clip: sample rate (16000 for nearly all), else 0
          +0x0E  u16 field_0e   0..100, differs from clip to clip
          +0x10  u16 field_10   100, or 0 in an empty slot
          +0x12  u16 field_12   255
          +0x14  u32 field_14   24522
          +0x18  u32 field_18   one value for most of a bank (200, 210, 5, 255)
          +0x1C  u32 field_1c   0

A clip is PS-ADPCM sound (tools/ps_adpcm.py), a multiple of 16 bytes, and is
not padded or aligned beyond that. Two slots may name the same bytes (an
assist bank does this where the main bank has a group). A group is read here
as "one of these slots": a fighter's hit reactions play the group, its slots
are short grunts, and the assist bank puts one of them in its place. That
reading comes from the data; the code that picks the slot was not traced.

A sound is asked for by number, bank number * 100 + slot (the event tracks of a
fighter's moveset archive hold such numbers; battle 0x08B33F8C adds 10000 for
a manikin, 30000 for an alternate bank and 50000 for an assist, which is why
those banks are numbered 100, 300 and 500 above the main one).

The meaning of field_0e to field_1c is not known; they are kept as read.

    sscf_bank.py list BANK.scd
    sscf_bank.py extract BANK.scd FOLDER        one .vag file of raw frames per clip
"""

import argparse
import struct
from dataclasses import dataclass, replace
from pathlib import Path

MAGIC = b"SSCF"
VERSION = 0x02070210
HEADER_BYTES = 0x20
ENTRY_BYTES = 0x20
CLIP, EMPTY, GROUP = 0, 0x00200000, 0x10000000
KINDS = {CLIP: "clip", EMPTY: "empty", GROUP: "group"}
FRAME_BYTES = 16
_ENTRY = struct.Struct("<IIIHHHHIII")


class BankError(ValueError):
    """The bytes are not a bank of this kind, or the bank cannot be written."""


@dataclass
class Entry:
    """One slot. `data` is a clip's bytes; `shares` is the earlier slot whose bytes a clip names instead of
    having its own; `first` and `count` describe a group. The rest is kept as read."""
    kind: int = EMPTY
    data: bytes = b""
    shares: int | None = None
    first: int = 0
    count: int = 0
    rate: int = 0
    field_0e: int = 0
    field_10: int = 0
    field_12: int = 255
    field_14: int = 24522
    field_18: int = 0
    field_1c: int = 0


def clip(data, like=None, **fields):
    """A clip entry holding `data`. Its other fields are those of `like` (an entry of a stock bank), or the
    commonest stock values at 16000 Hz, unless given by name (rate=..., field_0e=...)."""
    if not data or len(data) % FRAME_BYTES:
        raise BankError("A clip must be a whole number of 16-byte frames")
    base = replace(like) if like is not None else Entry(rate=16000, field_0e=100, field_10=100, field_18=200)
    return replace(base, kind=CLIP, data=bytes(data), shares=None, first=0, count=0, **fields)


def shared(slot, like=None, **fields):
    """A clip entry that names the bytes of an earlier slot."""
    base = replace(like) if like is not None else Entry(rate=16000, field_0e=100, field_10=100, field_18=200)
    return replace(base, kind=CLIP, data=b"", shares=slot, first=0, count=0, **fields)


@dataclass
class Bank:
    number: int = 0
    wide_number: bool = False       # the number is kept at 0x14 rather than 0x12
    entries: list = None
    field_10: int = 1
    reserved: bytes = bytes(8)

    def __post_init__(self):
        if self.entries is None:
            self.entries = []

    def clip_bytes(self, slot):
        """The bytes slot plays: its own, or those of the slot it shares with. None for other kinds."""
        entry = self.entries[slot]
        if entry.kind != CLIP:
            return None
        return self.entries[entry.shares].data if entry.shares is not None else entry.data

    def to_bytes(self):
        count = len(self.entries)
        position = HEADER_BYTES + count * ENTRY_BYTES
        offsets = {}
        body = bytearray()
        table = bytearray()
        for slot, entry in enumerate(self.entries):
            if entry.kind == CLIP and entry.shares is None:
                if not entry.data or len(entry.data) % FRAME_BYTES:
                    raise BankError(f"Slot {slot}: a clip must be a whole number of 16-byte frames")
                offsets[slot] = position + len(body)
                body += entry.data
        for slot, entry in enumerate(self.entries):
            if entry.kind == CLIP:
                source = slot if entry.shares is None else entry.shares
                if source not in offsets or source > slot:
                    raise BankError(f"Slot {slot} shares the bytes of slot {source}, which is not an earlier clip")
                where, size = offsets[source], len(self.entries[source].data)
            elif entry.kind == GROUP:
                if entry.count < 1 or entry.first + entry.count > count:
                    raise BankError(f"Slot {slot}: a group of slots {entry.first}..{entry.first + entry.count - 1}")
                where, size = entry.first, entry.count
            elif entry.kind == EMPTY:
                where, size = 0, 0
            else:
                raise BankError(f"Slot {slot}: kind {entry.kind:#x} is not known")
            table += _ENTRY.pack(entry.kind, where, size, entry.rate, entry.field_0e, entry.field_10,
                                 entry.field_12, entry.field_14, entry.field_18, entry.field_1c)
        if len(self.reserved) != 8:
            raise BankError("The header's reserved bytes are eight")
        size = position + len(body)
        header = MAGIC + struct.pack("<IIIHHI", VERSION, size, count, self.field_10,
                                     0 if self.wide_number else self.number,
                                     self.number if self.wide_number else 0) + self.reserved
        return bytes(header + table + body)


def parse(data):
    """Read a bank. Refuses what `Bank.to_bytes` could not write back as it is."""
    if len(data) < HEADER_BYTES or data[:4] != MAGIC:
        raise BankError("Not an SSCF bank")
    version, size, count, field_10, narrow, wide = struct.unpack_from("<IIIHHI", data, 4)
    if version != VERSION:
        raise BankError(f"Version {version:#010x} is not known")
    if size != len(data):
        raise BankError(f"The header gives {size} bytes; the file has {len(data)}")
    start = HEADER_BYTES + count * ENTRY_BYTES
    if start > len(data):
        raise BankError("The entry table is cut short")
    if narrow and wide:
        raise BankError("The bank has a number in both places")
    bank = Bank(number=wide or narrow, wide_number=bool(wide), field_10=field_10, reserved=bytes(data[0x18:0x20]))
    owners = {}
    position = start
    for slot in range(count):
        kind, where, length, rate, field_0e, field_10, field_12, field_14, field_18, field_1c = \
            _ENTRY.unpack_from(data, HEADER_BYTES + slot * ENTRY_BYTES)
        entry = Entry(kind=kind, rate=rate, field_0e=field_0e, field_10=field_10, field_12=field_12,
                      field_14=field_14, field_18=field_18, field_1c=field_1c)
        if kind == CLIP:
            if (where, length) in owners:
                entry.shares = owners[where, length]
            else:
                if where != position or length == 0 or length % FRAME_BYTES or where + length > len(data):
                    raise BankError(f"Slot {slot}: {length} bytes at {where:#x} do not follow the clip before")
                entry.data = bytes(data[where:where + length])
                owners[where, length] = slot
                position = where + length
        elif kind == GROUP:
            entry.first, entry.count = where, length
            if length < 1 or where + length > count:
                raise BankError(f"Slot {slot}: a group of slots {where}..{where + length - 1}")
        elif kind == EMPTY:
            if where or length:
                raise BankError(f"Slot {slot}: an empty slot with an offset or a size")
        else:
            raise BankError(f"Slot {slot}: kind {kind:#x} is not known")
        bank.entries.append(entry)
    if position != len(data):
        raise BankError(f"{len(data) - position} bytes follow the last clip")
    return bank


def describe(bank):
    """One line per slot, for the command line."""
    lines = [f"bank {bank.number}, {len(bank.entries)} slots"]
    for slot, entry in enumerate(bank.entries):
        text = f"{slot:3d} {KINDS[entry.kind]:5s}"
        if entry.kind == CLIP:
            data = bank.clip_bytes(slot)
            samples = (len(data) // FRAME_BYTES - 2) * 28
            text += f" {len(data):6d} bytes {entry.rate:5d} Hz {samples / entry.rate:6.3f} s"
            if entry.shares is not None:
                text += f" (the bytes of slot {entry.shares})"
        elif entry.kind == GROUP:
            text += f" one of slots {entry.first}..{entry.first + entry.count - 1}"
        if entry.kind != EMPTY:
            text += f"  0e={entry.field_0e} 10={entry.field_10} 18={entry.field_18}"
        lines.append(text)
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("action", choices=("list", "extract"))
    parser.add_argument("bank", type=Path)
    parser.add_argument("folder", type=Path, nargs="?")
    arguments = parser.parse_args()
    bank = parse(arguments.bank.read_bytes())
    if arguments.action == "list":
        print(describe(bank))
        return
    if arguments.folder is None:
        parser.error("extract needs a folder")
    arguments.folder.mkdir(parents=True, exist_ok=True)
    for slot, entry in enumerate(bank.entries):
        if entry.kind == CLIP and entry.shares is None:
            (arguments.folder / f"{arguments.bank.stem}_{slot:02d}.vag").write_bytes(entry.data)


if __name__ == "__main__":
    main()
