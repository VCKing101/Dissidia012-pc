typedef struct UnitInfo {
    char pad_00[0x4];
    void *body;
} UnitInfo;

typedef struct CheckOwner {
    unsigned char active;
    unsigned char hit;
    unsigned char miss;
    char pad_03[0x61];
    int unit;
    int field_68;
} CheckOwner;

extern UnitInfo *func_08AB3264(int unit);
extern int func_08B2F5E0(void *body);
extern int func_08B2F7E0(void *body);
extern void func_08A969BC(int target, int mode);

int func_08A707EC(CheckOwner *self, void *a, void *b, void *c)
{
    void *body;
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    body = func_08AB3264(self->unit)->body;
    if (body == 0) {
        return -2;
    }
    if (func_08B2F5E0(body) == 0 && func_08B2F7E0(body) == 0) {
        self->miss = 1;
        return 0;
    }
    func_08A969BC(self->field_68, 0);
    self->hit = 1;
    return 0;
}
