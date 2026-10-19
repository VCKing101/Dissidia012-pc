"""Safety checks for fixed-extent local test packages, using synthetic bytes."""

import struct
import tempfile
import unittest
from pathlib import Path

from prepare_runtime_packages import (copy_patched, directory_entries, replacement_patches,
                                      verify_patched)


def record(name, lba, size, flags=0):
    name = name.encode("ascii")
    length = 33 + len(name) + (len(name) % 2 == 0)
    result = bytearray(length)
    result[0] = length
    struct.pack_into("<I", result, 2, lba)
    struct.pack_into(">I", result, 6, lba)
    struct.pack_into("<I", result, 10, size)
    struct.pack_into(">I", result, 14, size)
    result[25] = flags
    result[32] = len(name)
    result[33:33 + len(name)] = name
    return bytes(result)


class SyntheticIso:
    block = 2048
    size = 8192
    root = {"extent_lba": 1, "size": 2048}

    def __init__(self, raw=None):
        from inspect_iso import Iso
        self.record = Iso.record
        self.raw = bytearray(self.size)
        if raw:
            self.raw[2048:2048 + len(raw)] = raw

    def read(self, offset, size):
        if offset + size > self.size:
            raise ValueError("Extent outside image")
        return bytes(self.raw[offset:offset + size])


class RuntimePackageChecks(unittest.TestCase):
    def test_smaller_file_updates_both_sizes_and_zeros_retired_tail(self):
        iso = SyntheticIso(record("EBOOT.BIN;1", 2, 17))
        entries = directory_entries(iso)
        iso.raw[4096:4113] = b"x" * 17
        patches, manifest = replacement_patches(iso, entries, {"/EBOOT.BIN": b"ELF"})
        self.assertEqual(patches[0], (2058, struct.pack("<I", 3) + struct.pack(">I", 3)))
        self.assertEqual(patches[1], (4096, b"ELF" + bytes(2045)))
        self.assertEqual(manifest[0]["extent_lba"], 2)

    def test_oversized_replacement_rejected(self):
        iso = SyntheticIso(record("X", 2, 1))
        with self.assertRaisesRegex(ValueError, "does not fit"):
            replacement_patches(iso, directory_entries(iso), {"/X": b"xx"})

    def test_missing_file_rejected(self):
        with self.assertRaisesRegex(ValueError, "missing"):
            replacement_patches(SyntheticIso(), [], {"/X": b"x"})

    def test_nonzero_original_padding_rejected(self):
        iso = SyntheticIso(record("X", 2, 1))
        iso.raw[4097] = 1
        with self.assertRaisesRegex(ValueError, "padding is nonzero"):
            replacement_patches(iso, directory_entries(iso), {"/X": b"x"})

    def test_inconsistent_endian_size_rejected(self):
        raw = bytearray(record("X", 2, 1))
        raw[17] = 2
        with self.assertRaisesRegex(ValueError, "endian"):
            directory_entries(SyntheticIso(raw))

    def test_overlap_and_duplicate_path_rejected(self):
        with self.assertRaisesRegex(ValueError, "overlap"):
            directory_entries(SyntheticIso(record("X", 2, 1) + record("Y", 2, 1)))
        with self.assertRaisesRegex(ValueError, "Duplicate"):
            directory_entries(SyntheticIso(record("X", 2, 1) + record("X", 3, 1)))

    def test_multi_extent_rejected(self):
        with self.assertRaisesRegex(ValueError, "multi-extent"):
            directory_entries(SyntheticIso(record("X", 2, 1, 0x80)))

    def test_outside_image_allocation_rejected(self):
        with self.assertRaisesRegex(ValueError, "outside"):
            directory_entries(SyntheticIso(record("X", 4, 1)))

    def test_copy_keeps_source_and_rejects_unlisted_byte_changes(self):
        with tempfile.TemporaryDirectory() as name:
            source, output = Path(name) / "source.iso", Path(name) / "output.iso"
            original = bytes(range(256)) * 128
            source.write_bytes(original)
            patches = [(4096, b"test"), (32000, b"end")]
            copy_patched(source, output, patches)
            verify_patched(source, output, patches)
            self.assertEqual(source.read_bytes(), original)
            with output.open("r+b") as stream:
                stream.seek(100)
                stream.write(b"x")
            with self.assertRaisesRegex(ValueError, "Unexpected"):
                verify_patched(source, output, patches)

    def test_source_overwrite_rejected(self):
        with tempfile.TemporaryDirectory() as name:
            source = Path(name) / "source.iso"
            source.write_bytes(b"source")
            with self.assertRaisesRegex(ValueError, "overwrite"):
                copy_patched(source, source, [])
            self.assertEqual(source.read_bytes(), b"source")

    def test_hard_link_cannot_truncate_source_or_another_image(self):
        with tempfile.TemporaryDirectory() as name:
            source, output, other = (Path(name) / f"{label}.iso" for label in ("source", "output", "other"))
            source.write_bytes(b"source")
            output.hardlink_to(source)
            with self.assertRaisesRegex(ValueError, "overwrite"):
                copy_patched(source, output, [])
            self.assertEqual(source.read_bytes(), b"source")
            output.unlink()
            other.write_bytes(b"other image")
            output.hardlink_to(other)
            copy_patched(source, output, [(0, b"S")])
            self.assertEqual(other.read_bytes(), b"other image")
            self.assertEqual(source.read_bytes(), b"source")
            self.assertEqual(output.read_bytes(), b"Source")


if __name__ == "__main__":
    unittest.main()
