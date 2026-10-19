"""Read and write the attack names a fighter's moveset archive carries for the battle screen.

When a fighter starts an HP attack or enters EX Mode the battle screen shows a
banner with a name. The text is not looked up by ability: it is string number
N of the `text` chunk in the fighter's own archive (obj/p_xxxNNN.objx), where
N comes with the attack (main 0x08856D48 makes the banner). A character with
an archive of its own can therefore have names of its own without any patch.

The chunk, as the game reads it:

    container   "mlng", u32 count (6), u32 offset[count], u32 size[count]: one message table for
                each language setting, picked by main 0x08870B10. The retail English table is
                number 0; the other five are one Japanese table stored five times.
    table       u16 count, u16 offset[count] (in units of two bytes, from the start of the table),
                then the strings: u16 byte length, text. Main 0x0887A800 finds a string.
    hiding      16-bit unit j of the text of string i is stored XORed with KEY[(i + j) & 15];
                main 0x0887A6A0 undoes it once, in memory, when the table is attached.
    text        a banner string starts with the unit 0x001C, which sends it to the glyph-run
                renderer (main 0x0887E9CC) instead of the message formatter. Bytes follow: the
                glyph numbers of the banner font, each as a little-endian base-128 number, then a
                zero byte, then a zero byte more if that is needed to end on a whole unit.

Only the glyphs of GLYPHS are known. A string that holds anything else (the
EX Mode prompt has a button picture in it) is handed out as `bytes`, its text
as the game sees it after the XOR, and is put back unchanged.

`rebuild` keeps the chunk's size, so nothing else in the archive moves: it
writes one table, points every language at it and fills the rest with zeros.
"""

import struct

# The game's key for hiding message text (main 0x089D68AA, 16 words), as read from the reference
# executable.
KEY = (0xED98, 0x90AB, 0x5E28, 0x8442, 0x32B1, 0x2DDA, 0xA657, 0xC8BF,
       0x0033, 0x579A, 0xF227, 0x4B4C, 0x36A7, 0x927D, 0x739D, 0x84CE)
KEY_ADDRESS = 0x089D68AA

ARCHIVE_MAGIC = b"ARC\x01"
CONTAINER_MAGIC = b"mlng"
TEXT_TAG = b"text"
GLYPH_RUN = 0x001C

# Glyph numbers of the banner font, as far as they are known.
GLYPHS = {" ": 0x20, "!": 0x12D}
GLYPHS.update({chr(ord("A") + index): 0x60 + index for index in range(26)})
GLYPHS.update({chr(ord("a") + index): 0x7A + index for index in range(26)})
LETTERS = {number: letter for letter, number in GLYPHS.items()}


class TextError(ValueError):
    """The chunk is not laid out as expected, or the new text cannot be written into it."""


def find_chunk(archive):
    """Offset and size of the `text` chunk, from the archive's own directory
    (16-byte entries after a 16-byte header: kind, tag, offset, size)."""
    if archive[:4] != ARCHIVE_MAGIC or len(archive) < 16:
        raise TextError("Not a moveset archive")
    count = struct.unpack_from("<I", archive, 4)[0]
    if 16 + count * 16 > len(archive):
        raise TextError("The archive's directory is cut short")
    found = []
    for index in range(count):
        _, tag, offset, size = struct.unpack_from("<I4sII", archive, 16 + index * 16)
        if tag == TEXT_TAG:
            if offset + size > len(archive):
                raise TextError("The text chunk lies outside the archive")
            found.append((offset, size))
    if len(found) != 1:
        raise TextError(f"The archive has {len(found)} text chunks, not one")
    return found[0]


def _mask(text, index):
    """XOR the text of string `index` with the key. The same call hides and reveals."""
    if len(text) % 2:
        raise TextError("A string's text must be a whole number of 16-bit units")
    units = struct.unpack(f"<{len(text) // 2}H", text)
    return struct.pack(f"<{len(units)}H", *(unit ^ KEY[(index + place) & 15] for place, unit in enumerate(units)))


def encode(name):
    """The text of a banner string showing `name`, as the game sees it after the XOR."""
    body = bytearray(struct.pack("<H", GLYPH_RUN))
    for letter in name:
        number = GLYPHS.get(letter)
        if number is None:
            raise TextError(f"No glyph is known for {letter!r} in {name!r}")
        while number >= 0x80:
            body.append(number & 0x7F | 0x80)
            number >>= 7
        body.append(number)
    body.append(0)
    if len(body) % 2:
        body.append(0)
    return bytes(body)


