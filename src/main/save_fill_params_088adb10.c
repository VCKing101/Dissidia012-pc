/* Fills the savedata parameter block (arg 2): clears it, sets the file name and the entry fields and the title text ("new data" when the game has none). */
typedef struct Entry {
    char pad[0x10];
    char *text;
} Entry;
typedef struct Param {
    char pad0[0x38];
    int unk_38;
    char name[0x584 - 0x3C];
    char e0[0x10];
    char e1[0x10];
    char e2[0x10];
    char e3[0x10];
    Entry *entry;
    char pad1[0x5EC - 0x5C8];
    int unk_5EC;
    int unk_5F0;
} Param;
typedef struct Obj {
    char pad[0xEC8];
    int unk_EC8;
    int unk_ECC;
    int unk_ED0;
    int unk_ED4;
    int pad2;
    int pad3;
    int unk_EE0;
} Obj;
extern char D_089CB428[];
extern void *D_089D7280;
extern char *D_089D6010;
void func_088B168C(void *a, void *b, int n);
void func_08935BB4(void *dst, void *src, int n);
void func_088ADF68(Obj *obj, void *dst, int a, int b);
char *func_0887AFBC(int a, int b, int c);

void func_088ADB10(Obj *obj, Param *param)
{
    char *text;
    func_088B168C(D_089D7280, param, 0x600);
    param->unk_38 = 0;
    func_08935BB4(param->name, D_089D6010, 13);
    func_088ADF68(obj, param->e0, obj->unk_ECC, obj->unk_EC8);
    func_088ADF68(obj, param->e1, 0, 0);
    func_088ADF68(obj, param->e2, 0, 0);
    func_088ADF68(obj, param->e3, 0, 0);
    func_088ADF68(obj, param->entry, obj->unk_ED4, obj->unk_ED0);
    text = func_0887AFBC(obj->unk_EE0, 7, 0);
    if (text == 0) {
        text = D_089CB428;
    }
    func_08935BB4(param->entry->text, text, 0x80);
    param->unk_5EC = 0;
    param->unk_5F0 = 0;
}
