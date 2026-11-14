/* Loads user icon `id` into its page slot; returns 0 when the slot is not ready. */
typedef struct IconSlot {
    char pad_00[0x8C];
    int field_8C;
} IconSlot;

typedef struct IconBody {
    char pad_0000[0x490];
    IconSlot field_490[16];
    char pad_1390[0x1268 - 0xD90];
    int field_1268;
} IconBody;

typedef struct IconLoader {
    char pad_0000[0x1274];
    IconBody *field_1274;
} IconLoader;

extern char D_08A3BE18[];
extern void func_0893476C(char *buffer, const char *format, int value);
extern int func_08869AB4(IconSlot *slot, const char *name);

int func_08A390D8(IconLoader *self, int id)
{
    char name[32];
    int index;
    IconSlot *slot;
    if (id < 0) {
        return 0;
    }
    index = (id - 1) / 32;
    if (index < 0 || index >= self->field_1274->field_1268) {
        return 0;
    }
    slot = &self->field_1274->field_490[index];
    if (slot->field_8C < 2) {
        return 0;
    }
    func_0893476C(name, D_08A3BE18, id);
    return func_08869AB4(slot, name);
}
