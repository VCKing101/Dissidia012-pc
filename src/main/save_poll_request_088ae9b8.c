/* Save manager per-frame poll: finishes a running utility, handles the confirm flag (+0xEC1) and the input state (cancel, exit, ready), and starts or finishes the next save/load request. */
typedef struct SaveMgr {
    char pad0[0x1FC];
    int state;
    char pad1[0x210 - 0x200];
    int mode;
    char pad2[0x7AC - 0x214];
    int has_data;
    char pad3[0xEAC - 0x7B0];
    int request;
    int request_arg;
    char pad4[0xEC1 - 0xEB4];
    unsigned char confirm;
    unsigned char waiting;
    char pad5[0xEF8 - 0xEC3];
    unsigned int flags;
} SaveMgr;
typedef struct Util { char pad[2]; unsigned char ready; } Util;

extern Util *D_089D7280;
extern void *D_089D727C;

int func_088AD10C(SaveMgr *m);
int func_088B178C(Util *u);
void func_088ABDF0(SaveMgr *m, int arg);
void func_08872A18(void);
int func_089C5868(void);
void func_089A0190(int a, int b);
void func_089C5898(int a);
void func_089C5878(void);
int func_088AE500(SaveMgr *m);
int func_088ACD8C(SaveMgr *m, int v);
void func_088B16FC(Util *u);
void func_088B1194(void *a, int b, void *cb, int c, int d);
void func_088ACDD0(void);
void func_088ACDF8(SaveMgr *m, int a, int b, unsigned char c);
void func_088B1764(Util *u);

void func_088AE9B8(SaveMgr *mp, int argp)
{
    SaveMgr *mm;
    int a;
    mm = 0;
    a = argp;
    mm = mp;
    int busy;
    unsigned int flags;
    if (func_088AD10C(mm) == 0) {
        return;
    }
    busy = 0;
    if (mm->waiting) {
        if (func_088B178C(D_089D7280) == 0) {
            mm->waiting = 0;
            func_088ABDF0(mm, a);
            func_08872A18();
        }
        return;
    }
    if (mm->confirm) {
        if (D_089D7280->ready) {
            mm->confirm = 0;
            if (mm->has_data == 0) {
                mm->state = 0;
            } else {
                if (mm->mode == 5 || mm->mode == 1) {
                    mm->request = 12;
                }
                busy = 1;
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
        if (mm->state == 2 && !busy) {
            mm->confirm = 1;
        } else {
            busy = func_088AE500(mm);
            if (func_088ACD8C(mm, busy)) {
                func_088ABDF0(mm, a);
                flags = mm->flags;
                if (flags & 2) {
                    mm->flags = flags & ~2u;
                    func_088B16FC(D_089D7280);
                }
                mm->request = 1;
                func_088B1194(D_089D727C, busy, (void *)func_088ACDD0, 0, 0xA0);
            } else if (mm->request != 0) {
                func_088ABDF0(mm, a);
                func_088ACDF8(mm, mm->request, mm->request_arg, (mm->flags & 2) != 0);
            } else if (mm->flags & 2) {
                func_088ABDF0(mm, a);
                func_08872A18();
            } else {
                func_088B1764(D_089D7280);
                mm->waiting = 1;
                break;
            }
        }
        break;
    default:
        break;
    }
}
