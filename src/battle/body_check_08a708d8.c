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
extern int func_08A83CF4(void *body);
extern int func_08A83DE0(void *body);

int func_08A708D8(CheckOwner *self, void *a, void *b, void *c)
{
    void *body;
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    body = func_08AB3264(self->unit)->body;
    if (body == 0) {
        return -2;
    }
    if (func_08A83CF4(body) == 0 || func_08A83DE0(body) == 1) {
        self->hit = 1;
    }
    return 0;
}
