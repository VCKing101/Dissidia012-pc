"""Decompile a fighter's moveset archive into text files a person can edit, and compile them back.

Usage (from the repo root):

    .venv/Scripts/python.exe tools/moveset.py decompile ARCHIVE OUTDIR [--gmo MODEL.gmo] [--builtin]
    .venv/Scripts/python.exe tools/moveset.py compile INDIR ARCHIVE
    .venv/Scripts/python.exe tools/moveset.py check ARCHIVE [ARCHIVE ...]
    .venv/Scripts/python.exe tools/moveset.py diff A B
    .venv/Scripts/python.exe tools/moveset.py sweep [--iso ISO] [--world]

decompile   writes OUTDIR/archive.txt (the archive's chunk list) and one file per chunk:
            text for the moveset tables, a hex block for chunks not yet read field by field,
            the bytes themselves (.bin) for assets with formats of their own (effects, sounds,
            pictures, models), and a folder for an archive nested in the archive.
            --gmo names each animation after its motion in the fighter's model
            (obj/P_<CODE>_1P.gmo). Field and event names come from the semantics notes, of
            which the folder keeps a copy (notes/); --builtin uses only the tool's own names
            (what docs/research/battle-moveset.md establishes, field_0xNN / event_0xNN else).
compile     builds the archive from such a folder. Counts, offsets, alignment and padding are
            worked out again, so records and events may be added, removed or lengthened.
check       decompiles to a temporary folder, compiles, and compares with the original.
diff        lists the differences between two archives record by record.
sweep       checks every fighter's main and assist moveset archive in the game image
            (--world also the world-map ones).

An ARCHIVE argument is a file, or `iso:obj/p_eht100.objx` to read one straight from the game
image in the repo root.

The text format. A file is a list of records. A record starts with a line at the left margin
(`attack`, `track 43`, `motion 10000 anim=43 variant=1`) and goes on with indented lines:
fields (`  name: value`) or the record's items (`  R 16 24 event_0x00 861 48`). Everything after
a `#` is a comment, ignored by the compiler; the decompiler puts what it knows there (the
attack a motion is used by, the name of a hit record...). A field left out is zero. Values:

    1470, -1, 0x05AE          integers, in any base, whatever the field's own display
    10.5, -0.5, 1e-05         floats; a float field also takes 0x3F800000 as the raw bits
    "LAND_NEAR1"              strings, with \\xNN, \\0, \\" and \\\\ escapes
    00 01 FF                  in an array of hex bytes (and hex words), each item is hex

Event arguments are 32-bit words without a declared type: a number with a point or an exponent is
stored as a float, any other number as an integer. The decompiler writes each word the way that
reads best and always the way that gives the same bits back.

Field and event names come from the semantics notes, build/scratch/moveset_semantics/fields.tsv
and events.tsv, when they exist (see load_semantics); without them the fields known from
docs/research/battle-moveset.md are named and the rest are neutral (field_0x1C, event_0x15). A
decompiled folder keeps a copy of the notes it was written with and is compiled by that copy.
Each field also answers to its neutral name and each event to event_0xNN or 0xNN.
"""

import argparse
import difflib
import re
import shutil
import struct
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SEMANTICS = ROOT / "build/scratch/moveset_semantics"
ISO = ROOT / "Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso"

ARC_MAGIC = b"ARC\x01"
MANIFEST = "archive.txt"

# Every fighter code the game has (docs/research/character-assets.md). Chaos (org200) has three
# archives of his own, one per difficulty; Aerith (sev120) only an assist archive.
CODES = ("one100", "one200", "two100", "two200", "thr100", "thr200", "for100", "for200", "fiv100",
         "fiv200", "six100", "six200", "sev100", "sev200", "eht100", "eht200", "nin100", "nin200",
         "ten100", "ten200", "gst100", "gst200", "org200", "for110", "fiv210", "sev110", "eht110",
         "ten110", "gst110", "gst120", "gst130", "org210", "sev120", "org800")


class MovesetError(ValueError):
    """The archive or a text file is not as expected. The message says where."""


_QUIET = []


def warn(message):
    """Something the compiler builds as written but the game may not take."""
    if not _QUIET:
        print(f"warning: {message}", file=sys.stderr)


# Limits the game's code sets (docs/research/battle-moveset.md section 5): the masks of usable
# attacks and of enabled combo entries are 192 and 256 bits wide; the hit event switches collision
# shapes through a 64-bit mask, by shape id (the first byte of a shape record), not by record number.
MAX_ATTACKS = 192
MAX_COMBOS = 256
MAX_HIT_SHAPE = 63
# The highest stock ids, which are unique across the whole game (docs/research/moveset-format.md):
# a new record is safest above these.
STOCK_ID_LIMITS = {"plya": 3035, "atkp": 3355, "magi": 2277}


# ---------------------------------------------------------------------------------------- numbers
def f32_bits(value):
    try:
        return struct.unpack("<I", struct.pack("<f", value))[0]
    except (OverflowError, struct.error):
        raise MovesetError(f"{value!r} does not fit a 32-bit float") from None


def bits_f32(bits):
    return struct.unpack("<f", struct.pack("<I", bits))[0]


def float_text(bits):
    """The shortest decimal that is stored as exactly these bits. Infinities, NaNs and the tiny
    denormal numbers (which are usually integers in a float's place) are written as the bits."""
    exponent = bits >> 23 & 0xFF
    if exponent == 0xFF or (exponent == 0 and bits & 0x7FFFFF):
        return f"0x{bits:08X}"
    value = bits_f32(bits)
    for digits in range(1, 10):
        text = f"{value:.{digits}g}"
        if f32_bits(float(text)) == bits:
            break
    return repr(float(text))        # the same number, as 180.0 rather than 1.8e+02


def auto_text(bits):
    """A 32-bit word of unknown type, written as what it most likely is."""
    if bits < 0x01000000:
        return str(bits)
    if bits >= 0xFFFF0000:
        return str(bits - 0x100000000)
    if 0x66 <= (bits >> 23 & 0xFF) <= 0x98:
        text = float_text(bits)
        digits = text.lstrip("-").split("e")[0].replace(".", "").strip("0")
        if len(digits) <= 6:
            return text
    return f"0x{bits:08X}"


_INT = re.compile(r"[+-]?(0[xX][0-9A-Fa-f]+|\d+)$")
_FLOAT = re.compile(r"[+-]?(\d+\.\d*|\.\d+|\d+(\.\d*)?[eE][+-]?\d+|\.\d+[eE][+-]?\d+|inf|nan)$", re.IGNORECASE)


def parse_int(token, where):
    if not _INT.match(token):
        raise MovesetError(f"{where}: {token!r} is not an integer")
    return int(token, 16) if "x" in token.lower() else int(token, 10)


def parse_float(token, where):
    """Bits of a float field: a number with a point or an exponent, or 0x... for the raw bits.
    A bare integer is refused rather than guessed at."""
    if re.match(r"[+-]?0[xX]", token):
        return fit(parse_int(token, where), 32, False, where)
    if _FLOAT.match(token):
        return f32_bits(float(token))
    if _INT.match(token):
        raise MovesetError(f"{where}: {token} in a float field: write {token}.0 for the number, or 0x{int(token) & 0xFFFFFFFF:08X} for the bits")
    raise MovesetError(f"{where}: {token!r} is not a number")


def parse_auto(token, where):
    """Bits of an untyped 32-bit word: a float if written with a point or an exponent."""
    if _FLOAT.match(token) and not re.match(r"[+-]?0[xX]", token):
        return f32_bits(float(token))
    return fit(parse_int(token, where), 32, False, where)


def fit(value, bits, signed, where):
    """A value that fits `bits` either way (signed or not), as the field stores it."""
    if not -(1 << bits - 1) <= value < 1 << bits:
        raise MovesetError(f"{where}: {value} does not fit {bits} bits")
    if signed:
        return value - (1 << bits) if value >= 1 << bits - 1 else value
    return value & ((1 << bits) - 1)


def string_text(data):
    """A fixed-size string field as a quoted literal: up to its last byte that is not zero."""
    data = bytes(data).rstrip(b"\0")
    out = []
    for byte in data:
        char = chr(byte)
        if char in '"\\':
            out.append("\\" + char)
        elif 0x20 <= byte < 0x7F:
            out.append(char)
        elif byte == 0:
            out.append("\\0")
        else:
            out.append(f"\\x{byte:02X}")
    return '"' + "".join(out) + '"'


def parse_string(token, where):
    if len(token) < 2 or token[0] != '"' or token[-1] != '"':
        raise MovesetError(f"{where}: {token!r} is not a quoted string")
    body, out, index = token[1:-1], bytearray(), 0
    while index < len(body):
        char = body[index]
        if char == "\\":
            code = body[index + 1:index + 2]
            if code == "x":
                out.append(int(body[index + 2:index + 4], 16))
                index += 4
                continue
            out += {"0": b"\0", "n": b"\n", "t": b"\t", '"': b'"', "\\": b"\\"}.get(code, b"")
            if code not in ("0", "n", "t", '"', "\\"):
                raise MovesetError(f"{where}: unknown escape \\{code} in {token}")
            index += 2
            continue
        if ord(char) > 0x7F:
            raise MovesetError(f"{where}: {char!r} is not ASCII; write it as \\xNN")
        out.append(ord(char))
        index += 1
    return bytes(out)


def hex_bytes(tokens, where):
    out = bytearray()
    for token in tokens:
        if len(token) % 2 or not re.fullmatch(r"[0-9A-Fa-f]*", token):
            raise MovesetError(f"{where}: {token!r} is not hex bytes")
        out += bytes.fromhex(token)
    return bytes(out)


def hex_lines(data, keyword="hex", per_line=16, indent=""):
    lines = []
    for start in range(0, len(data), per_line):
        row = data[start:start + per_line]
        lines.append(f"{indent}{keyword} {row.hex(' ').upper():{per_line * 3 - 1}}  # +0x{start:04X}")
    return lines


# ----------------------------------------------------------------------------------------- fields
KINDS = {"u8": (1, "B", False), "s8": (1, "b", True), "x8": (1, "B", False),
         "u16": (2, "H", False), "s16": (2, "h", True), "x16": (2, "H", False),
         "u32": (4, "I", False), "s32": (4, "i", True), "x32": (4, "I", False),
         "f32": (4, "I", False), "a32": (4, "I", False)}
KIND_WORDS = {"u8": "u8", "s8": "s8", "x8": "u8 (hex)", "u16": "u16", "s16": "s16", "x16": "u16 (hex)",
              "u32": "u32", "s32": "s32", "x32": "u32 (hex)", "f32": "float", "a32": "32-bit word", "str": "string"}


def neutral_name(offset):
    return f"field_0x{offset:02X}"


