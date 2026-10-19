"""Check the moveset compiler and decompiler (tools/moveset.py): exact values, the text format, the
archive layout, round trips of game archives and edits that change lengths and offsets.

The full sweep over every fighter's archives is `tools/moveset.py sweep`; here a few are sampled."""
import contextlib
import io
import random
import struct
import tempfile
import unittest
from pathlib import Path

import character_text
import character_voice
import moveset

ROOT = Path(__file__).resolve().parents[1]
ISO = ROOT / "Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso"
SQUALL = "obj/p_eht100.objx"
# A sample across the kinds of archive: a plain fighter, one with three forms, Chaos, a fighter
# added by this game, an assist, and the assist-only Aerith.
SAMPLE = ("obj/p_sev100.objx", "obj/p_gst130.objx", "obj/p_org200_00.objx", "obj/p_fiv210.objx",
          "obj/p_eht100_a.objx", "obj/p_sev120_a.objx")


# ------------------------------------------------------------------------------- synthetic archive
def arc(chunks, words=(0, 0)):
    """An ARC archive laid out as the game's packer lays them out."""
    out = bytearray(b"ARC\x01" + struct.pack("<III", len(chunks), *words))
    directory = len(out)
    out += bytes(16 * len(chunks))
    for index, (kind, tag, body) in enumerate(chunks):
        out += bytes(-len(out) % moveset.alignment(kind))
        struct.pack_into("<I4sII", out, directory + 16 * index, kind, tag, len(out), len(body))
        out += body
    return bytes(out)


def attack(number, motion, name, reach=7.5):
    record = bytearray(0x74)
    struct.pack_into("<H", record, 0, number)
    struct.pack_into("<HhI", record, 0x1C, motion, -1, 0)
    struct.pack_into("<3f", record, 0x28, reach, -1.0, 1.0)
    struct.pack_into("<I", record, 0x10, 0xFFFFFFFF)
    record[0x64:0x64 + len(name)] = name
    return bytes(record)


def hit(number, name):
    record = bytearray(0x44)
    struct.pack_into("<I", record, 0, number)
    struct.pack_into("<f", record, 0x1C, 1.3)
    record[0x34:0x34 + len(name)] = name
    return bytes(record)


def track(anim, ranged, instant):
    body = b"".join(struct.pack("<HHBBBBI", start, end, kind, len(args), 0, 0, 0) + struct.pack(f"<{len(args)}I", *args)
                    for start, end, kind, args in ranged)
    tail = b"".join(struct.pack("<HBBI", tick, kind, len(args), 0) + struct.pack(f"<{len(args)}I", *args)
                    for tick, kind, args in instant)
    return struct.pack("<HBBHH", anim, len(ranged), len(instant), 8 + len(body), 8 + len(body) + len(tail)) + body + tail


def tracks(items):
    out = bytearray(struct.pack("<II", 3, len(items)))
    position = 8 + 4 * len(items)
    for item in items:
        out += struct.pack("<I", position)
        position += len(item)
    return bytes(out) + b"".join(items)


def small_archive():
    plya = struct.pack("<II", 22, 2) + attack(1470, 10000, b"LAND_NEAR1") + attack(1471, 10001, b"LAND_NEAR2")
    atkp = struct.pack("<II", 12, 1) + hit(861, b"LAND_NEAR1")
    comb = struct.pack("<HhI", 6, 1, 8) + struct.pack("<Hhhh", 314, 2, -1, -1) + struct.pack("<HhhH", 1470, 0, -1, 0) \
        + struct.pack("<HhhH", 1471, 10, 30, 0)
    motions = struct.pack("<II", 2, 2) + struct.pack("<HHB3x", 10000, 1, 1) + struct.pack("<HHB3x", 10001, 0, 0)
    events = tracks([track(0, [(0, 2, 2, [180, 0, 1, 0])], [(14, 0x15, [2210019, 0, 0])]),
                     track(1, [(16, 24, 0, [861, 48])], [(14, 0x25, [struct.unpack("<I", struct.pack("<f", 0.48))[0], 0xFFFFFFFF])])])
    text = character_text.rebuild(b"mlng" + struct.pack("<III", 1, 16, 2) + b"\0\0" + bytes(64), ["Cross Slash", None])
    nested = arc([(7, b"d_el", struct.pack("<II", 2, 1) + struct.pack("<HHB3x", 2000, 2, 0))])
    return arc([(7, b"p_xx", motions), (8, b"p_xx", events), (6, b"p_xx", b"\x02\0\0\0" * 3 + b"\x01\x02"),
                (14, b"snd_", b"SSCF" + bytes(27)), (1, b"plya", plya), (1, b"comb", comb), (1, b"atkp", atkp),
                (1, b"text", text), (3, b"wind", b"MIG.00.1PSP\0" + bytes(5)), (18, b"d_xx", nested)])


