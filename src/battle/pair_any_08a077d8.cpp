typedef struct QueryOwner {
    unsigned char active;
} QueryOwner;

extern "C" {

extern int func_08A0785C(QueryOwner *self, int index);

int func_08A077D8(QueryOwner *self)
{
    if (!self->active) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 2; i++) {
        if (func_08A0785C(self, i) == 1) {
            result = 1;
            break;
        }
    }
    return result;
}

}