class Field:
    """One field of a fixed-size record. kind is one of KINDS or "str" (count = bytes);
    count > 1 makes an array. ref names what the value points at, for comments only."""

    def __init__(self, offset, kind, name, count=1, ref=None, note="", quiet=False, aliases=()):
        if kind != "str" and kind not in KINDS:
            raise MovesetError(f"Unknown field type {kind!r}")
        self.offset, self.kind, self.name, self.count = offset, kind, name, count
        self.ref, self.note, self.quiet, self.aliases = ref, note, quiet, tuple(aliases)
        self.size = count if kind == "str" else KINDS[kind][0] * count
        self.array = kind != "str" and count > 1

    @property
    def neutral(self):
        return neutral_name(self.offset)

    def describe(self):
        kind = KIND_WORDS[self.kind]
        if self.kind == "str":
            kind = f"string of {self.count} bytes"
        elif self.array:
            kind = f"{self.count} x {kind}"
        return kind

    def read(self, data, base=0):
        start = base + self.offset
        if self.kind == "str":
            return bytes(data[start:start + self.count])
        size, code, _ = KINDS[self.kind]
        values = struct.unpack_from(f"<{self.count}{code}", data, start)
        return list(values) if self.array else values[0]

    def pack(self, value):
        if self.kind == "str":
            return value + bytes(self.count - len(value))
        code = KINDS[self.kind][1]
        values = value if self.array else [value]
        return struct.pack(f"<{self.count}{code}", *values)

    def zero(self):
        if self.kind == "str":
            return b""
        return [0] * self.count if self.array else 0

    def is_zero(self, value):
        return value == self.zero() or (self.kind == "str" and not value.strip(b"\0"))

    def scalar_text(self, value, in_array=False):
        kind = self.kind
        if kind == "f32":
            return float_text(value)
        if kind == "a32":
            return auto_text(value)
        if kind[0] == "x":
            digits = KINDS[kind][0] * 2
            return f"{value:0{digits}X}" if in_array else f"0x{value:0{digits}X}"
        return str(value)

    def text(self, value):
        if self.kind == "str":
            return string_text(value)
        if self.array:
            return " ".join(self.scalar_text(item, True) for item in value)
        return self.scalar_text(value)

    def parse_scalar(self, token, where, in_array=False):
        kind = self.kind
        if kind == "f32":
            return parse_float(token, where)
        if kind == "a32":
            return parse_auto(token, where)
        size, _, signed = KINDS[kind]
        if kind[0] == "x" and in_array and not token.lower().startswith(("0x", "-")):
            token = "0x" + token
        return fit(parse_int(token, where), size * 8, signed, where)

    def parse(self, tokens, where):
        where = f"{where} ({self.name})"
        if self.kind == "str":
            if len(tokens) != 1:
                raise MovesetError(f"{where}: give one quoted string")
            value = parse_string(tokens[0], where)
            if len(value) > self.count:
                raise MovesetError(f"{where}: {len(value)} bytes do not fit {self.count}")
            return value
        if self.array:
            if len(tokens) != self.count:
                raise MovesetError(f"{where}: give {self.count} values, not {len(tokens)}")
            return [self.parse_scalar(token, where, True) for token in tokens]
        if len(tokens) != 1:
            raise MovesetError(f"{where}: give one value, not {len(tokens)}")
        return self.parse_scalar(tokens[0], where)


class Layout:
    """A fixed-size record: named fields, with the bytes nothing names filled in by neutral
    fields (32-bit words where aligned, else 16- and 8-bit hex), so no byte is ever dropped."""

    def __init__(self, title, size, fields):
        self.title, self.size = title, size
        fields = sorted(fields, key=lambda field: field.offset)
        filled, position = [], 0
        for field in fields + [None]:
            end = size if field is None else field.offset
            if end < position:
                raise MovesetError(f"{title}: field {field.name} overlaps the one before it")
            while position < end:
                if position % 4 == 0 and end - position >= 4:
                    filled.append(Field(position, "a32", neutral_name(position)))
                elif position % 2 == 0 and end - position >= 2:
                    filled.append(Field(position, "x16", neutral_name(position)))
                else:
                    filled.append(Field(position, "x8", neutral_name(position)))
                position = filled[-1].offset + filled[-1].size
            if field is not None:
                if field.offset + field.size > size:
                    raise MovesetError(f"{title}: field {field.name} runs past the record's {size} bytes")
                filled.append(field)
                position = field.offset + field.size
        self.fields = filled
        self.names = {}
        for field in filled:
            for name in (field.name, field.neutral, *field.aliases):
                if self.names.get(name, field) is not field:
                    raise MovesetError(f"{title}: two fields are called {name}")
                self.names[name] = field

    def field(self, name):
        return self.names[name]

    def unpack(self, data, base=0):
        if base + self.size > len(data):
            raise MovesetError(f"A {self.title} record is cut short")
        return [field.read(data, base) for field in self.fields]

    def pack(self, values):
        return b"".join(field.pack(value) for field, value in zip(self.fields, values))

    def value(self, values, name):
        return values[self.fields.index(self.names[name])]

    def at(self, values, offset):
        """The value of the field that starts at `offset`, whatever it is called (None if none does)."""
        for field, value in zip(self.fields, values):
            if field.offset == offset:
                return value
        return None

    def assign(self, given, where):
        """Field values from {name: (tokens, line number)}; what is not given is zero."""
        values = [field.zero() for field in self.fields]
        seen = {}
        for name, (tokens, line) in given.items():
            field = self.names.get(name)
            if field is None:
                raise MovesetError(f"{where}, line {line}: the {self.title} has no field {name}")
            if field in seen:
                raise MovesetError(f"{where}, line {line}: {field.name} is given twice (also on line {seen[field]})")
            seen[field] = line
            values[self.fields.index(field)] = field.parse(tokens, f"{where}, line {line}")
        return values

    def glossary(self, keyword, skip=()):
        lines = [f"# Fields of each `{keyword}` ({self.title}, {self.size} bytes):"]
        for field in self.fields:
            if field.name in skip:
                continue
            note = f"  {field.note}" if field.note else ""
            lines.append(f"#   +0x{field.offset:02X} {field.name:22s} {field.describe()}{note}")
        return lines

    def block(self, keyword, values, head="", comments=None, skip=()):
        """A record written one field to a line."""
        comments = comments or {}
        lines = [keyword + (f"  # {head}" if head else "")]
        shown = [(field, value) for field, value in zip(self.fields, values) if field.name not in skip]
        width = max([len(field.name) for field, _ in shown] + [1]) + 1
        for field, value in shown:
            line = f"  {field.name + ':':{width}} {field.text(value)}"
            if comments.get(field.name):
                line = f"{line:40s}  # {comments[field.name]}"
            lines.append(line)
        return lines

    def line(self, keyword, values, first=True, skip=()):
        """A record written on one line: the first field's value, then name=value for each other
        field that is not zero (or is not quiet)."""
        parts = [keyword]
        for index, (field, value) in enumerate(zip(self.fields, values)):
            if field.name in skip:
                continue
            if index == 0 and first:
                parts.append(field.text(value))
            elif not (field.quiet and field.is_zero(value)):
                parts.append(f"{field.name}={field.text(value)}")
        return " ".join(parts)


# ------------------------------------------------------------------------------------ text format
def split_tokens(line, where):
    """Tokens of a line, comments dropped. A quoted string stays one token, quotes and all, also
    inside name=value."""
    tokens, current, index, quoted = [], "", 0, False
    while index < len(line):
        char = line[index]
        if quoted:
            current += char
            if char == "\\" and index + 1 < len(line):
                current += line[index + 1]
                index += 1
            elif char == '"':
                quoted = False
        elif char == '"':
            current += char
            quoted = True
        elif char == "#":
            break
        elif char in " \t\r":
            if current:
                tokens.append(current)
            current = ""
        else:
            current += char
        index += 1
    if quoted:
        raise MovesetError(f"{where}: a string is not closed")
    if current:
        tokens.append(current)
    return tokens


class Statement:
    def __init__(self, line, tokens):
        self.line, self.tokens = line, tokens
        self.positional = [token for token in tokens if not _is_pair(token)]
        self.pairs = {}
        for token in tokens:
            if _is_pair(token):
                name, value = token.split("=", 1)
                if name in self.pairs:
                    raise MovesetError(f"line {line}: {name} is given twice")
                self.pairs[name] = ([value], line)


def _is_pair(token):
    return not token.startswith('"') and "=" in token and token.index("=") > 0


class Record:
    """A line at the left margin with its indented lines: fields (name: value) and items."""

    def __init__(self, head):
        self.head, self.fields, self.items = head, {}, []

    @property
    def keyword(self):
        return self.head.tokens[0]

    @property
    def line(self):
        return self.head.line

    def all_fields(self):
        merged = dict(self.head.pairs)
        for name, value in self.fields.items():
            if name in merged:
                raise MovesetError(f"line {value[1]}: {name} is given twice")
            merged[name] = value
        return merged


def parse_records(text, where):
    records = []
    for number, raw in enumerate(text.splitlines(), 1):
        tokens = split_tokens(raw, f"{where}, line {number}")
        if not tokens:
            continue
        if raw[:1] not in (" ", "\t"):
            records.append(Record(Statement(number, tokens)))
            continue
        if not records:
            raise MovesetError(f"{where}, line {number}: an indented line comes before any record")
        record = records[-1]
        first = tokens[0]
        if not first.startswith('"') and ":" in first:
            name, rest = first.split(":", 1)
            values = ([rest] if rest else []) + tokens[1:]
            if name in record.fields:
                raise MovesetError(f"{where}, line {number}: {name} is given twice")
            record.fields[name] = (values, number)
        else:
            record.items.append(Statement(number, tokens))
    return records


def take_format(records, name, where):
    if not records or records[0].keyword != "format":
        raise MovesetError(f"{where}: the file does not start with `format {name}`")
    if records[0].head.tokens[1:] != [name]:
        raise MovesetError(f"{where}: this is format {' '.join(records[0].head.tokens[1:])}, not {name}")
    return records[1:]


def expect(record, keyword, where):
    if record.keyword != keyword:
        raise MovesetError(f"{where}, line {record.line}: expected `{keyword}`, found `{record.keyword}`")


def no_items(record, where):
    if record.items:
        raise MovesetError(f"{where}, line {record.items[0].line}: a {record.keyword} has no items")


