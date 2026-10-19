"""Read files from the game's PACKAGE.BIN by name, through PACKAGE_INFO.BIN.

The retail ISO is opened read-only (tools/inspect_iso.Iso). Nothing is written.
"""

import struct

from inspect_iso import Iso

ISO_DATA = "/PSP_GAME/USRDIR/DATA/"

_CRC = []
for _byte in range(256):
    _value = _byte << 24
    for _ in range(8):
        _value = ((_value << 1) ^ 0x04C11DB7 if _value & 0x80000000 else _value << 1) & 0xFFFFFFFF
    _CRC.append(_value)


def name_hash(name):
    """The game's file-name hash (main 0x08871288): CRC-32/BZIP2 of the lower-case name."""
    crc = 0xFFFFFFFF
    for byte in name.lower().encode("ascii"):
        crc = ((crc << 8) & 0xFFFFFFFF) ^ _CRC[(crc >> 24) ^ byte]
    return crc ^ 0xFFFFFFFF


def parse_index(data):
    """PACKAGE_INFO.BIN: {name hash: (start sector, byte size)}."""
    _, magic, count, _ = struct.unpack_from("<I4sII", data, 0)
    if magic != b"pack" or len(data) != 16 + count * 12:
        raise ValueError("Not a PACKAGE_INFO.BIN")
    records = {}
    for index in range(count):
        key, sector, size = struct.unpack_from("<III", data, 16 + index * 12)
        records[key] = (sector & 0xFFFFF, size & 0x0FFFFFFF)
    return records


class Archive:
    """PACKAGE.BIN inside the ISO: files found by name hash through PACKAGE_INFO.BIN."""

    def __init__(self, iso_path):
        self.iso = Iso(iso_path)
        entries = {entry["path"].upper(): entry for entry in self.iso.files() if entry["path"].upper().startswith(ISO_DATA)}
        index = self.iso.content(entries[ISO_DATA + "PACKAGE_INFO.BIN"])
        self.package = entries[ISO_DATA + "PACKAGE.BIN"]
        self.records = parse_index(index)

    def read(self, name):
        record = self.records.get(name_hash(name))
        if record is None:
            return None
        sector, size = record
        if sector * 2048 + size > self.package["size"]:
            raise ValueError(f"{name} lies outside PACKAGE.BIN")
        return self.iso.read(self.package["extent_lba"] * self.iso.block + sector * 2048, size)

    def close(self):
        self.iso.stream.close()