def chunk(data, wanted_kind, wanted_tag=None):
    _, chunks = moveset.arc_directory(data)
    for kind, tag, offset, size in chunks:
        if kind == wanted_kind and (wanted_tag is None or tag == wanted_tag):
            return offset, data[offset:offset + size]
    raise KeyError(wanted_kind)


class NumberChecks(unittest.TestCase):
    def test_floats_are_written_short_and_read_back_exactly(self):
        self.assertEqual(moveset.float_text(0x3F8CCCCD), "1.1")
        self.assertEqual(moveset.float_text(0x41800000), "16.0")
        self.assertEqual(moveset.float_text(0x80000000), "-0.0")
        self.assertEqual(moveset.float_text(0x7FC00000), "0x7FC00000")       # NaN: the bits
        self.assertEqual(moveset.float_text(0x00000005), "0x00000005")       # denormal: the bits
        rng = random.Random(12)
        for bits in [rng.getrandbits(32) for _ in range(3000)] + [0, 1, 0x7F800000, 0xFF800000, 0x00800000]:
            self.assertEqual(moveset.parse_float(moveset.float_text(bits), "test"), bits)
            self.assertEqual(moveset.parse_auto(moveset.auto_text(bits), "test"), bits)

    def test_words_of_unknown_type_read_as_what_they_look_like(self):
        self.assertEqual(moveset.auto_text(861), "861")
        self.assertEqual(moveset.auto_text(0xFFFFFFFF), "-1")
        self.assertEqual(moveset.auto_text(0x3EF5C28F), "0.48")
        self.assertEqual(moveset.auto_text(0x21B8E3), "2210019")
        self.assertEqual(moveset.auto_text(0x80000000 | 0x12345), "0x80012345")
        self.assertEqual(moveset.parse_auto("1.0", "x"), 0x3F800000)
        self.assertEqual(moveset.parse_auto("1", "x"), 1)
        self.assertEqual(moveset.parse_auto("-2", "x"), 0xFFFFFFFE)

    def test_strings_keep_every_byte(self):
        for data in (b"LAND_NEAR1", b"A\0B\x05", b'say "hi"\\', b""):
            self.assertEqual(moveset.parse_string(moveset.string_text(data), "x"), data)
        self.assertEqual(moveset.string_text(b"NEAR\0\0\0\0"), '"NEAR"')