def decode(text):
    """The name a banner string shows, or None when it holds something `encode` cannot write."""
    if len(text) < 4 or struct.unpack_from("<H", text, 0)[0] != GLYPH_RUN:
        return None
    letters, number, shift = [], 0, 0
    for byte in text[2:]:
        if byte == 0 and shift == 0:
            break
        number |= (byte & 0x7F) << shift
        shift += 7
        if byte < 0x80:
            if number not in LETTERS:
                return None
            letters.append(LETTERS[number])
            number, shift = 0, 0
    name = "".join(letters)
    return name if encode(name) == text else None


def _tables(chunk):
    """[(offset, size)] of the container's tables, one per language setting."""
    if chunk[:4] != CONTAINER_MAGIC or len(chunk) < 8:
        raise TextError("The text chunk is not a language container")
    count = struct.unpack_from("<I", chunk, 4)[0]
    if count == 0 or 8 + count * 8 > len(chunk):
        raise TextError("The language container's header is cut short")
    offsets = struct.unpack_from(f"<{count}I", chunk, 8)
    sizes = struct.unpack_from(f"<{count}I", chunk, 8 + count * 4)
    for offset, size in zip(offsets, sizes):
        if offset < 8 + count * 8 or offset + size > len(chunk):
            raise TextError("A language table lies outside the chunk")
    return list(zip(offsets, sizes))


def table(chunk, language=0):
    """The bytes of one language's message table, as stored."""
    offset, size = _tables(chunk)[language]
    return chunk[offset:offset + size]


def read_table(data):
    """The strings of a stored message table: a name where the string is a
    plain banner name, else its text as `bytes`. A string the table leaves out
    (offset 0) is None."""
    if len(data) < 2:
        raise TextError("The message table is cut short")
    count = struct.unpack_from("<H", data, 0)[0]
    if 2 + count * 2 > len(data):
        raise TextError("The message table's offsets are cut short")
    strings = []
    for index in range(count):
        offset = struct.unpack_from("<H", data, 2 + index * 2)[0] * 2
        if offset == 0:
            strings.append(None)
            continue
        if offset + 2 > len(data):
            raise TextError(f"String {index} lies outside its table")
        size = struct.unpack_from("<H", data, offset)[0]
        if offset + 2 + size > len(data):
            raise TextError(f"String {index} runs past the end of its table")
        text = _mask(data[offset + 2:offset + 2 + size], index)
        name = decode(text)
        strings.append(text if name is None else name)
    return strings


def write_table(strings):
    """A stored message table holding `strings` (names, `bytes` as `read_table` gives them, or None)."""
    if len(strings) > 0xFFFF:
        raise TextError("Too many strings for one table")
    offsets, body = [], bytearray()
    start = 2 + len(strings) * 2
    for index, string in enumerate(strings):
        if string is None:
            offsets.append(0)
            continue
        text = encode(string) if isinstance(string, str) else bytes(string)
        position = start + len(body)
        if position // 2 > 0xFFFF or len(text) > 0xFFFF:
            raise TextError("The strings are too long for one table")
        offsets.append(position // 2)
        body += struct.pack("<H", len(text)) + _mask(text, index)
    return struct.pack(f"<H{len(offsets)}H", len(strings), *offsets) + bytes(body)


def read(chunk, language=0):
    """The strings of the chunk for one language setting (0 is the retail English)."""
    return read_table(table(chunk, language))


def rebuild(chunk, strings):
    """A chunk of the same size as `chunk`, with the same number of language
    entries, all of them pointing at one table that holds `strings`."""
    count = len(_tables(chunk))
    data = write_table(strings)
    head = 8 + count * 8
    if head + len(data) > len(chunk):
        raise TextError(f"The names need {head + len(data)} bytes and the text chunk has {len(chunk)}")
    header = CONTAINER_MAGIC + struct.pack(f"<I{count}I{count}I", count, *([head] * count), *([len(data)] * count))
    return header + data + bytes(len(chunk) - head - len(data))


def rename(archive, names, language=0):
    """The archive with banner strings replaced: `names` is {string number: name}.
    Returns the archive unchanged (the same bytes) when it already says so."""
    offset, size = find_chunk(archive)
    chunk = archive[offset:offset + size]
    strings = read(chunk, language)
    wanted = list(strings)
    for number, name in names.items():
        if not 0 <= number < len(wanted):
            raise TextError(f"The archive has no banner string {number}")
        encode(name)        # an unknown glyph should fail here, by name
        wanted[number] = name
    if wanted == strings:
        return archive
    return archive[:offset] + rebuild(chunk, wanted) + archive[offset + size:]
