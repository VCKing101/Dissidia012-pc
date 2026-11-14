struct BodyUnit {
    char pad_000[0x3C0];
    float height;
};

struct UnitSlot {
    char pad_00[0x4];
    BodyUnit *body;
};

struct BodyCheck {
    char pad_00[0x64];
    int id;
};

extern "C" {
extern UnitSlot *func_08AB3264(int id);
extern int func_08A83FAC(BodyUnit *body);
extern int func_08A83FDC(BodyUnit *body);
extern float D_08A7AC18;
}

struct BodyRules {
    static unsigned char low(BodyUnit *body, int force)
    {
        int result = 0;
        if (func_08A83FAC(body) == 1 || force == 1) {
            if (body->height < D_08A7AC18 && func_08A83FDC(body) == 0) {
                result = 1;
            }
        }
        return result;
    }
};

extern "C" {

/* 1 when the unit's body (grounded, or forced) is below the limit and not blocked. */
unsigned char func_08A6B9D8(BodyCheck *self, unsigned char force)
{
    BodyUnit *body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return 0;
    }
    return BodyRules::low(body, force);
}

}