# ------------------------------------------------------------------------------------- semantics
class Semantics:
    """Names and meanings of fields and events: the tool's own (what docs/research/battle-moveset.md
    establishes; the rest neutral), overlaid with the semantics notes when `texts` holds them
    ({"fields.tsv": text, "events.tsv": text}, the format load_semantics describes). Rows of the
    notes that cannot be used are listed in `skipped`, with the reason."""

    def __init__(self, texts=None):
        self.fields = {key: [Field(*spec[:3], **spec[3]) if len(spec) > 3 else Field(*spec)
                             for spec in specs] for key, specs in DEFAULT_FIELDS.items()}
        self.events = {}        # ("R" or "I", type) -> {"name", "args": [(name, kind)], "note"}
        self.texts = {name: text for name, text in (texts or {}).items() if text is not None}
        self.sources = []       # the notes files read, when loaded from a folder (load_semantics)
        self.skipped = []
        if "fields.tsv" in self.texts:
            self._overlay_fields(self.texts["fields.tsv"])
        if "events.tsv" in self.texts:
            self._read_events(self.texts["events.tsv"])

    @property
    def mode(self):
        return "notes" if self.texts else "builtin"

    def layout(self, key):
        title, size = RECORD_SHAPES[key]
        return Layout(title, size, self.fields.get(key, []))

    def event_name(self, which, kind):
        info = self.events.get((which, kind))
        return info["name"] if info and info.get("name") else f"event_0x{kind:02X}"

    def event_type(self, which, token, where):
        lowered = token.lower()
        for prefix in ("event_0x", "0x"):
            if lowered.startswith(prefix):
                return fit(int(lowered[len(prefix):], 16), 8, False, where)
        if token.isdigit():
            return fit(int(token), 8, False, where)
        for (side, kind), info in self.events.items():
            if side == which and info.get("name") == token:
                return kind
        raise MovesetError(f"{where}: unknown event type {token!r} (write it as 0xNN)")

    def _overlay_fields(self, text):
        """The notes' fields win over the tool's own wherever they overlap, except over the
        fields the tool reads or works out itself (PROTECTED, the counts)."""
        taken = {}
        for number, row in enumerate(_columns(text), 2):
            where = f"fields.tsv line {number}"
            key = _RECORD_WORDS.get(_norm(_pick(row, "record", "chunk", "table", "struct")))
            name = _norm(_pick(row, "name", "field"))
            typed = _type_of(_pick(row, "type", "size"))
            try:
                offset = int(_pick(row, "offset", "off"), 0)
            except ValueError:
                self.skipped.append(f"{where}: no offset")
                continue
            if not key or not name or not typed:
                self.skipped.append(f"{where}: record, name or type not understood")
                continue
            kind, count = typed
            if re.fullmatch(r"field_0x[0-9a-f]+", name):
                name = neutral_name(offset)
            status, meaning = _pick(row, "status"), _pick(row, "meaning", "description", "note", "notes", "comment")
            note = (f"[{status}] " if status else "") + meaning
            new = Field(offset, kind, name, count=count, note=note if len(note) <= 100 else note[:99] + "~")
            size = RECORD_SHAPES[key][1]
            others = taken.setdefault(key, [])
            protected = [field for field in self.fields.get(key, []) if field.name in PROTECTED.get(key, ())]
            if offset + new.size > size:
                self.skipped.append(f"{where}: {name} runs past the {size}-byte record")
            elif any(_overlap(new, field) for field in others):
                self.skipped.append(f"{where}: {name} overlaps a field named on an earlier row")
            elif any(_overlap(new, field) for field in protected):
                same = [field for field in protected if (field.offset, field.size, field.name) == (offset, new.size, name)]
                if not same:        # naming a counted field what the tool calls it is no news
                    self.skipped.append(f"{where}: {name} overlaps a field the tool works out itself")
            elif any(field.name == name for field in others) and not name.startswith("field_0x"):
                self.skipped.append(f"{where}: a second field called {name}")
            else:
                others.append(new)
        for key, news in taken.items():
            defaults = self.fields.get(key, [])
            kept = [field for field in defaults if not any(_overlap(field, new) for new in news)]
            names = {field.name for field in kept + news} | {field.neutral for field in kept + news}
            for new in news:
                old = next((field for field in defaults if field.offset == new.offset), None)
                if old is not None:
                    new.ref, new.quiet = old.ref, old.quiet
                    # The name it replaces stays good, so files written before the notes compile.
                    new.aliases = tuple(alias for alias in (old.name, *old.aliases)
                                        if alias not in names and not alias.startswith("field_0x"))
                if new.ref is None and new.kind not in ("f32", "str") and key != "motion":
                    if new.name.endswith("motion"):
                        new.ref = "motion"
                    elif "atkp" in new.name:
                        new.ref = "atkp"
                    elif "banner" in new.name:
                        new.ref = "text"
            self.fields[key] = kept + news
            Layout(RECORD_SHAPES[key][0], RECORD_SHAPES[key][1], self.fields[key])

    def _read_events(self, text):
        for number, row in enumerate(_columns(text), 2):
            side = _pick(row, "side", "list", "kind", "track").strip().lower()[:1].upper()
            try:
                kind = int(_pick(row, "type", "id", "event"), 0)
            except ValueError:
                kind = None
            if side not in ("R", "I") or kind is None or not 0 <= kind <= 0xFF:
                if not _pick(row, "type", "id", "event").lstrip().startswith((">", "<")):    # a range of types
                    self.skipped.append(f"events.tsv line {number}: side or type not understood")
                continue
            name = _norm(_pick(row, "name"))
            if name and (re.fullmatch(r"(event_)?0x[0-9a-f]+", name) or name in ("r", "i")):
                name = ""
            if name and any(info["name"] == name for (other, _), info in self.events.items() if other == side):
                self.skipped.append(f"events.tsv line {number}: a second {side} event called {name}")
                name = ""
            args = []
            for part in re.split(r"[,;]", _pick(row, "args", "arguments", "parameters")):
                part = part.strip()
                if part:
                    arg_name, _, arg_type = part.partition(":")
                    typed = _type_of(arg_type) if arg_type else None
                    args.append((_norm(arg_name) or part, typed[0] if typed and typed[0] != "str" else None))
            self.events[(side, kind)] = {"name": name, "args": args,
                                         "note": _pick(row, "meaning", "description", "note", "notes")[:100]}


def _overlap(one, other):
    return one.offset < other.offset + other.size and other.offset < one.offset + one.size


RECORD_SHAPES = {
    "plya": ("attack record", 0x74), "plya.header": ("attack table header", 8),
    "atkp": ("hit record", 0x44), "atkp.header": ("hit table header", 8),
    "comb": ("combo entry", 8), "comb.stage": ("combo stage", 8), "comb.header": ("combo table header", 4),
    "magi": ("projectile record", 0xCC), "magi.header": ("projectile table header", 16),
    "brat": ("AI attack group", 28), "brat.header": ("AI table header", 20),
    "motion": ("motion table entry", 8), "motion.header": ("motion table header", 8),
    "shape": ("collision shape", 0x20), "shape.header": ("collision shape table header", 16),
    "ranged": ("ranged event", 12), "instant": ("instant event", 8),
    "tracks.header": ("event track chunk header", 8),
}

# What docs/research/battle-moveset.md establishes, as (offset, type, name[, options]).
DEFAULT_FIELDS = {
    "plya.header": [(0, "u32", "version"), (4, "u32", "count")],
    "plya": [(0x00, "u16", "id"),
             (0x03, "u8", "kind", {"note": "0 opener, 1 follow-up, 2 chase, 3 counter-chase (by name)"}),
             (0x05, "u8", "self_position", {"note": "ground/air requirement of the attacker (main 0x08847F10)"}),
             (0x06, "u8", "target_position", {"note": "ground/air requirement of the target"}),
             (0x07, "u8", "field_0x07", {"note": "handed to main 0x088A9DE8 when above 0"}),
             (0x08, "s16", "field_0x08"), (0x0A, "s16", "field_0x0A"), (0x0C, "s16", "field_0x0C"),
             (0x0F, "x8", "slot_class", {"note": "bits 1/2/4, 0x80 family, 8 = class 6 (main 0x08848160)"}),
             (0x10, "s32", "field_0x10"),
             (0x18, "x32", "flags", {"note": "bit 0: approach the target first"}),
             (0x1C, "u16", "motion", {"ref": "motion", "note": "motion id, through the motion table"}),
             (0x1E, "s16", "motion2", {"ref": "motion", "note": "second motion id"}),
             (0x21, "s8", "banner", {"ref": "text", "note": "HUD name banner: string index in the text chunk, -1 none"}),
             (0x24, "f32", "field_0x24"),
             (0x28, "f32", "reach", {"note": "distance the attack is chosen from"}),
             (0x2C, "f32", "height_min", {"note": "height difference window"}),
             (0x30, "f32", "height_max"),
             (0x34, "f32", "field_0x34"), (0x38, "f32", "field_0x38"), (0x3C, "f32", "field_0x3C"),
             (0x40, "f32", "field_0x40", {"note": "+0x40..+0x5C approach parameters (battle 0x08A60BC8)"}),
             (0x44, "f32", "field_0x44"), (0x48, "f32", "field_0x48"), (0x4C, "f32", "field_0x4C"),
             (0x50, "f32", "field_0x50"), (0x54, "f32", "field_0x54"), (0x58, "f32", "field_0x58"),
             (0x5C, "f32", "field_0x5C"),
             (0x60, "x8", "field_0x60"), (0x61, "x8", "field_0x61"), (0x62, "x8", "field_0x62"),
             (0x63, "x8", "field_0x63"),
             (0x64, "str", "name", {"count": 16, "note": "debug name; LIMIT_START is looked up by name"})],
    "atkp.header": [(0, "u32", "version"), (4, "u32", "count")],
    "atkp": [(0x00, "u32", "id"),
             (0x18, "f32", "field_0x18"), (0x1C, "f32", "field_0x1C"), (0x20, "f32", "field_0x20"),
             (0x24, "f32", "field_0x24"), (0x28, "f32", "field_0x28"),
             (0x34, "str", "name", {"count": 16})],
    "comb.header": [(0, "u16", "version"), (2, "s16", "count")],
    "comb": [(0, "u16", "command", {"note": "ability command id (system.bin ablt +0x0C)"}),
             (2, "s16", "stages"), (4, "s16", "alt_command"), (6, "s16", "prerequisite")],
    "comb.stage": [(0, "u16", "attack", {"ref": "plya", "note": "attack record id"}),
                   (2, "s16", "min_frame", {"note": "follow-up accepted from this frame of the stage before"}),
                   (4, "s16", "max_frame", {"note": "... to this one (-1: to the end)"}),
                   (6, "x16", "flags")],
    "magi.header": [(0, "str", "magic", {"count": 4}), (4, "x32", "version"), (8, "u32", "count"),
                    (12, "u32", "floats")],
    "magi": [(0x00, "u16", "id"), (0x02, "u16", "bullet_class", {"note": "4 = class B (battle 0x08AD43B4)"}),
             (0x04, "u16", "behaviour", {"note": "projectile behaviour type"}),
             (0x06, "s16", "field_0x06"), (0x08, "u16", "field_0x08"),
             (0x0A, "s16", "field_0x0A", {"count": 10, "ref": "atkp", "note": "hit records (-1: none)"}),
             (0x1E, "u16", "field_0x1E"), (0x20, "u16", "field_0x20"), (0x22, "u16", "field_0x22"),
             (0x24, "u32", "field_0x24"), (0x28, "u32", "field_0x28"),
             (0x2C, "f32", "field_0x2C"), (0x30, "f32", "field_0x30"), (0x34, "f32", "field_0x34"),
             (0x38, "x8", "field_0x38", {"count": 8}),
             (0x40, "x8", "field_0x40", {"count": 0x8A}),
             (0xCA, "x16", "field_0xCA")],
    "brat.header": [(0, "str", "magic", {"count": 4}), (4, "f32", "version"), (8, "a32", "field_0x08"),
                    (12, "a32", "field_0x0C"), (16, "u32", "count")],
    "brat": [(0, "u32", "command", {"note": "combo command id"}),
             (4, "u16", "field_0x04", {"count": 12})],
    "motion.header": [(0, "u32", "version"), (4, "u32", "count")],
    "motion": [(0, "u16", "motion", {"note": "motion id (shared vocabulary; attacks use 10000 up)"}),
               (2, "u16", "anim", {"note": "animation index in the fighter's model"}),
               (4, "u8", "variant", {"note": "compared with an event's variant condition"}),
               (5, "x8", "field_0x05", {"quiet": True}), (6, "x16", "field_0x06", {"quiet": True})],
    "shape.header": [(0, "x32", "version"), (4, "a32", "field_0x04"), (8, "u32", "count"),
                     (12, "a32", "field_0x0C")],
    # A shape is switched on by id: every record whose first byte equals the id (main 0x08837D18 tests
    # *(u8 *)record; several records may share one id). Its sphere is placed by main 0x08812664: the u16 at
    # +0x06 (bone, with 0x4000 from flags 0x40) picks the frame (main 0x088369a4), (x, y, z) is the offset.
    "shape": [(0x00, "u32", "id", {"note": "shape id: hit (R 0x00) and shape_on/off events name it; matched against byte 0"}),
              (0x04, "u8", "class", {"note": "which list: 0 body, 1-3 hurt, 4 attack, 5/6 clash..."}),
              (0x05, "u8", "kind", {"note": "3 = sphere"}),
              (0x06, "u8", "bone", {"note": "model bone the sphere follows (with flags 0x40 and bone 0: the character's own frame)"}),
              (0x07, "x8", "shape_flags", {"note": "0x40: bone value | 0x4000 (0x4000 alone = character frame)"}),
              (0x08, "f32", "x"), (0x0C, "f32", "y"), (0x10, "f32", "z"),
              (0x14, "f32", "w", {"note": "subtracted from the centre's y"}),
              (0x18, "f32", "radius"), (0x1C, "f32", "radius2")],
    "tracks.header": [(0, "u32", "version"), (4, "u32", "count")],
    "ranged": [(0, "u16", "start"), (2, "u16", "end"), (4, "u8", "type"), (5, "u8", "argc"),
               (6, "u8", "field_0x06"), (7, "u8", "field_0x07"), (8, "x32", "field_0x08")],
    "instant": [(0, "u16", "tick"), (2, "u8", "type"), (3, "u8", "argc"),
                (4, "u8", "field_0x04"), (5, "u8", "field_0x05"), (6, "x16", "field_0x06")],
}
# The event lines' own columns (the other header fields are written as name=value).
EVENT_HEAD = {"ranged": ("start", "end", "type", "argc"), "instant": ("tick", "type", "argc")}
# Fields the tool reads by name or works out itself: the semantics notes cannot rename them.
PROTECTED = {"comb": ("stages",), **EVENT_HEAD}


