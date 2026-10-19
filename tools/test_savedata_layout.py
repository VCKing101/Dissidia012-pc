"""Compile the observed savedata views with real 32-bit PSP C/C++ layouts."""
import unittest
from pathlib import Path
from compile_psp import compile_object

ROOT = Path(__file__).resolve().parents[1]
LAYOUT = r'''
#include "menu/execution_state.h"
#define OFFSET(t,m) ((unsigned int)&(((t *)0)->m))
typedef char RequestSize[(sizeof(MenuSavedataRequest)==0x600)?1:-1];
typedef char TimestampSize[(sizeof(MenuSaveTimestamp)==16)?1:-1];
typedef char StateAllocationSize[(sizeof(MenuExecutionState)==0x3FAB0)?1:-1];
typedef char DataAt[(OFFSET(MenuExecutionState,data_0DD0)==0xDD0)?1:-1];
typedef char DataCapacity[(sizeof(((MenuExecutionState *)0)->data_0DD0)==0x3ECC8)?1:-1];
typedef char CachedAt[(OFFSET(MenuExecutionState,cached_time_3FA98)==0x3FA98)?1:-1];
typedef char SlotSize[(sizeof(MenuSaveSlotEntry)==0x48)?1:-1];
typedef char SlotListSize[(sizeof(MenuSaveSlotList)==0xC)?1:-1];
typedef char SaveNames[(sizeof(((MenuExecutionState *)0)->names_0C28)==0x1A4)?1:-1];
typedef char RequestAt[(OFFSET(MenuExecutionState,savedata_007C)==0x7C)?1:-1];
typedef char SlotsAt[(OFFSET(MenuExecutionState,slots_067C)==0x67C)?1:-1];
typedef char EntriesAt[(OFFSET(MenuExecutionState,entries_0688)==0x688)?1:-1];
typedef char NamesAt[(OFFSET(MenuExecutionState,names_0C28)==0xC28)?1:-1];
typedef char LoadedAt[(OFFSET(MenuExecutionState,field_0DCC)==0xDCC)?1:-1];
typedef char ModeAt[(OFFSET(MenuSavedataRequest,mode)==0x30)?1:-1];
typedef char BufferAt[(OFFSET(MenuSavedataRequest,data)==0x74)?1:-1];
typedef char FocusAt[(OFFSET(MenuSavedataRequest,focus)==0x5C8)?1:-1];
typedef char KeyAt[(OFFSET(MenuSavedataRequest,key)==0x5DC)?1:-1];
typedef char ListAt[(OFFSET(MenuSavedataRequest,id_list)==0x5F4)?1:-1];
'''


class SavedataLayoutChecks(unittest.TestCase):
    def compile(self, tag, suffix, source):
        directory = ROOT / "build/savedata-layout-tests"
        directory.mkdir(parents=True, exist_ok=True)
        path = directory / (tag + suffix)
        path.write_text(source, encoding="utf-8")
        return compile_object("tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe", ["-O2", "-g"],
                              path.relative_to(ROOT), (directory / (tag + ".raw.o")).relative_to(ROOT))

    def test_psp_c_layout(self):
        result = self.compile("c", ".c", LAYOUT)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_psp_cpp_layout(self):
        result = self.compile("cpp", ".cpp", LAYOUT)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_changed_sdk_list_offset_is_rejected(self):
        result = self.compile("bad_offset", ".c", LAYOUT.replace("id_list)==0x5F4", "id_list)==0x5F0"))
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("ListAt", result.stdout + result.stderr)


if __name__ == "__main__": unittest.main()
