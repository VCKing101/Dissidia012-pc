/* The save manager starts a request of kind `request` (+0x2B30) with argument `arg` (+0x2B34): optionally clears flag 1 and runs the utility call, then either opens the request's message box (table 0x089D7140) or starts the utility directly. */
typedef struct SaveObj {
    char pad[0x2B30];
    int request;
    int arg;
} SaveObj;
typedef struct SaveManager {
    int unk_00;
    unsigned int flags;
    int *buffer;
    SaveObj *save;
} SaveManager;
typedef struct RequestRow {
    unsigned short message;
    unsigned short pad;
    int param;
    int unk_08;
    int unk_0C;
} RequestRow;
typedef struct Global9D712C { char pad[0xEE0]; void *texts; } Global9D712C;

extern RequestRow D_089D7140[];
extern Global9D712C *D_089D712C;
extern void *D_089D727C;
extern void *D_089D7280;

void func_088AF424(SaveManager *manager, int a, int b);
void *func_0887AFBC(void *texts, int id, int flag);
void func_088B1230(void *a, void *b, void *callback, int c, int d, int e);
void func_088B16FC(void *a);
void func_088AFB64(void);

void func_088AF620(SaveManager *manager, int request, int arg, unsigned char clear)
{
    int c = clear;
    SaveObj *save = manager->save;
    if (c != 0) {
        manager->flags = manager->flags & ~1u;
        func_088B16FC(D_089D7280);
        save = manager->save;
    }
    save->request = request;
    manager->save->arg = arg;
    if (D_089D7140[manager->save->request].message != 0) {
        void *text = func_0887AFBC(D_089D712C->texts, D_089D7140[manager->save->request].message, 0);
        func_088B1230(D_089D727C, text, (void *)func_088AFB64, 0, D_089D7140[manager->save->request].param, manager->save->arg);
    } else {
        func_088AF424(manager, 0, 0);
    }
}