def _norm(text):
    return re.sub(r"[^a-z0-9]+", "_", text.strip().lower()).strip("_")


_TYPE_WORDS = {"u8": "u8", "uint8": "u8", "byte": "u8", "s8": "s8", "int8": "s8", "u16": "u16", "uint16": "u16",
               "s16": "s16", "int16": "s16", "u32": "u32", "uint32": "u32", "s32": "s32", "int32": "s32",
               "f32": "f32", "float": "f32", "x8": "x8", "x16": "x16", "x32": "x32", "hex8": "x8",
               "hex16": "x16", "hex32": "x32", "flags8": "x8", "flags16": "x16", "flags32": "x32",
               "char": "str", "str": "str", "string": "str", "a32": "a32", "word": "a32", "id": "u32"}
_RECORD_WORDS = {"plya": "plya", "attack": "plya", "atkp": "atkp", "hit": "atkp", "comb": "comb",
                 "combo": "comb", "comb_entry": "comb", "comb_stage": "comb.stage", "stage": "comb.stage",
                 "magi": "magi", "projectile": "magi", "brat": "brat", "motion": "motion", "kind7": "motion",
                 "motion_table": "motion", "shape": "shape", "kind5": "shape", "collision": "shape",
                 "ranged": "ranged", "ranged_event": "ranged", "instant": "instant", "instant_event": "instant"}


def _columns(text):
    rows = [line.split("\t") for line in text.splitlines() if line.strip() and not line.startswith("#")]
    if not rows:
        return []
    head = [_norm(cell) for cell in rows[0]]
    return [dict(zip(head, [cell.strip() for cell in row])) for row in rows[1:]]


def _pick(row, *names):
    for name in names:
        if row.get(name):
            return row[name]
    return ""


def _type_of(text):
    """(kind, count) from a type column such as u16, f32, char[16], u8[8], s16 x 10."""
    match = re.match(r"\s*([A-Za-z0-9_]+)\s*(?:\[\s*(\d+)\s*\]|x\s*(\d+))?", text or "")
    if not match:
        return None
    kind = _TYPE_WORDS.get(match.group(1).lower())
    count = int(match.group(2) or match.group(3) or 1)
    return (kind, count) if kind else None


NOTES = ("fields.tsv", "events.tsv")


def load_semantics(folder=SEMANTICS):
    """The tool's own names overlaid with fields.tsv and events.tsv from `folder`, when there.

    fields.tsv: a header row, then one row per field. Columns used (by name; case and spacing
    ignored): record (plya, atkp, comb, comb_stage, magi, brat, motion, shape, ranged, instant),
    offset (0x1C), type (u8 s8 x8 u16 s16 x16 u32 s32 x32 f32 a32, char[16], u8[8]...), name,
    status and meaning. A row that runs past its record, overlaps an earlier row or a field the
    tool works out itself (counts, the event lines' columns) is skipped (Semantics.skipped).
    events.tsv: side (R or I), type (0x15), name, args (comma separated, each name:type), meaning."""
    if folder is None:
        return Semantics()
    folder = Path(folder)
    paths = [folder / name for name in NOTES if (folder / name).is_file()]
    semantics = Semantics({path.name: path.read_text(encoding="utf-8") for path in paths})
    semantics.sources = paths
    return semantics


# ---------------------------------------------------------------------------------------- archive
def arc_directory(data, where="archive"):
    if data[:4] != ARC_MAGIC or len(data) < 16:
        raise MovesetError(f"{where} is not an ARC archive")
    count, word8, word12 = struct.unpack_from("<III", data, 4)
    if 16 + count * 16 > len(data):
        raise MovesetError(f"{where}: the directory is cut short")
    chunks = []
    for index in range(count):
        kind, tag, offset, size = struct.unpack_from("<I4sII", data, 16 + index * 16)
        if offset + size > len(data):
            raise MovesetError(f"{where}: chunk {index} lies outside the archive")
        chunks.append((kind, tag, offset, size))
    return (word8, word12), chunks


def alignment(kind):
    """Where the game's packer starts a chunk of each kind (measured on every retail archive):
    sound banks on 64 bytes, pictures and models on 16, everything else on 4."""
    return {14: 64, 3: 16, 2: 16}.get(kind, 4)


def chunk_label(kind, tag):
    known = {7: "motions", 8: "tracks", 5: "shapes", 6: "kind6", 9: "kind9"}
    if kind in known:
        return known[kind]
    text = re.sub(r"[^A-Za-z0-9_]", "_", tag.decode("latin-1"))
    return f"{text}" if kind == 1 else f"kind{kind}_{text}"


class Context:
    """What one chunk's comments may say about the others: names of attacks and hit records,
    the motion table, the model's animation names."""

    def __init__(self, data, chunks, semantics, gmo_names=None):
        self.semantics = semantics
        self.gmo = list(gmo_names or [])
        self.plya, self.atkp, self.magi, self.motions = {}, {}, {}, {}
        self.anim_motions, self.motion_attacks, self.strings = {}, {}, []
        for kind, tag, offset, size in chunks:
            body = data[offset:offset + size]
            try:
                if kind == 1 and tag == b"plya":
                    for record in range(struct.unpack_from("<I", body, 4)[0]):
                        base = 8 + record * 0x74
                        number, motion = struct.unpack_from("<H", body, base)[0], struct.unpack_from("<H", body, base + 0x1C)[0]
                        name = body[base + 0x64:base + 0x74].split(b"\0")[0].decode("latin-1")
                        self.plya.setdefault(number, name)
                        self.motion_attacks.setdefault(motion, []).append(name)
                elif kind == 1 and tag == b"atkp":
                    for record in range(struct.unpack_from("<I", body, 4)[0]):
                        base = 8 + record * 0x44
                        self.atkp.setdefault(struct.unpack_from("<I", body, base)[0],
                                             body[base + 0x34:base + 0x44].split(b"\0")[0].decode("latin-1"))
                elif kind == 1 and tag == b"magi":
                    for record in range(struct.unpack_from("<I", body, 8)[0]):
                        self.magi.setdefault(struct.unpack_from("<H", body, 16 + record * 0xCC)[0], record)
                elif kind == 1 and tag == b"text":
                    import character_text
                    self.strings = character_text.read(body)
                elif kind == 7:
                    for record in range(struct.unpack_from("<I", body, 4)[0]):
                        motion, anim, variant = struct.unpack_from("<HHB", body, 8 + record * 8)
                        self.motions.setdefault(motion, (anim, variant))
                        self.anim_motions.setdefault(anim, []).append((motion, variant))
            except (struct.error, ValueError):
                pass

    def anim_name(self, anim):
        return self.gmo[anim] if 0 <= anim < len(self.gmo) else ""

    def motion_note(self, motion):
        if motion not in self.motions:
            return "not in the motion table" if motion not in (0, 0xFFFF) else ""
        anim, variant = self.motions[motion]
        note = f"anim {anim} {self.anim_name(anim)}".rstrip()
        return note + (f" variant {variant}" if variant else "")

    def ref_note(self, ref, value):
        if ref == "motion":
            return self.motion_note(value & 0xFFFF)
        if ref == "plya" and value in self.plya:
            return self.plya[value]
        if ref == "atkp" and value in self.atkp:
            return f"atkp {value} {self.atkp[value]}"
        if ref == "magi" and value in self.magi:
            return f"magi {value}"
        if ref == "text" and 0 <= value < len(self.strings) and isinstance(self.strings[value], str):
            return f"text {value} \"{self.strings[value]}\""
        return ""

    def notes(self, layout, values):
        out = {}
        for field, value in zip(layout.fields, values):
            if not field.ref:
                continue
            items = value if field.array else [value]
            words = [self.ref_note(field.ref, item) for item in items]
            words = [word for word in words if word]
            if words:
                out[field.name] = ", ".join(words)
        return out


# ------------------------------------------------------------------------------------------ codecs
class Codec:
    """Turns one chunk's bytes into text and back. name is what the file's `format` line says."""
    name = ""
    suffix = ".txt"

    def decode(self, data, context):
        raise NotImplementedError

    def encode(self, text, where, semantics):
        raise NotImplementedError


def _header_lines(title, about):
    return [f"# {title}", *(f"# {line}" if line else "#" for line in about)]


