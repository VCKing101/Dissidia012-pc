import copy
import json
import unittest
from pathlib import Path
from audit_overlay_dispatch import FileImage
from audit_menu_member_table import inspect_empty_callback

ROOT = Path(__file__).resolve().parents[1]


class ChangedImage:
    def __init__(self, original, changes):
        self.original, self.changes = original, changes
    def word(self, address, executable=False):
        return self.changes.get(address, self.original.word(address, executable))
    def read(self, address, size, executable=False):
        import struct
        result = bytearray(self.original.read(address, size, executable))
        for position, word in self.changes.items():
            if address <= position and position + 4 <= address + size:
                struct.pack_into('<I', result, position - address, word)
        return bytes(result)


class MenuMemberChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.review = json.loads((ROOT / 'config/menu-member-table-review.json').read_text())
        cls.image = FileImage(ROOT / 'orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF', cls.review['reference_sha256'])
        cls.functions = json.loads((ROOT / 'config/inventory/menu.json').read_text())['functions']

    def test_real_record_and_empty_boundary(self):
        result = inspect_empty_callback(self.image, self.functions, self.review)
        self.assertEqual(result['entry'], '0x089DE3A4')
        self.assertEqual(result['bytes'], 8)

    def test_redirected_callback_record_is_rejected(self):
        image = ChangedImage(self.image, {0x089E0A84: 0x089DE3AC})
        with self.assertRaisesRegex(ValueError, 'does not bind'):
            inspect_empty_callback(image, self.functions, self.review)

    def test_copy_that_omits_last_record_is_rejected(self):
        image = ChangedImage(self.image, {0x089DD5F0: 0x34060098})
        with self.assertRaisesRegex(ValueError, 'different table size'):
            inspect_empty_callback(image, self.functions, self.review)

    def test_nonempty_body_is_rejected(self):
        image = ChangedImage(self.image, {0x089DE3A8: 0x00801021})
        with self.assertRaisesRegex(ValueError, 'jr-ra/nop'):
            inspect_empty_callback(image, self.functions, self.review)

    def test_wrong_neighbor_boundary_is_rejected(self):
        review = copy.deepcopy(self.review)
        review['empty_callback']['next_function'] = '0x089DE548'
        with self.assertRaisesRegex(ValueError, 'next known function'):
            inspect_empty_callback(self.image, self.functions, review)
