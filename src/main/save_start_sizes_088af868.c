/* The save manager starts a SIZES request (flag 8's mode 0x2B3C == 8): picks a slot, fills the name fields, waits for the utility to be idle, then runs the request; returns 1. */
typedef struct SaveObj {
    char pad[0x800];
    int slot_ready;
    char pad2[0x814 - 0x804];
    int mode;
    char pad3[0xDD4 - 0x818];
    int unk_DD4;
    char pad4[0x2B3C - 0xDD8];
    int request_mode;
} SaveObj;
typedef struct SaveManager {
    int unk_00;
    unsigned int flags;
    int *buffer;
    SaveObj *save;
} SaveManager;
typedef struct Util { char pad[3]; unsigned char busy; } Util;
typedef struct Obj9D69E0 Obj9D69E0;

extern Obj9D69E0 *D_089D69E0;
extern Util *D_089D7280;

int func_088AF7B0(SaveManager *manager);
void *func_08935828(void *dst, int value, unsigned int size);
char *func_08935BB4(char *dst, const char *src, unsigned int size);
void func_08883F9C(Obj9D69E0 *self, char arg1, unsigned char arg2);
void func_089C5858(void *param);

int func_088AF868(SaveManager *manager)
{
    int slot = -1;
    unsigned char busy;
    if (manager->save->slot_ready == 0) {
        slot = func_088AF7B0(manager);
    }
    if (manager->save->request_mode == 8) {
        func_08935828((char *)manager->save + 0xE0C, 0, 0x40);
        func_08935BB4((char *)manager->save + 0xE0C, (char *)manager->save + 0x820, 0xD);
        if (slot < 0) {
            slot = 0;
        }
        func_08935BB4((char *)manager->save + 0xE1C, (char *)manager->save + slot * 0x14, 0x14);
        busy = D_089D7280->busy;
    } else {
        busy = D_089D7280->busy;
    }
    while (busy != 0) {
        func_08883F9C(D_089D69E0, 1, 0);
        busy = D_089D7280->busy;
    }
    manager->save->unk_DD4 = 3;
    manager->save->mode = manager->save->request_mode;
    func_089C5858((char *)manager->save + 0x7E4);
    return 1;
}