class TableCodec(Codec):
    """A header with a record count, then fixed-size records (attacks, hit records, shapes,
    the motion table), then for some chunks a tail of their own (tail_text, tail_bytes)."""
    counted = ("count",)

    def __init__(self, name, key, keyword, title, about, line_style=False, note=None):
        self.name, self.key, self.keyword, self.title, self.about = name, key, keyword, title, about
        self.line_style, self.note = line_style, note

    def layouts(self, semantics):
        return semantics.layout(self.key + ".header"), semantics.layout(self.key)

    def decode(self, data, context):
        header, record = self.layouts(context.semantics)
        head = header.unpack(data)
        count = header.value(head, "count")
        end = header.size + count * record.size
        if end > len(data):
            raise MovesetError(f"{self.title}: {count} records do not fit {len(data)} bytes")
        lines = [f"format {self.name}", *_header_lines(self.title, self.about)]
        lines += record.glossary(self.keyword) + ["", *header.block("header", head, skip=self.counted), ""]
        for index in range(count):
            values = record.unpack(data, header.size + index * record.size)
            notes = context.notes(record, values)
            label = self.note(values, record, context) if self.note else ""
            head_note = f"{index}: {label}" if label else f"{index}"
            if self.line_style:
                line = record.line(self.keyword, values)
                lines.append(f"{line:48s}  # " + "; ".join([head_note] + list(notes.values())))
            else:
                lines += record.block(self.keyword, values, head_note, notes) + [""]
        lines += self.tail_text(data[end:], header, head)
        return "\n".join(lines).rstrip() + "\n"

    def tail_text(self, rest, header, head):
        if rest:
            raise MovesetError(f"{self.title}: {len(rest)} bytes after the records")
        return []

    def check(self, records, record, where):
        """Warnings about records the game may not take."""
        if self.key == "plya" and len(records) > MAX_ATTACKS:
            warn(f"{where}: {len(records)} attack records; the game keeps a mask of {MAX_ATTACKS}")
        if self.key in STOCK_ID_LIMITS:
            ids = [record.at(values, 0) for values in records]
            repeated = sorted({number for number in ids if ids.count(number) > 1})
            if repeated and self.key != "plya":       # stock attack tables repeat ids (1972)
                warn(f"{where}: id {repeated[0]} is used by more than one {self.keyword}")
            if self.key in ("atkp", "magi") and any(b < a for a, b in zip(ids, ids[1:])):
                bad = next(b for a, b in zip(ids, ids[1:]) if b < a)
                warn(f"{where}: {self.keyword} {bad} is out of order; the game binary-searches "
                     f"these by id, so keep them sorted")
        if self.key == "motion":
            motions = [record.at(values, 0) for values in records]
            for index in range(1, len(motions)):
                if motions[index] <= motions[index - 1]:
                    warn(f"{where}: motion {motions[index]} comes after {motions[index - 1]}; the game "
                         "binary-searches this table, so keep it sorted and without repeats")
                    break

    def tail_bytes(self, records, header, head, where):
        """The bytes after the records, from the records of the file that follow them. May set
        header fields (head) before the header is packed."""
        if records:
            raise MovesetError(f"{where}, line {records[0].line}: expected `{self.keyword}`, found `{records[0].keyword}`")
        return b""

    def encode(self, text, where, semantics):
        header, record = self.layouts(semantics)
        records = take_format(parse_records(text, where), self.name, where)
        if not records or records[0].keyword != "header":
            raise MovesetError(f"{where}: the first record must be `header`")
        given = records[0].all_fields()
        for name in self.counted:
            for key in (name, header.field(name).neutral):
                if key in given:
                    raise MovesetError(f"{where}, line {given[key][1]}: the header's {name} is worked out, not given")
        head = header.assign(given, where)
        no_items(records[0], where)
        body, index = [], 1
        while index < len(records) and records[index].keyword == self.keyword:
            item = records[index]
            fields = item.all_fields()
            if self.line_style:
                if len(item.head.positional) > 2:
                    raise MovesetError(f"{where}, line {item.line}: write the other fields as name=value")
                if len(item.head.positional) == 2:
                    first = record.fields[0]
                    if first.name in fields or first.neutral in fields:
                        raise MovesetError(f"{where}, line {item.line}: {first.name} is given twice")
                    fields[first.name] = ([item.head.positional[1]], item.line)
            elif len(item.head.tokens) > 1:
                raise MovesetError(f"{where}, line {item.line}: a {self.keyword}'s fields go on lines of their own")
            no_items(item, where)
            body.append(record.assign(fields, where))
            index += 1
        self.check(body, record, where)
        body = [record.pack(values) for values in body]
        head[header.fields.index(header.field("count"))] = len(body)
        tail = self.tail_bytes(records[index:], header, head, where)
        return header.pack(head) + b"".join(body) + tail


def _name_at(values, layout, offset):
    value = layout.at(values, offset)
    return bytes(value).split(b"\0")[0].decode("latin-1") if isinstance(value, bytes) else ""


def _attack_note(values, layout, context):
    return _name_at(values, layout, 0x64)


def _hit_note(values, layout, context):
    return _name_at(values, layout, 0x34)


def _motion_note(values, layout, context):
    motion, anim = layout.at(values, 0), layout.at(values, 2)
    if not isinstance(motion, int) or not isinstance(anim, int):
        return ""
    words = [context.anim_name(anim)] + context.motion_attacks.get(motion, [])
    return " ".join(word for word in words if word)


class MagiCodec(TableCodec):
    """Projectiles: MAGI header, 0xCC-byte records, then a pool of floats the records index."""
    counted = ("count", "floats")

    def __init__(self):
        super().__init__("magi", "magi", "projectile", "Projectile definitions (chunk kind 1, tag magi)",
                         ["Run by battle-overlay code (battle 0x08AD0978): 0xCC-byte records, then a pool of",
                          "floats (`pool` lines). The record and float counts are worked out by the compiler."],
                         note=lambda values, layout, context: f"id {layout.at(values, 0)}")

    def tail_text(self, rest, header, head):
        floats = header.value(head, "floats")
        if len(rest) != floats * 4:
            raise MovesetError(f"projectiles: the float pool should be {floats * 4} bytes, not {len(rest)}")
        words = struct.unpack(f"<{floats}I", rest)
        lines = ["", "# The float pool, eight to a line; the comment is the index of the first."]
        for start in range(0, floats, 8):
            lines.append(f"pool {' '.join(float_text(word) for word in words[start:start + 8])}  # {start}")
        return lines

    def tail_bytes(self, records, header, head, where):
        pool = bytearray()
        for record in records:
            expect(record, "pool", where)
            no_items(record, where)
            for token in record.head.tokens[1:]:
                pool += struct.pack("<I", parse_float(token, f"{where}, line {record.line}"))
        head[header.fields.index(header.field("floats"))] = len(pool) // 4
        return bytes(pool)


class BratCodec(TableCodec):
    """AI attack weights: header, one group per combo command, then a byte table."""

    def __init__(self):
        super().__init__("brat", "brat", "group", "AI attack weights (chunk kind 1, tag brat)",
                         ["Read by main 0x088105FC. Magic `atta`, one group per combo command, then a",
                          "table of bytes (`table`, stored after its u32 length) that the groups' entries index."],
                         note=lambda values, layout, context: f"command {layout.at(values, 0)}")

    def tail_text(self, rest, header, head):
        if len(rest) < 4 or struct.unpack_from("<I", rest)[0] != len(rest) - 4:
            raise MovesetError("AI weights: the byte table's length does not match")
        return ["", "table", *hex_lines(rest[4:], indent="  ")]

    def tail_bytes(self, records, header, head, where):
        if len(records) != 1 or records[0].keyword != "table":
            raise MovesetError(f"{where}: the groups must be followed by one `table`")
        table = hex_bytes([token for item in records[0].items for token in _hex_item(item, where)], where)
        return struct.pack("<I", len(table)) + table


def _hex_item(item, where):
    if item.tokens[0] != "hex":
        raise MovesetError(f"{where}, line {item.line}: expected a `hex` line")
    return item.tokens[1:]


class CombCodec(Codec):
    """Combos: u16 version, s16 count, u32 offset per entry, then the entries, each followed by its
    stages."""
    name = "comb"

    def decode(self, data, context):
        semantics = context.semantics
        header, entry, stage = semantics.layout("comb.header"), semantics.layout("comb"), semantics.layout("comb.stage")
        head = header.unpack(data)
        count = header.value(head, "count")
        offsets = struct.unpack_from(f"<{count}I", data, 4)
        position = 4 + 4 * count
        lines = ["format comb", *_header_lines("Combo chains (chunk kind 1, tag comb)", [
            "An ability's command id maps to the attack records of its stages (battle 0x08AB7F40, 0x08AB81E0).",
            "A follow-up is accepted while the frame of the stage before is in [min_frame, max_frame).",
            "Counts and offsets are worked out by the compiler."])]
        lines += entry.glossary("combo", skip=("stages",)) + stage.glossary("stage")
        lines += ["", *header.block("header", head, skip=("count",)), ""]
        for index, offset in enumerate(offsets):
            if offset != position:
                raise MovesetError("combos: the entries are not one after another")
            values = entry.unpack(data, offset)
            number = entry.value(values, "stages")
            lines.append(f"{entry.line('combo', values, skip=('stages',)):48s}  # {index}")
            position = offset + entry.size
            for _ in range(number):
                stage_values = stage.unpack(data, position)
                notes = context.notes(stage, stage_values)
                line = "  " + stage.line("stage", stage_values)
                lines.append(f"{line:48s}  # {'; '.join(notes.values())}" if notes else line)
                position += stage.size
        if position != len(data):
            raise MovesetError(f"combos: {len(data) - position} bytes after the last entry")
        return "\n".join(lines) + "\n"

    def encode(self, text, where, semantics):
        header, entry, stage = semantics.layout("comb.header"), semantics.layout("comb"), semantics.layout("comb.stage")
        records = take_format(parse_records(text, where), "comb", where)
        if not records or records[0].keyword != "header":
            raise MovesetError(f"{where}: the first record must be `header`")
        head = header.assign(records[0].all_fields(), where)
        entries = []
        for record in records[1:]:
            expect(record, "combo", where)
            given = record.all_fields()
            if len(record.head.positional) > 2:
                raise MovesetError(f"{where}, line {record.line}: write the other fields as name=value")
            if len(record.head.positional) == 2:
                given[entry.fields[0].name] = ([record.head.positional[1]], record.line)
            if record.fields:
                raise MovesetError(f"{where}, line {record.line}: write a combo's fields on its own line")
            stages = []
            for item in record.items:
                if item.tokens[0] != "stage":
                    raise MovesetError(f"{where}, line {item.line}: expected `stage`")
                stage_given = dict(item.pairs)
                if len(item.positional) > 2:
                    raise MovesetError(f"{where}, line {item.line}: write the other fields as name=value")
                if len(item.positional) == 2:
                    stage_given[stage.fields[0].name] = ([item.positional[1]], item.line)
                stages.append(stage.pack(stage.assign(stage_given, where)))
            if "stages" in given or entry.field("stages").neutral in given:
                raise MovesetError(f"{where}, line {record.line}: the number of stages is counted, not given")
            values = entry.assign(given, where)
            values[entry.fields.index(entry.field("stages"))] = fit(len(stages), 16, True, where)
            entries.append(entry.pack(values) + b"".join(stages))
        if len(entries) > MAX_COMBOS:
            warn(f"{where}: {len(entries)} combo entries; the game keeps a mask of {MAX_COMBOS}")
        head[header.fields.index(header.field("count"))] = fit(len(entries), 16, True, where)
        out = bytearray(header.pack(head))
        position = 4 + 4 * len(entries)
        for body in entries:
            out += struct.pack("<I", position)
            position += len(body)
        return bytes(out) + b"".join(entries)


