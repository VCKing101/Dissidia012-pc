typedef struct BodyStatus {
    char pad_0[0x3];
    unsigned char flags;

    static unsigned char isDown(BodyStatus *status)
    {
        unsigned int bit = ((unsigned char *)status)[3] & 1;
        return bit;
    }
} BodyStatus;

typedef struct CheckBody {
    char pad_000[0x190];
    BodyStatus status;
} CheckBody;

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

extern "C" {

extern UnitInfo *func_08AB3264(int unit);
extern int func_08837458(CheckBody *body);

int func_08A70750(CheckOwner *self, void *a, void *b, void *c)
{
    CheckBody *body;
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    body = (CheckBody *)func_08AB3264(self->unit)->body;
    if (body == 0) {
        return -2;
    }
    if (func_08837458(body) == 0 || BodyStatus::isDown(&body->status) == 1) {
        self->hit = 1;
    }
    return 0;
}

}
