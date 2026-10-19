"""Check the copied menu member table and its previously unclassified callback."""
import json
import struct
from pathlib import Path
from audit_overlay_dispatch import FileImage

ROOT = Path(__file__).resolve().parents[1]


def inspect_empty_callback(image, functions, review):
    if (review.get('module') != 'menu' or review.get('record_count') != 20
            or review.get('record_bytes') != 8):
        raise ValueError('Unexpected menu member-table geometry')
    dispatcher = int(review['dispatcher'], 16)
    for field, offset in [('table_high_pc', 12), ('table_low_pc', 32),
                          ('copy_size_pc', 20), ('copy_call_pc', 28)]:
        if int(review[field], 16) != dispatcher + offset:
            raise ValueError('Member-table evidence belongs to another dispatcher prefix')
    table = int(review['table'], 16)
    size = review['record_count'] * review['record_bytes']
    high = image.word(int(review['table_high_pc'], 16), True)
    low = image.word(int(review['table_low_pc'], 16), True)
    immediate = low & 0xFFFF
    if immediate >= 0x8000:
        immediate -= 0x10000
    if high >> 16 != 0x3C05 or low >> 16 != 0x24A5 or ((high & 0xFFFF) << 16) + immediate != table:
        raise ValueError('Menu dispatcher materializes a different member table')
    copy_pc = int(review['copy_call_pc'], 16)
    call = image.word(copy_pc, True)
    target = ((copy_pc + 4) & 0xF0000000) | ((call & 0x03FFFFFF) << 2)
    if call >> 26 != 3 or target != int(review['copy_helper'], 16):
        raise ValueError('Menu dispatcher copy helper differs')
    if image.word(int(review['copy_size_pc'], 16), True) != (0x34060000 | size):
        raise ValueError('Menu dispatcher copies a different table size')
    callback = review['empty_callback']
    entry, extent = int(callback['entry'], 16), callback['bytes']
    index = callback['record_index']
    if extent != 8 or not 0 <= index < review['record_count']:
        raise ValueError('Empty menu callback has invalid extent or record index')
    record = table + index * review['record_bytes']
    adjustment, flags, pointer = struct.unpack('<hhI', image.read(record, 8))
    if (adjustment, flags, pointer) != (0, 0, entry):
        raise ValueError('Menu member record does not bind the reviewed empty callback')
    if image.read(entry, extent, True) != struct.pack('<II', 0x03E00008, 0):
        raise ValueError('Menu empty callback is no longer jr-ra/nop')
    next_entry = int(callback['next_function'], 16)
    if entry + extent != next_entry or next_entry not in {int(f['address'], 16) for f in functions}:
        raise ValueError('Menu empty callback does not abut the next known function')
    return {'entry': callback['entry'], 'bytes': extent, 'member_record': f'0x{record:08X}',
            'dispatcher': review['dispatcher'], 'copied_table_bytes': size,
            'next_function': callback['next_function']}


def main():
    review = json.loads((ROOT / 'config/menu-member-table-review.json').read_text())
    image = FileImage(ROOT / 'orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF', review['reference_sha256'])
    functions = json.loads((ROOT / 'config/inventory/menu.json').read_text())['functions']
    print(json.dumps(inspect_empty_callback(image, functions, review), indent=2))


if __name__ == '__main__':
    main()