class TracksCodec(Codec):
    """Event tracks: u32 version, u32 count, u32 offset per track; each track
    {u16 anim, u8 ranged count, u8 instant count, u16 offset of the instant events, u16 size},
    its ranged events, then its instant events (docs/research/battle-moveset.md 2.3)."""
    name = "tracks"

    def args_text(self, which, kind, args, semantics):
        info = semantics.events.get((which, kind)) or {}
        types = [arg_type for _, arg_type in info.get("args", [])]
        out = []
        for index, word in enumerate(args):
            arg_type = types[index] if index < len(types) else None
            # Integer-typed, but holding what can only be a float (6.0 rather than 1086324736):
            # show the float; the notes' type is likely wrong for this use.
            if arg_type in ("s32", "u32") and 0x01000000 <= word < 0xFFFF0000 and "." in auto_text(word):
                arg_type = None
            if arg_type in KINDS and arg_type not in ("a32",):
                field = Field(0, arg_type, "arg")
                size, code, signed = KINDS[arg_type]
                if size == 4:
                    value = struct.unpack(f"<{code}", struct.pack("<I", word))[0]
                    text = field.scalar_text(value)
                    if _round_trips(text, word):
                        out.append(text)
                        continue
            out.append(auto_text(word))
        return out

    def event_notes(self, which, kind, args, context, texts):
        notes = []
        info = context.semantics.events.get((which, kind)) or {}
        names = [name for name, _ in info.get("args", [])]
        if names:
            notes.append(" ".join(f"{name}={text}" for name, text in zip(names, texts)))
        if which == "R" and kind == 0x00 and args:
            note = context.ref_note("atkp", args[0])
            if note:
                notes.append(note)
        if which == "I" and kind == 0x15 and args:
            notes.append(f"voice bank {args[0] // 100} slot {args[0] % 100}")
        return notes

    def decode(self, data, context):
        semantics = context.semantics
        header, ranged, instant = semantics.layout("tracks.header"), semantics.layout("ranged"), semantics.layout("instant")
        head = header.unpack(data)
        count = header.value(head, "count")
        offsets = struct.unpack_from(f"<{count}I", data, 8)
        lines = ["format tracks", *_header_lines("Event tracks (chunk kind 8): one per animation of the fighter's model", [
            "Evaluated each frame by main 0x08829930 (ranged) and main 0x0882CEC0 (instant).",
            "  R START END TYPE [name=value ...] ARGS...   an event that holds from frame START to END",
            "  I TICK TYPE [name=value ...] ARGS...        an event that fires once, at frame TICK",
            "name=value sets one of the event's condition fields (shown only when not zero):",
            "  R: " + ", ".join(f"{f.name} (+{f.offset})" for f in ranged.fields if f.name not in EVENT_HEAD["ranged"]),
            "  I: " + ", ".join(f"{f.name} (+{f.offset})" for f in instant.fields if f.name not in EVENT_HEAD["instant"]),
            "Arguments are 32-bit words: written with a point or an exponent they are floats.",
            "TYPE is a name from the semantics notes, event_0xNN, or 0xNN. Counts, offsets and sizes",
            "are worked out by the compiler; ranged and instant lines may come in any order."])]
        known = sorted(semantics.events.items())
        if known:
            lines.append("# Event types named in the semantics notes:")
            for (which, kind), info in known:
                args = ", ".join(name for name, _ in info.get("args", []))
                lines.append(f"#   {which} 0x{kind:02X} {info.get('name') or '':24s} ({args}) {info.get('note', '')}".rstrip())
        lines += ["", *header.block("header", head, skip=("count",)), ""]
        position = 8 + 4 * count
        for offset in offsets:
            if offset != position:
                raise MovesetError("event tracks: the tracks are not one after another")
            anim, n_ranged, n_instant, instant_at, size = struct.unpack_from("<HBBHH", data, offset)
            if offset + size > len(data):
                raise MovesetError(f"event tracks: the track of animation {anim} runs past the chunk")
            notes = [context.anim_name(anim)]
            for motion, variant in context.anim_motions.get(anim, []):
                attacks = context.motion_attacks.get(motion, [])
                notes.append(f"motion {motion}" + (f" v{variant}" if variant else "") + (f" {'/'.join(attacks)}" if attacks else ""))
            lines.append(f"track {anim:<36d}  # " + "; ".join(note for note in notes if note) if any(notes) else f"track {anim}")
            cursor = offset + 8
            for _ in range(n_ranged):
                values = ranged.unpack(data, cursor)
                start, end, kind, argc = (ranged.value(values, name) for name in EVENT_HEAD["ranged"])
                args = struct.unpack_from(f"<{argc}I", data, cursor + 12)
                conds = [f"{f.name}={f.text(v)}" for f, v in zip(ranged.fields, values)
                         if f.name not in EVENT_HEAD["ranged"] and not f.is_zero(v)]
                texts = self.args_text("R", kind, args, semantics)
                line = "  " + " ".join([f"R {start:4d} {end:5d}", semantics.event_name("R", kind)] + conds + texts)
                notes = self.event_notes("R", kind, args, context, texts)
                lines.append(f"{line:60s}  # {'; '.join(notes)}" if notes else line)
                cursor += 12 + 4 * argc
            if cursor != offset + instant_at:
                raise MovesetError(f"event tracks: the ranged events of animation {anim} do not end where its instant events start")
            for _ in range(n_instant):
                values = instant.unpack(data, cursor)
                tick, kind, argc = (instant.value(values, name) for name in EVENT_HEAD["instant"])
                args = struct.unpack_from(f"<{argc}I", data, cursor + 8)
                conds = [f"{f.name}={f.text(v)}" for f, v in zip(instant.fields, values)
                         if f.name not in EVENT_HEAD["instant"] and not f.is_zero(v)]
                texts = self.args_text("I", kind, args, semantics)
                line = "  " + " ".join([f"I {tick:4d}      ", semantics.event_name("I", kind)] + conds + texts)
                notes = self.event_notes("I", kind, args, context, texts)
                lines.append(f"{line:60s}  # {'; '.join(notes)}" if notes else line)
                cursor += 8 + 4 * argc
            if cursor != offset + size:
                raise MovesetError(f"event tracks: the instant events of animation {anim} do not end with its track")
            position = offset + size
        if position != len(data):
            raise MovesetError(f"event tracks: {len(data) - position} bytes after the last track")
        return "\n".join(lines) + "\n"

    def event(self, item, layout, which, semantics, where):
        place = f"{where}, line {item.line}"
        data = self._event(item, layout, which, semantics, where, place)
        if which == "R" and data[4] == 0x00 and data[5] >= 2:
            shape = struct.unpack_from("<I", data, 16)[0]
            if shape > MAX_HIT_SHAPE:
                warn(f"{place}: the hit event's shape {shape} is past {MAX_HIT_SHAPE}; it is a shape id (matched "
                     f"against each shape record's first byte) and only ids 0..{MAX_HIT_SHAPE} can be switched on by it")
        return data

    def _event(self, item, layout, which, semantics, where, place):
        columns = 3 if which == "R" else 2
        if len(item.positional) < columns + 1:
            raise MovesetError(f"{place}: an {which} line needs " + ("START END TYPE" if which == "R" else "TICK TYPE"))
        given = dict(item.pairs)
        heads = EVENT_HEAD["ranged" if which == "R" else "instant"]
        for name in heads:
            if name in given or layout.field(name).neutral in given:
                raise MovesetError(f"{place}: {name} is a column of the line, not a name=value")
        values = layout.assign(given, where)
        columns_given = item.positional[1:columns]
        kind = semantics.event_type(which, item.positional[columns], place)
        # By the literal's form only (a point or an exponent: float), never by the type the notes
        # give, so the same line means the same bits under any notes.
        args = [parse_auto(token, place) for token in item.positional[columns + 1:]]
        if len(args) > 255:
            raise MovesetError(f"{place}: an event takes at most 255 arguments")
        for name, token in zip(heads, columns_given):
            values[layout.fields.index(layout.field(name))] = layout.field(name).parse([token], place)
        values[layout.fields.index(layout.field("type"))] = kind
        values[layout.fields.index(layout.field("argc"))] = len(args)
        return layout.pack(values) + struct.pack(f"<{len(args)}I", *args)

    def encode(self, text, where, semantics):
        header, ranged, instant = semantics.layout("tracks.header"), semantics.layout("ranged"), semantics.layout("instant")
        records = take_format(parse_records(text, where), "tracks", where)
        if not records or records[0].keyword != "header":
            raise MovesetError(f"{where}: the first record must be `header`")
        head = header.assign(records[0].all_fields(), where)
        tracks = []
        for record in records[1:]:
            expect(record, "track", where)
            if len(record.head.tokens) != 2 or record.fields:
                raise MovesetError(f"{where}, line {record.line}: write a track as `track ANIM`")
            anim = fit(parse_int(record.head.tokens[1], f"{where}, line {record.line}"), 16, False, where)
            ranged_events, instant_events = [], []
            for item in record.items:
                which = item.tokens[0]
                if which == "R":
                    ranged_events.append(self.event(item, ranged, "R", semantics, where))
                elif which == "I":
                    instant_events.append(self.event(item, instant, "I", semantics, where))
                else:
                    raise MovesetError(f"{where}, line {item.line}: expected an `R` or `I` line")
            if len(ranged_events) > 255 or len(instant_events) > 255:
                raise MovesetError(f"{where}, line {record.line}: a track holds at most 255 events of each kind")
            body = b"".join(ranged_events)
            size = 8 + len(body) + sum(map(len, instant_events))
            if size > 0xFFFF:
                raise MovesetError(f"{where}, line {record.line}: the track is {size} bytes, more than 65535")
            tracks.append(struct.pack("<HBBHH", anim, len(ranged_events), len(instant_events), 8 + len(body), size)
                          + body + b"".join(instant_events))
        head[header.fields.index(header.field("count"))] = len(tracks)
        out = bytearray(header.pack(head))
        position = 8 + 4 * len(tracks)
        for track in tracks:
            out += struct.pack("<I", position)
            position += len(track)
        return bytes(out) + b"".join(tracks)


def _round_trips(text, word):
    try:
        return parse_auto(text, "") == word
    except MovesetError:
        return False