class FormatChecks(unittest.TestCase):
    def setUp(self):
        self.data = small_archive()
        self.files, self.raw = moveset.render(self.data)

    def build(self, files=None):
        files = files or self.files
        return moveset.build(lambda path: files[path])

    def test_a_built_archive_comes_back_byte_for_byte(self):
        self.assertEqual(self.build(), self.data)
        self.assertEqual({path for path, *_ in self.raw}, {"02_kind6.txt", "03_kind14_snd_.bin", "08_kind3_wind.bin"})
        self.assertIn("09_kind18_d_xx/00_motions.txt", self.files)

    def test_chunks_start_where_the_games_packer_puts_them(self):
        _, chunks = moveset.arc_directory(self.data)
        for kind, tag, offset, size in chunks:
            self.assertEqual(offset % moveset.alignment(kind), 0, (kind, tag))
        offset, _ = chunk(self.data, 14)
        self.assertEqual(offset % 64, 0)

    def test_the_text_names_what_it_can(self):
        plya = self.files["04_plya.txt"].decode()
        self.assertIn('name:            "LAND_NEAR1"', plya)
        self.assertRegex(plya, r"motion: +10000 +# anim 1 variant 1")
        self.assertIn("reach:           7.5", plya)
        events = self.files["01_tracks.txt"].decode()
        self.assertRegex(events, r"R +16 +24 event_0x00 861 48 +# atkp 861 LAND_NEAR1")
        self.assertRegex(events, r"I +14 +event_0x15 2210019 0 0 +# voice bank 22100 slot 19")
        self.assertRegex(events, r"I +14 +event_0x25 0.48 -1\n")
        self.assertRegex(self.files["05_comb.txt"].decode(), r"stage 1471 min_frame=10 max_frame=30 flags=0x0000 +# LAND_NEAR2")
        text = self.files["07_text.txt"].decode()
        self.assertIn('string 0 "Cross Slash"', text)
        self.assertIn("string 1 none", text)
        self.assertIn("padding", text)

    def test_comments_and_spacing_do_not_matter(self):
        files = dict(self.files)
        lines = []
        for line in files["04_plya.txt"].decode().splitlines():
            lines.append("\t" + line.strip() + "   # note" if line.startswith("  ") else line)
        files["04_plya.txt"] = ("# my notes\n" + "\n".join(lines)).encode()
        self.assertEqual(self.build(files), self.data)

    def test_mistakes_are_refused_with_the_line(self):
        cases = [("  reach:           7.5", "  reech: 7.5", "line \\d+: the attack record has no field reech"),
                 ("  reach:           7.5", "  reach: far", "'far' is not a number"),
                 ('  name:            "LAND_NEAR1"', '  name: "A_NAME_TOO_LONG_TO_FIT"', "22 bytes do not fit 16"),
                 ("  id:              1470", "  id: 70000", "70000 does not fit 16 bits"),
                 ("  id:              1470", "  id: 1470\n  field_0x00: 5", "given twice")]
        for old, new, message in cases:
            files = dict(self.files)
            text = files["04_plya.txt"].decode()
            self.assertIn(old, text)
            files["04_plya.txt"] = text.replace(old, new, 1).encode()
            with self.assertRaisesRegex(moveset.MovesetError, message):
                self.build(files)

    def test_an_unsorted_motion_table_is_built_with_a_warning(self):
        files = dict(self.files)
        text = files["00_motions.txt"].decode()
        files["00_motions.txt"] = text.replace("motion 10000 ", "motion 10005 ", 1).encode()
        printed = io.StringIO()
        with contextlib.redirect_stderr(printed):
            built = self.build(files)
        self.assertIn("motion 10001 comes after 10005", printed.getvalue())
        _, body = chunk(built, 7)
        self.assertEqual(struct.unpack_from("<H", body, 8)[0], 10005)

    def test_records_the_game_would_misread_are_built_with_a_warning(self):
        files = dict(self.files)
        files["06_atkp.txt"] = (files["06_atkp.txt"].decode()
                                + '\nhit\n  id: 860\n  name: "EARLIER"\nhit\n  id: 860\n').encode()
        files["01_tracks.txt"] = files["01_tracks.txt"].decode().replace("event_0x00 861 48", "event_0x00 861 64", 1).encode()
        printed = io.StringIO()
        with contextlib.redirect_stderr(printed):
            self.build(files)
        self.assertIn("hit 860 is out of order", printed.getvalue())
        self.assertIn("id 860 is used by more than one hit", printed.getvalue())
        self.assertIn("shape 64 is past 63", printed.getvalue())

    def test_a_field_answers_to_its_neutral_name(self):
        files = dict(self.files)
        text = files["04_plya.txt"].decode().replace("  motion:          10000", "  field_0x1C: 10002", 1)
        files["04_plya.txt"] = text.encode()
        built = self.build(files)
        offset, body = chunk(built, 1, b"plya")
        self.assertEqual(struct.unpack_from("<H", body, 8 + 0x1C)[0], 10002)

    def semantics(self, fields, events=""):
        with tempfile.TemporaryDirectory() as folder:
            Path(folder, "fields.tsv").write_text("record\toffset\ttype\tname\tmeaning\n" + fields, encoding="utf-8")
            Path(folder, "events.tsv").write_text("list\ttype\tname\targs\tmeaning\n" + events, encoding="utf-8")
            return moveset.load_semantics(folder)

    def test_semantics_notes_rename_fields_and_events_and_change_no_byte(self):
        semantics = self.semantics(
            "plya\t0x28\tf32\tchoose_distance\tdistance at which the attack is chosen\n"
            "atkp\t0x1C\tf32\tknockback\t\n"
            "plya\t0x2A\tu16\toverlapping\tcuts into the row before\n"
            "plya\t0x72\tu32\tpast_the_end\truns past the record\n"
            "comb\t2\ts16\tnumber\tthe stage count is the tool's own\n",
            "ranged\t0x00\thit_window\tatkp:u32, collision_bit:u32\tarms a hit\n"
            "instant\t0x15\tsay\tvoice:u32, a, b\tplays a voice clip\n"
            "instant\t0x25\tdash\tspeed:f32, angle:s32\t\n")
        files, _ = moveset.render(self.data, semantics)
        plya, events = files["04_plya.txt"].decode(), files["01_tracks.txt"].decode()
        self.assertIn("choose_distance: 7.5", plya)
        self.assertIn('name:            "LAND_NEAR1"', plya)
        self.assertEqual(len(semantics.skipped), 3, semantics.skipped)
        for word in ("overlapping", "past_the_end"):
            self.assertNotIn(word, plya)
        self.assertNotIn("number", files["05_comb.txt"].decode())
        self.assertEqual(files["notes/fields.tsv"].decode(), semantics.texts["fields.tsv"])
        self.assertIn("names notes", files["archive.txt"].decode())
        # The folder compiles by its own copy of the notes, whatever the notes say by then.
        self.assertEqual(moveset.build(lambda path: files[path]), self.data)
        self.assertRegex(events, r"R +16 +24 hit_window 861 48 +# atkp=861 collision_bit=48; atkp 861 LAND_NEAR1")
        self.assertRegex(events, r"I +14 +dash 0.48 -1 ")
        self.assertIn("knockback:", files["06_atkp.txt"].decode())
        self.assertEqual(moveset.build(lambda path: files[path], semantics), self.data)
        # A file written before the notes (built-in or neutral names) still compiles under them.
        self.assertEqual(moveset.build(lambda path: self.files[path], semantics), self.data)
        neutral, _ = moveset.render(self.data, moveset.Semantics())
        self.assertEqual(moveset.build(lambda path: neutral[path], semantics), self.data)

    def test_semantics_notes_may_join_fields_into_an_array(self):
        semantics = self.semantics("plya\t0x60\tu8[4]\tbytes_60\tfour bytes\n")
        files, _ = moveset.render(self.data, semantics)
        self.assertIn("bytes_60:        0 0 0 0", files["04_plya.txt"].decode())
        self.assertEqual(moveset.build(lambda path: files[path], semantics), self.data)


