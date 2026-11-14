typedef struct FlagBody {
    char pad_00[0xF];
    unsigned char flags;

    static unsigned char isGuarding(FlagBody *body)
    {
        unsigned int bit = (body->flags & 2) != 0;
        return bit;
    }
} FlagBody;

extern "C" {

extern int func_088377C0(void *unit);
extern FlagBody *func_08B30898(void *unit);

int func_08AB4198(void *self, void *unit)
{
    if (unit == 0) {
        return 0;
    }
    if (func_088377C0(unit) == 0) {
        return 0;
    }
    FlagBody *body = func_08B30898(unit);
    if (body == 0) {
        return 0;
    }
    if (!FlagBody::isGuarding(body)) {
        return 0;
    }
    return 1;
}

}