class TextCodec(Codec):
    """Attack names for the battle banner: an `mlng` container of message tables, one per language
    setting (tools/character_text.py)."""
    name = "text"

    def decode(self, data, context):
        import character_text
        if data[:4] != b"mlng" or len(data) < 8:
            raise MovesetError("text: not an mlng container")
        count = struct.unpack_from("<I", data, 4)[0]
        offsets = struct.unpack_from(f"<{count}I", data, 8)
        sizes = struct.unpack_from(f"<{count}I", data, 8 + 4 * count)
        cursor = 8 + 8 * count
        lines = ["format text", *_header_lines("Banner names (chunk kind 1, tag text): an mlng container", [
            "One message table per language setting (main 0x08870B10 picks it; 0 is the retail English).",
            "  string N \"Name\"     a banner name, in the banner font (tools/character_text.py GLYPHS)",
            "  string N hex ...    anything else, as the game sees it after the XOR",
            "  string N none       no string",
            "  language L copy=K   the same strings as language K, stored again",
            "  language L share=K  no table of its own: points at language K's",
            "  padding N           zero bytes after the tables"])]
        seen = []
        for language, (offset, size) in enumerate(zip(offsets, sizes)):
            shared = next((index for index, (o, s) in enumerate(seen) if (o, s) == (offset, size)), None)
            if shared is not None and offset != cursor:
                lines += ["", f"language {language} share={shared}"]
                seen.append((offset, size))
                continue
            if offset != cursor or offset + size > len(data):
                raise MovesetError("text: the tables are not stored one after another")
            table = data[offset:offset + size]
            copy = next((index for index, (o, s) in enumerate(seen) if data[o:o + s] == table), None)
            seen.append((offset, size))
            cursor += size
            if copy is not None:
                lines += ["", f"language {language} copy={copy}"]
                continue
            strings = character_text.read_table(table)
            if character_text.write_table(strings) != table:
                raise MovesetError(f"text: language {language}'s table is not stored as the game's writer stores it")
            lines += ["", f"language {language}"]
            for number, string in enumerate(strings):
                if string is None:
                    lines.append(f"  string {number} none")
                elif isinstance(string, str):
                    lines.append(f"  string {number} {string_text(string.encode('ascii'))}")
                else:
                    lines.append(f"  string {number} hex {string.hex(' ').upper()}")
        padding = data[cursor:]
        if padding.strip(b"\0"):
            raise MovesetError("text: the bytes after the tables are not zero")
        lines += ["", f"padding {len(padding)}"]
        return "\n".join(lines) + "\n"

    def encode(self, text, where, semantics):
        import character_text
        records = take_format(parse_records(text, where), "text", where)
        tables, entries, padding = [], [], 0
        for record in records:
            place = f"{where}, line {record.line}"
            if record.keyword == "padding":
                padding = fit(parse_int(record.head.tokens[1], place), 32, False, place)
                continue
            expect(record, "language", where)
            if record.head.positional[1:] != [str(len(entries))]:
                raise MovesetError(f"{place}: languages are numbered 0, 1, 2... in order")
            pairs = record.head.pairs
            if "share" in pairs or "copy" in pairs:
                which = "share" if "share" in pairs else "copy"
                other = parse_int(pairs[which][0][0], place)
                if not 0 <= other < len(entries) or record.items:
                    raise MovesetError(f"{place}: {which} names a language before this one, and nothing follows")
                if which == "share":
                    entries.append(entries[other])
                else:
                    tables.append(tables[entries[other]])
                    entries.append(len(tables) - 1)
                continue
            strings = {}
            for item in record.items:
                spot = f"{where}, line {item.line}"
                if item.tokens[0] != "string" or len(item.tokens) < 3:
                    raise MovesetError(f"{spot}: expected `string N ...`")
                number = parse_int(item.tokens[1], spot)
                if number in strings:
                    raise MovesetError(f"{spot}: string {number} is given twice")
                kind = item.tokens[2]
                if kind == "none" and len(item.tokens) == 3:
                    strings[number] = None
                elif kind == "hex":
                    strings[number] = hex_bytes(item.tokens[3:], spot)
                elif len(item.tokens) == 3:
                    name = parse_string(kind, spot).decode("ascii")
                    try:
                        character_text.encode(name)
                    except character_text.TextError as error:
                        raise MovesetError(f"{spot}: {error}") from None
                    strings[number] = name
                else:
                    raise MovesetError(f"{spot}: expected a name, `hex ...` or `none`")
            ordered = [strings.get(number) for number in range(max(strings) + 1)] if strings else []
            tables.append(character_text.write_table(ordered))
            entries.append(len(tables) - 1)
        offsets, position = [], 8 + 8 * len(entries)
        for table in tables:
            offsets.append(position)
            position += len(table)
        return (b"mlng" + struct.pack(f"<I{len(entries)}I{len(entries)}I", len(entries),
                                      *(offsets[entry] for entry in entries), *(len(tables[entry]) for entry in entries))
                + b"".join(tables) + bytes(padding))


class RawCodec(Codec):
    """Bytes not yet read field by field, as hex. Edits may change the length."""
    name = "raw"

    def __init__(self, reason=""):
        self.reason = reason

    def decode(self, data, context):
        about = ["Not read field by field yet: the bytes as hex, 16 to a line (the comment is the offset).",
                 "The compiler joins the hex lines; it may be edited, lengthened or shortened."]
        if self.reason:
            about.append(self.reason)
        return "\n".join(["format raw", *_header_lines("Raw chunk", about), ""] + hex_lines(data)) + "\n"

    def encode(self, text, where, semantics):
        records = take_format(parse_records(text, where), "raw", where)
        out = bytearray()
        for record in records:
            expect(record, "hex", where)
            no_items(record, where)
            out += hex_bytes(record.head.tokens[1:], f"{where}, line {record.line}")
        return bytes(out)


TEXT_CODECS = {
    "plya": TableCodec("plya", "plya", "attack", "Attack records (chunk kind 1, tag plya)",
                       ["Read by main 0x088480D0; started by battle 0x08B382B8, which plays `motion`.",
                        "The record count is worked out by the compiler."], note=_attack_note),
    "atkp": TableCodec("atkp", "atkp", "hit", "Hit records (chunk kind 1, tag atkp)",
                       ["Named by the hit-window event (R 0x00) and by projectiles. The record count is worked out by the compiler."],
                       note=_hit_note),
    "shapes": TableCodec("shapes", "shape", "shape", "Collision shapes (chunk kind 5)",
                         ["Read by main 0x0883E6E8. The record count is worked out by the compiler."]),
    "motions": TableCodec("motions", "motion", "motion", "Motion table (chunk kind 7)",
                          ["Motion id -> animation of the fighter's model, binary-searched by main 0x0882F0D4:",
                           "keep it sorted by motion id. The entry count is worked out by the compiler."],
                          line_style=True, note=_motion_note),
    "magi": MagiCodec(),
    "brat": BratCodec(),
    "comb": CombCodec(),
    "tracks": TracksCodec(),
    "text": TextCodec(),
    "raw": RawCodec(),
}


def codec_for(kind, tag):
    if kind == 1 and tag.decode("latin-1") in ("plya", "atkp", "magi", "brat", "comb", "text"):
        return TEXT_CODECS[tag.decode("latin-1")]
    return {7: TEXT_CODECS["motions"], 8: TEXT_CODECS["tracks"], 5: TEXT_CODECS["shapes"]}.get(kind)


# Chunks carried as their own bytes: formats of their own, read by other tools if at all.
BINARY_KINDS = {2: "GMO model", 3: "GIM picture", 4: "MPK effect package (FEP/FEPA/FXSE)", 13: "EX burst resource",
                14: "SSCF sound bank", 17: "EX burst resource (mlng/SEQ)"}
NESTED_KINDS = {18}


# ---------------------------------------------------------------------------------- decompile/build
def gmo_motion_names(data):
    """Names of the motions of a GMO model, in file order (the animation index)."""
    names = []
    if data[:11] != b"OMG.00.1PSP":
        raise MovesetError("not a GMO model")

    def walk(position, end):
        while position + 8 <= end:
            kind, head, size = struct.unpack_from("<HHI", data, position)
            if size < 8 or position + size > end:
                return
            if kind < 0x8000 and head:
                if kind == 0xB:
                    names.append(data[position + 16:position + head].split(b"\0")[0].decode("latin-1"))
                elif kind not in (7, 0xC):
                    walk(position + head, position + size)
            position += size
    walk(16, len(data))
    return names


def render(data, semantics=None, gmo_names=None, where="archive", top=True):
    """The decompiled folder of an archive as {relative path: bytes}, and notes on what was carried
    raw: [(path, kind, tag, size, reason)]."""
    semantics = semantics or Semantics()
    words, chunks = arc_directory(data, where)
    context = Context(data, chunks, semantics, gmo_names)
    files, raw = {}, []
    lines = ["format archive", *_header_lines("An ARC archive (main 0x0885C93C): its chunks, in order", [
        "  chunk KIND \"TAG\" FILE align=N   N: the chunk starts on a multiple of N bytes",
        "Chunks may be added, removed or moved; offsets and sizes are worked out by the compiler.",
        "FILE is a text file (its `format` line says how to read it), a .bin file (the bytes as they",
        "are) or a folder ending in / (an archive inside the archive).",
        "`names builtin`: fields and events go by the tool's own names. `names notes`: by the",
        "semantics notes copied into notes/ when this folder was written; the compiler reads them",
        "from there, so later changes to the notes do not change what this folder means."]), ""]
    lines += [f"names {semantics.mode}", ""]
    lines += ["archive", f"  field_0x08: 0x{words[0]:08X}", f"  field_0x0C: 0x{words[1]:08X}", ""]
    if top:
        files.update({f"notes/{name}": text.encode("utf-8") for name, text in semantics.texts.items()})
    for index, (kind, tag, offset, size) in enumerate(chunks):
        body = data[offset:offset + size]
        label = f"{index:02d}_{chunk_label(kind, tag)}"
        codec = codec_for(kind, tag)
        note = ""
        if kind in NESTED_KINDS and body[:4] == ARC_MAGIC:
            name = label + "/"
            try:
                inner, inner_raw = render(body, semantics, None, f"{where} chunk {index}", top=False)
                _QUIET.append(True)
                try:
                    rebuilt = build(lambda path: inner[path], semantics, f"{where} chunk {index}")
                finally:
                    _QUIET.pop()
                if rebuilt != body:
                    raise MovesetError("the nested archive does not build back the same")
                files.update({name + path: value for path, value in inner.items()})
                raw += [(name + path, *rest) for path, *rest in inner_raw]
                note = "archive inside the archive"
            except MovesetError as error:
                name, codec = label + ".bin", None
                files[name] = body
                raw.append((name, kind, tag, size, f"nested archive not decompiled: {error}"))
        elif codec is not None:
            name = label + ".txt"
            try:
                text = codec.decode(body, context)
                _QUIET.append(True)
                try:
                    same = codec.encode(text, name, semantics) == body
                finally:
                    _QUIET.pop()
                if not same:
                    raise MovesetError("the text does not compile back to the same bytes")
                files[name] = text.encode("utf-8")
                note = text.split("\n", 2)[1].lstrip("# ")
            except (MovesetError, struct.error, IndexError, UnicodeError) as error:
                reason = f"{codec.name} chunk not read field by field: {error}"
                files[name] = RawCodec(reason).decode(body, context).encode("utf-8")
                raw.append((name, kind, tag, size, reason))
                note = "raw: " + reason
        elif kind in BINARY_KINDS:
            name = label + ".bin"
            files[name] = body
            raw.append((name, kind, tag, size, BINARY_KINDS[kind]))
            note = BINARY_KINDS[kind] + ", as its bytes"
        else:
            name = label + ".txt"
            reason = {6: "Chunk kind 6: cloth/swing-bone physics (main 0x0880D82C).",
                      9: "Chunk kind 9: EX burst background `hibg` (battle 0x08ACDA90)."}.get(kind, f"Chunk kind {kind}.")
            files[name] = RawCodec(reason).decode(body, context).encode("utf-8")
            raw.append((name, kind, tag, size, reason))
            note = "raw hex: " + reason
        line = f"chunk {kind} {string_text(tag)} {name} align={alignment(kind)}"
        lines.append(f"{line:48s}  # {note}" if note else line)
    files[MANIFEST] = ("\n".join(lines) + "\n").encode("utf-8")
    return files, raw


def _read_optional(read, path):
    try:
        return read(path)
    except (MovesetError, KeyError, FileNotFoundError):
        return None


