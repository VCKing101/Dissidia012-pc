extern "C" {
/* 1 when one of the six slots holds an entry with the id. */
typedef struct Entry08839120 {
    int id;
} Entry08839120;

typedef struct Slot08839120 {
    char pad_00[0x20];
    Entry08839120 *entry;
    char pad_24[0x1C];
} Slot08839120;

typedef struct Unit08839120 {
    char pad_000[0x534];
    Slot08839120 *slots;
} Unit08839120;

int func_08839120(Unit08839120 *u, int id)
{
    for (int i = 0; i < 6; i++) {
        Slot08839120 *slot = &u->slots[i];
        if (slot->entry != 0 && slot->entry->id == id) {
            return 1;
        }
    }
    return 0;
}
}
