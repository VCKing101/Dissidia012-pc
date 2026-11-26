/* The save manager's "Update" task body: waits for the pending save/load state, starts requests, runs the utility's state machine and opens the request message box or finishes the request. */
typedef struct SaveObj {
    char pad0[0x800];
    int util_state;
    char pad1[0xDB0 - 0x804];
    int util_result;
    char pad2[0x2B30 - 0xDB4];
    int result;
    int arg;
    char pad3[0x2B7C - 0x2B38];
    unsigned char hold;
    unsigned char waiting;
    unsigned char closing;
} SaveObj;
typedef struct SaveManager {
    int unk_00;
    unsigned int flags;
    int *buffer;
    SaveObj *save;
} SaveManager;
typedef struct Util { char pad[2]; unsigned char idle; } Util;

extern Util *D_089D7280;
extern void *D_089D727C;

int func_088FEA4C(void);
void func_088FE9E8(void);
void func_088AFBCC(SaveManager *manager);
void func_089C5858(void *p);
int func_088AF98C(SaveManager *manager, int arg);
int func_088B178C(Util *u);
void func_088AFA94(SaveManager *manager, int arg);
void func_08872A18(void);
unsigned int func_089C5868(void);
void func_089A0190(int a, int b);
void func_089C5898(int a);
void func_089C5878(void);
int func_088AFDE4(SaveManager *manager);
int func_088AF5D4(SaveManager *manager, int err);
void func_088B16FC(Util *u);
void func_088B1194(void *a, int b, void *cb, int c, int d);
void func_088AF620(SaveManager *manager, int request, int arg, unsigned char clear);
void func_088B1764(Util *u);
void func_088AFB64(void);

void func_088B01C0(SaveManager *manager, int arg)
{
    SaveObj *save = manager->save;
    int pending;
    unsigned int flags;
    if (save->closing != 0) {
        if (func_088FEA4C() != 0) {
            return;
        }
        func_088FE9E8();
        func_088AFBCC(manager);
        manager->save->closing = 0;
        func_089C5858((char *)manager->save + 0x7E4);
    } else {
        if (func_088AF98C(manager, arg) == 0) {
            return;
        }
        save = manager->save;
        pending = 0;
        if (save->waiting != 0) {
            if (func_088B178C(D_089D7280) == 0) {
                manager->save->waiting = 0;
                func_088AFA94(manager, arg);
                func_08872A18();
            }
            return;
        }
        if (save->hold != 0) {
            if (D_089D7280->idle != 0) {
                save->hold = 0;
                save = manager->save;
                if (save->util_result == 0) {
                    save->util_state = 0;
                } else {
                    pending = 1;
                }
            } else {
                return;
            }
        }
        switch (func_089C5868()) {
        case 1:
            break;
        case 2:
            func_089A0190(4, 0);
            func_089C5898(2);
            break;
        case 3:
            func_089C5878();
            break;
        case 4:
            break;
        case 0:
            save = manager->save;
            if (save->util_state == 2 && pending == 0) {
                save->hold = 1;
            } else {
                pending = func_088AFDE4(manager);
                if (func_088AF5D4(manager, pending) != 0) {
                    func_088AFA94(manager, arg);
                    flags = manager->flags;
                    if ((flags & 1) != 0) {
                        manager->flags = flags & ~1u;
                        func_088B16FC(D_089D7280);
                    }
                    manager->save->result = 1;
                    func_088B1194(D_089D727C, pending, (void *)func_088AFB64, 0, 0xA0);
                } else if (manager->save->result != 0) {
                    func_088AFA94(manager, arg);
                    func_088AF620(manager, manager->save->result, manager->save->arg, manager->flags & 1);
                } else if ((manager->flags & 1) != 0) {
                    func_088AFA94(manager, arg);
                    func_08872A18();
                } else {
                    func_088B1764(D_089D7280);
                    manager->save->waiting = 1;
                    return;
                }
            }
            break;
        default:
            break;
        }
    }
}