def build(read, semantics=None, where="archive"):
    """An archive from a decompiled folder; read(relative path) gives a file's bytes. Fields and
    events go by `semantics` if given, else by what the folder's `names` line says."""
    text = read(MANIFEST).decode("utf-8")
    records = take_format(parse_records(text, f"{where}/{MANIFEST}"), "archive", f"{where}/{MANIFEST}")
    names = [record for record in records if record.keyword == "names"]
    records = [record for record in records if record.keyword != "names"]
    if semantics is None:
        mode = names[0].head.tokens[1:] if names else []
        if mode == ["notes"]:
            texts = {name: _read_optional(read, f"notes/{name}") for name in NOTES}
            semantics = Semantics({name: body.decode("utf-8") for name, body in texts.items() if body is not None})
        elif mode == ["builtin"]:
            semantics = Semantics()
        elif not names:
            semantics = load_semantics()
        else:
            raise MovesetError(f"{where}/{MANIFEST}, line {names[0].line}: names is `builtin` or `notes`")
    words = [0, 0]
    chunks = []
    for record in records:
        place = f"{where}/{MANIFEST}, line {record.line}"
        if record.keyword == "archive":
            no_items(record, place)
            layout = Layout("archive header", 8, [Field(0, "x32", "field_0x08"), Field(4, "x32", "field_0x0C")])
            words = layout.assign(record.all_fields(), place)
            continue
        expect(record, "chunk", f"{where}/{MANIFEST}")
        no_items(record, place)
        if len(record.head.positional) != 4:
            raise MovesetError(f"{place}: write a chunk as `chunk KIND \"TAG\" FILE align=N`")
        kind = fit(parse_int(record.head.positional[1], place), 32, False, place)
        tag = parse_string(record.head.positional[2], place)
        if len(tag) > 4:
            raise MovesetError(f"{place}: a tag is at most 4 bytes")
        tag += bytes(4 - len(tag))
        name = record.head.positional[3]
        align = parse_int(record.head.pairs["align"][0][0], place) if "align" in record.head.pairs else alignment(kind)
        if align < 1 or align & (align - 1):
            raise MovesetError(f"{place}: align must be a power of two")
        extra = set(record.head.pairs) - {"align"}
        if extra:
            raise MovesetError(f"{place}: unknown setting {sorted(extra)[0]}")
        if name.endswith("/"):
            body = build(lambda path, prefix=name: read(prefix + path), semantics, f"{where}/{name.rstrip('/')}")
        elif name.endswith(".bin"):
            body = read(name)
        else:
            content = read(name).decode("utf-8")
            first = parse_records(content, f"{where}/{name}")
            if not first or first[0].keyword != "format" or len(first[0].head.tokens) != 2:
                raise MovesetError(f"{where}/{name}: the file does not start with a `format` line")
            codec = TEXT_CODECS.get(first[0].head.tokens[1])
            if codec is None:
                raise MovesetError(f"{where}/{name}: unknown format {first[0].head.tokens[1]}")
            body = codec.encode(content, f"{where}/{name}", semantics)
        chunks.append((kind, tag, align, body))
    out = bytearray(ARC_MAGIC + struct.pack("<III", len(chunks), *words))
    directory = len(out)
    out += bytes(16 * len(chunks))
    for index, (kind, tag, align, body) in enumerate(chunks):
        out += bytes(-len(out) % align)
        struct.pack_into("<I4sII", out, directory + 16 * index, kind, tag, len(out), len(body))
        out += body
    return bytes(out)


def decompile(data, outdir, semantics=None, gmo=None):
    """Write the decompiled folder. Returns the notes on raw chunks."""
    names = gmo_motion_names(gmo) if gmo else None
    files, raw = render(data, semantics or load_semantics(), names)
    outdir = Path(outdir)
    for path, body in files.items():
        target = outdir / path
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(body)
    return raw


def compile_folder(indir, semantics=None):
    indir = Path(indir)

    def read(path):
        target = indir / path
        if not target.is_file():
            raise MovesetError(f"{target} is missing")
        return target.read_bytes()
    return build(read, semantics, indir.name)


def check(data, semantics=None, gmo=None):
    """Decompile into a temporary folder, compile, compare. Returns (same, message, raw notes)."""
    semantics = semantics or load_semantics()
    with tempfile.TemporaryDirectory() as folder:
        raw = decompile(data, folder, semantics, gmo)
        rebuilt = compile_folder(folder)
    if rebuilt == data:
        return True, f"{len(data)} bytes, identical", raw
    if len(rebuilt) != len(data):
        message = f"{len(rebuilt)} bytes built, {len(data)} expected"
    else:
        first = next(index for index in range(len(data)) if data[index] != rebuilt[index])
        message = f"first difference at byte 0x{first:X}"
    return False, message, raw


# ------------------------------------------------------------------------------------------- diff
def _record_blocks(text):
    """[(header line, [field and item lines], the header line's comment)] of a decompiled text
    file, comments left out of the lines."""
    blocks = []
    for raw in text.splitlines():
        bare = _strip_comment(raw)
        if not bare.strip():
            continue
        if bare[:1] in (" ", "	") and blocks:
            blocks[-1][1].append(bare.strip())
        else:
            blocks.append((bare.strip(), [], raw[len(bare):].strip().lstrip("#").strip()))
    return blocks


def _strip_comment(line):
    quoted = False
    for index, char in enumerate(line):
        if char == '"' and (index == 0 or line[index - 1] != "\\"):
            quoted = not quoted
        elif char == "#" and not quoted:
            return line[:index].rstrip()
    return line.rstrip()


def diff(a, b, semantics=None):
    """Lines describing how archive b differs from archive a, record by record."""
    semantics = semantics or load_semantics()
    left, _ = render(a, semantics)
    right, _ = render(b, semantics)
    out = []
    for path in sorted(set(left) | set(right)):
        if left.get(path) == right.get(path):
            continue
        if path not in left or path not in right:
            out.append(f"{'+' if path in right else '-'} {path}")
            continue
        if path.endswith(".bin"):
            out.append(f"~ {path}: {len(left[path])} bytes -> {len(right[path])} bytes")
            continue
        old, new = _record_blocks(left[path].decode("utf-8")), _record_blocks(right[path].decode("utf-8"))
        changes = []
        matcher = difflib.SequenceMatcher(None, [(head, tuple(body)) for head, body, _ in old],
                                          [(head, tuple(body)) for head, body, _ in new], autojunk=False)
        for op, i1, i2, j1, j2 in matcher.get_opcodes():
            if op == "equal":
                continue
            pairs = min(i2 - i1, j2 - j1) if op == "replace" else 0
            for k in range(pairs):
                (head_a, body_a, note_a), (head_b, body_b, note_b) = old[i1 + k], new[j1 + k]
                changes.append(f"  ~ {head_a}" + (f"  ->  {head_b}" if head_a != head_b else "")
                               + (f"  # {note_a}" if note_a else ""))
                for line in difflib.unified_diff(body_a, body_b, lineterm="", n=0):
                    if not line.startswith(("---", "+++", "@@")):
                        changes.append(f"      {line[0]} {line[1:]}")
            for head, body, note in old[i1 + pairs:i2]:
                changes.append(f"  - {head}" + (f"  # {note}" if note else ""))
            for head, body, note in new[j1 + pairs:j2]:
                changes.append(f"  + {head}" + (f"  # {note}" if note else ""))
                changes += [f"      {line}" for line in body]
        if changes:
            out.append(f"== {path}")
            out += changes
    return out


# ------------------------------------------------------------------------------------------ sweep
def archive_names(world=False):
    names = []
    for code in CODES:
        if code == "org200":
            names += [f"obj/p_org200_{number:02d}.objx" for number in range(3)]
        else:
            names.append(f"obj/p_{code}.objx")
        names.append(f"obj/p_{code}_a.objx")
        if world:
            names += [f"obj/p_{code}_w.objx", f"obj/P_{code.upper()}_ZAKO_W.objx"]
    return names


def open_game(iso=ISO):
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import package_archive
    return package_archive.Archive(iso)


def read_archive(argument, game=None):
    if argument.startswith("iso:"):
        owned = game is None
        game = game or open_game()
        try:
            data = game.read(argument[4:])
        finally:
            if owned:
                game.close()
        if data is None:
            raise MovesetError(f"{argument[4:]} is not in the game image")
        return data
    return Path(argument).read_bytes()


def sweep(iso=ISO, world=False, out=print):
    """Check every fighter's main and assist moveset archive. Returns (checked, identical, raw)."""
    semantics = load_semantics()
    game = open_game(iso)
    checked = identical = 0
    raw_kinds = {}
    try:
        for name in archive_names(world):
            data = game.read(name)
            if data is None:
                continue
            same, message, raw = check(data, semantics)
            checked += 1
            identical += same
            for path, kind, tag, size, reason in raw:
                key = (kind, tag.decode("latin-1") if kind in (1, 3, 4, 13, 14, 17) else "*")
                entry = raw_kinds.setdefault(key, [0, 0, reason])
                entry[0] += 1
                entry[1] += size
            out(f"{'same' if same else 'DIFFERENT':9s} {name:32s} {message}")
    finally:
        game.close()
    out(f"{identical} of {checked} archives compile back byte for byte")
    out("Carried as bytes or hex, not field by field:")
    for (kind, tag), (count, size, reason) in sorted(raw_kinds.items()):
        out(f"  kind {kind:2d} {tag:5s} {count:4d} chunks {size:12,d} bytes  {reason}")
    return checked, identical, raw_kinds


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    sub = parser.add_subparsers(dest="command", required=True)
    one = sub.add_parser("decompile")
    one.add_argument("archive")
    one.add_argument("outdir")
    one.add_argument("--gmo", help="the fighter's model, to name animations")
    one.add_argument("--builtin", action="store_true", help="the tool's own names only, not the semantics notes")
    two = sub.add_parser("compile")
    two.add_argument("indir")
    two.add_argument("archive")
    three = sub.add_parser("check")
    three.add_argument("archives", nargs="+")
    four = sub.add_parser("diff")
    four.add_argument("a")
    four.add_argument("b")
    five = sub.add_parser("sweep")
    five.add_argument("--iso", default=str(ISO))
    five.add_argument("--world", action="store_true")
    args = parser.parse_args(argv)
    try:
        if args.command == "decompile":
            semantics = Semantics() if args.builtin else load_semantics()
            gmo = read_archive(args.gmo) if args.gmo else None
            raw = decompile(read_archive(args.archive), args.outdir, semantics, gmo)
            print(f"Wrote {args.outdir} ({semantics.mode} names); {len(raw)} chunks kept as bytes or hex")
            for line in semantics.skipped:
                print(f"note: not used from the semantics notes: {line}", file=sys.stderr)
        elif args.command == "compile":
            data = compile_folder(args.indir)
            Path(args.archive).write_bytes(data)
            print(f"Wrote {args.archive}, {len(data)} bytes")
        elif args.command == "check":
            bad = 0
            for argument in args.archives:
                same, message, _ = check(read_archive(argument))
                bad += not same
                print(f"{'same' if same else 'DIFFERENT':9s} {argument}: {message}")
            return 1 if bad else 0
        elif args.command == "diff":
            lines = diff(read_archive(args.a), read_archive(args.b))
            print("\n".join(lines) if lines else "No differences")
        elif args.command == "sweep":
            checked, identical, _ = sweep(Path(args.iso), args.world)
            return 0 if checked == identical else 1
    except MovesetError as error:
        print(f"error: {error}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