@unittest.skipUnless(ISO.is_file(), "the game image is not here")
class GameArchiveChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        game = moveset.open_game(ISO)
        try:
            cls.squall = game.read(SQUALL)
            cls.sample = {name: game.read(name) for name in SAMPLE}
            cls.model = game.read("obj/P_EHT100_1P.gmo")
        finally:
            game.close()
        cls.neutral = moveset.Semantics()
        cls.files, cls.raw = moveset.render(cls.squall, cls.neutral, moveset.gmo_motion_names(cls.model))

    def build(self, files):
        return moveset.build(lambda path: files[path], self.neutral)

    def assert_only_chunk_changed(self, built, kind, tag):
        """Every chunk but one holds what it held, on its own alignment, in the same order."""
        old_dir, new_dir = moveset.arc_directory(self.squall)[1], moveset.arc_directory(built)[1]
        self.assertEqual([(k, t) for k, t, _, _ in old_dir], [(k, t) for k, t, _, _ in new_dir])
        for (k, t, offset, size), (_, _, offset2, size2) in zip(old_dir, new_dir):
            self.assertEqual(offset2 % moveset.alignment(k), 0)
            if (k, t) != (kind, tag):
                self.assertEqual(built[offset2:offset2 + size2], self.squall[offset:offset + size], (k, t))
        self.assertEqual(len(built), new_dir[-1][2] + new_dir[-1][3])

    def edited(self, path, old, new):
        files = dict(self.files)
        text = files[path].decode()
        self.assertIn(old, text)
        files[path] = text.replace(old, new, 1).encode()
        return files

    def test_sampled_archives_come_back_byte_for_byte(self):
        for name, data in [(SQUALL, self.squall)] + list(self.sample.items()):
            same, message, raw = moveset.check(data)
            self.assertTrue(same, f"{name}: {message}")
            parsed = {path.split("_", 1)[1].split(".")[0] for path, kind, *_ in raw}
            for wanted in ("plya", "comb", "atkp", "text", "motions", "tracks", "shapes", "magi", "brat"):
                self.assertNotIn(wanted, parsed, name)

    def test_squalls_attack_chunks_are_all_read_field_by_field(self):
        raw = {path for path, *_ in self.raw}
        self.assertEqual(raw, {"03_kind6.txt", "04_kind9.txt", "05_kind4_effe.bin", "06_kind14_snd_.bin",
                               "13_kind3_wind.bin", "14_kind3_cart.bin", "15_kind3_spec.bin", "16_kind17_spec.bin",
                               "17_kind13_spec.bin", "19_kind2_d_eh.bin"})
        self.assertRegex(self.files["07_plya.txt"].decode(), r"motion: +10015 +# anim 52 _613A variant 2")
        self.assertRegex(self.files["01_tracks.txt"].decode(), r"track 43 +# _600A; motion 10000 NEAR/FORCE_NEAR")

    def test_changing_one_field_changes_exactly_its_bytes(self):
        files = self.edited("07_plya.txt", "  reach:           10.5\n", "  reach:           12.25\n")
        built = self.build(files)
        self.assertEqual(len(built), len(self.squall))
        offset, _ = chunk(self.squall, 1, b"plya")
        field = offset + 8 + 0x28        # record 0, reach
        changed = [index for index in range(len(built)) if built[index] != self.squall[index]]
        self.assertTrue(changed)
        self.assertTrue(all(field <= index < field + 4 for index in changed), changed)
        self.assertEqual(struct.unpack_from("<f", built, field)[0], 12.25)
        again, _ = moveset.render(built, self.neutral)
        self.assertIn("  reach:           12.25\n", again["07_plya.txt"].decode())
        self.assertEqual(moveset.diff(self.squall, built, self.neutral),
                         ["== 07_plya.txt", "  ~ attack  # 0: MAGIC_DEFO", "      - reach:           10.5",
                          "      + reach:           12.25"])

    def test_an_added_event_rebuilds_the_track_and_every_offset_after_it(self):
        line = "  I   30       event_0x15 2410019 0 0"
        files = dict(self.files)
        text = files["01_tracks.txt"].decode()
        start = text.index("\ntrack 43 ")
        end = text.index("\ntrack 44 ")
        files["01_tracks.txt"] = (text[:end] + "\n" + line + "   # added" + text[end:]).encode()
        built = self.build(files)
        old_dir, new_dir = moveset.arc_directory(self.squall)[1], moveset.arc_directory(built)[1]
        self.assertEqual(len(old_dir), len(new_dir))
        for (kind, tag, offset, size), (kind2, tag2, offset2, size2) in zip(old_dir, new_dir):
            self.assertEqual((kind, tag), (kind2, tag2))
            self.assertEqual(offset2 % moveset.alignment(kind), 0)
            if kind == 8:
                self.assertEqual(size2, size + 20)       # 8-byte instant event + three arguments
            else:
                self.assertEqual(built[offset2:offset2 + size2], self.squall[offset:offset + size], (kind, tag))
        _, old_tracks = chunk(self.squall, 8)
        _, new_tracks = chunk(built, 8)
        count = struct.unpack_from("<I", old_tracks, 4)[0]
        old_offsets = struct.unpack_from(f"<{count}I", old_tracks, 8)
        new_offsets = struct.unpack_from(f"<{count}I", new_tracks, 8)
        index = next(i for i, offset in enumerate(old_offsets) if struct.unpack_from("<H", old_tracks, offset)[0] == 43)
        self.assertEqual(new_offsets[:index + 1], old_offsets[:index + 1])
        self.assertEqual(new_offsets[index + 1:], tuple(offset + 20 for offset in old_offsets[index + 1:]))
        anim, ranged, instant, instant_at, size = struct.unpack_from("<HBBHH", old_tracks, old_offsets[index])
        self.assertEqual(struct.unpack_from("<HBBHH", new_tracks, new_offsets[index]),
                         (anim, ranged, instant + 1, instant_at, size + 20))
        # The voice tool, which walks the tracks on its own, finds the new line too.
        says = {(animation, tick, number) for _, number, animation, tick in character_voice.say_events(built)}
        self.assertIn((43, 30, 2410019), says)
        self.assertEqual(len(says), len({(a, t, n) for _, n, a, t in character_voice.say_events(self.squall)}) + 1)
        again, _ = moveset.render(built, self.neutral)
        block = again["01_tracks.txt"].decode()
        block = block[block.index("\ntrack 43 "):block.index("\ntrack 44 ")]
        self.assertIn(line, [moveset._strip_comment(text) for text in block.splitlines()])

    def test_an_added_attack_record_is_counted_and_shifts_what_follows(self):
        record = ('attack  # a new one\n  id: 4000\n  kind: 1\n  motion: 10000\n  reach: 8.0\n  flags: 0x00010006\n'
                  '  name: "NEW_ATTACK"\n')
        files = dict(self.files)
        files["07_plya.txt"] = (files["07_plya.txt"].decode() + "\n" + record).encode()
        built = self.build(files)
        offset, body = chunk(built, 1, b"plya")
        _, old = chunk(self.squall, 1, b"plya")
        count = struct.unpack_from("<I", old, 4)[0]
        self.assertEqual(struct.unpack_from("<I", body, 4)[0], count + 1)
        self.assertEqual(body[:4] + body[8:len(old)], old[:4] + old[8:])
        new = body[len(old):]
        self.assertEqual(len(new), 0x74)
        self.assertEqual(struct.unpack_from("<HxB", new, 0), (4000, 1))
        self.assertEqual(struct.unpack_from("<IH", new, 0x18), (0x00010006, 10000))
        self.assertEqual(struct.unpack_from("<f", new, 0x28)[0], 8.0)
        self.assertEqual(new[0x64:0x74], b"NEW_ATTACK" + bytes(6))
        self.assert_only_chunk_changed(built, 1, b"plya")
        again, _ = moveset.render(built, self.neutral)
        blocks = moveset._record_blocks(again["07_plya.txt"].decode())
        head, lines, note = blocks[-1]
        written = {line.split(":")[0]: line.split(":", 1)[1].strip() for line in lines}
        self.assertEqual((head, note), ("attack", f"{count}: NEW_ATTACK"))
        self.assertEqual({key: written[key] for key in ("id", "kind", "motion", "reach", "flags", "name")},
                         {"id": "4000", "kind": "1", "motion": "10000", "reach": "8.0", "flags": "0x00010006",
                          "name": '"NEW_ATTACK"'})
        self.assertTrue(all(value in ("0", "0.0", "0x00", "0x0000") for key, value in written.items()
                            if key not in ("id", "kind", "motion", "reach", "flags", "name")))
        # A combo can name it at once.
        files["08_comb.txt"] = files["08_comb.txt"].decode().replace(
            "  stage 72 min_frame=0 max_frame=-1 flags=0x0000",
            "  stage 72 min_frame=0 max_frame=-1 flags=0x0000\n  stage 4000 min_frame=12 max_frame=40", 1).encode()
        again, _ = moveset.render(self.build(files), self.neutral)
        self.assertRegex(again["08_comb.txt"].decode(), r"stage 4000 min_frame=12 max_frame=40 flags=0x0000 +# NEW_ATTACK")

    def test_archives_the_other_tools_rewrote_still_come_back(self):
        renamed = character_text.rename(self.squall, {4: "Noble Charge", 7: "Steel Pirouette"})
        self.assertNotEqual(renamed, self.squall)
        renumbered = character_voice.renumber(renamed, 220100)
        for data in (renamed, renumbered):
            files, _ = moveset.render(data, self.neutral)
            self.assertEqual(self.build(files), data)
        files, _ = moveset.render(renamed, self.neutral)
        text = files["12_text.txt"].decode()
        self.assertIn('string 4 "Noble Charge"', text)
        self.assertIn("language 5 share=0", text)

    def test_the_command_line_checks_and_writes(self):
        printed = io.StringIO()
        with tempfile.TemporaryDirectory() as folder, contextlib.redirect_stdout(printed):
            source = Path(folder, "p_eht100.objx")
            source.write_bytes(self.squall)
            self.assertEqual(moveset.main(["check", str(source)]), 0)
            self.assertEqual(moveset.main(["decompile", str(source), str(Path(folder, "out")), "--builtin"]), 0)
            self.assertEqual(moveset.main(["compile", str(Path(folder, "out")), str(Path(folder, "again.objx"))]), 0)
            self.assertEqual(Path(folder, "again.objx").read_bytes(), self.squall)
        self.assertIn("identical", printed.getvalue())


if __name__ == "__main__":
    unittest.main()
