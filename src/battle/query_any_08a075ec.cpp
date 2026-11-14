typedef struct QueryOwner {
    unsigned char active;
} QueryOwner;

extern "C" {
extern int func_08A07674(QueryOwner *self);
extern int func_08A0775C(QueryOwner *self);
extern int func_08A077D8(QueryOwner *self);
}

struct QueryOps {
    static unsigned char any(QueryOwner *self)
    {
        int result = 0;
        if (func_08A07674(self) == 1 || func_08A0775C(self) == 1 || func_08A077D8(self) != 0) {
            result = 1;
        }
        return result;
    }
};

extern "C" {

unsigned char func_08A075EC(QueryOwner *self)
{
    if (!self->active) {
        return 0;
    }
    return QueryOps::any(self);
}

}
