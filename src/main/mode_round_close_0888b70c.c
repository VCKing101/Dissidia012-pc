/* Closes a mode round's systems and, when the active unit stands on one of
   two named stages, restores the stage effect. */
typedef struct UnitKind {
    unsigned char kind;
} UnitKind;

typedef struct ModeUnit {
    char pad_000[0x5A4];
    UnitKind *kind;
} ModeUnit;

typedef struct StageInfo {
    char pad_00[8];
    char name[8];
} StageInfo;

typedef struct StageCtl {
    char pad_000[0x710];
    StageInfo *info;
} StageCtl;

extern int D_089D4DC8;
extern int D_08B54330;
extern int D_08ABBDF8;
extern int D_08B11E28;
extern int D_089D6A10;
extern StageCtl *D_089D6AC8;
extern char D_089CA0C0[];
extern char D_089CA0C4[];

extern void func_0885A4FC(int p);
extern void func_08B5341C(int p);
extern void func_08ABB4FC(int p);
extern void func_08ABB5BC(int p);
extern int func_0881BAA8(void);
extern void func_0881C010(int p);
extern void func_0881BA04(int p);
extern void func_0881BA2C(int p);
extern void func_08B0DB40(int p);
extern void func_08889944(int mode, int menu);
extern ModeUnit *func_08889E44(void);
extern int func_08935970(const char *a, const char *b);

void func_0888B70C(void)
{
    ModeUnit *unit;
    StageInfo *info;

    func_0885A4FC(D_089D4DC8);
    func_08B5341C(D_08B54330);
    func_08ABB4FC(D_08ABBDF8);
    func_08ABB5BC(D_08ABBDF8);
    func_0881C010(func_0881BAA8());
    func_0881BA04(func_0881BAA8());
    func_08B0DB40(D_08B11E28);
    func_08889944(D_089D6A10, 1);
    unit = func_08889E44();
    info = D_089D6AC8->info;
    if (unit != 0 && unit->kind->kind == 0x1C && info != 0 &&
        (func_08935970(info->name, D_089CA0C0) == 0 || func_08935970(info->name, D_089CA0C4) == 0)) {
        func_0881BA2C(func_0881BAA8());
    }
}
