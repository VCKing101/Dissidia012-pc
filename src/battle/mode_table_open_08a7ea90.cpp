/* Opens the mode table for an owner: clears the five slots (-1), starts
 * slot 0 at mode 0's first value, marks the alternate rule when the owner's
 * stage kind allows it, and records the owner (-1 without an owner or
 * argument). */
typedef struct OpenEntry {
    int value;
    int pad_04;
} OpenEntry;

typedef struct OpenInfo {
    int slot;
    OpenEntry *entries;
    int count;
} OpenInfo;

typedef struct OpenSlot {
    int value;
    int limit;
    int marks[3];
} OpenSlot;

typedef struct OpenKind {
    unsigned char kind;
} OpenKind;

typedef struct OpenStage {
    unsigned char pad_000[0x5A4];
    OpenKind *kind;
} OpenStage;

typedef struct OpenOwner {
    unsigned char pad_00[4];
    OpenStage *stage;
} OpenOwner;

typedef struct OpenTable {
    unsigned char open;
    unsigned char pad_01[3];
    OpenOwner *owner;
    void *arg;
    unsigned char alternate;
    unsigned char pad_0d[0x1B];
    OpenSlot slots[5];
} OpenTable;

extern OpenInfo D_08A7FFE0[7];
extern void *D_08A80218;

extern "C" {

extern void func_08A7EA74(OpenTable *self);
extern void *func_08A80200(void *rules);
extern int func_08AACD30(void *rules, unsigned char kind);

int func_08A7EA90(OpenTable *self, OpenOwner *owner, void *arg)
{
    func_08A7EA74(self);
    if (owner == 0 || arg == 0) {
        return -1;
    }
    for (int i = 0; i < 5; i++) {
        OpenSlot *slot = &self->slots[i];
        slot->value = -1;
        slot->limit = -1;
        for (int j = 0; j < 3; j++) {
            slot->marks[j] = -1;
        }
    }
    int first = D_08A7FFE0[0].entries[0].value;
    self->slots[0].value = first;
    self->slots[0].limit = first;
    OpenStage *stage = owner->stage;
    if (stage != 0) {
        OpenKind *kind = stage->kind;
        if (kind != 0) {
            if (func_08AACD30(func_08A80200(D_08A80218), kind->kind) != 0) {
                self->alternate = 1;
            }
        }
    }
    self->owner = owner;
    self->arg = arg;
    self->open = 1;
    return 0;
}

}
