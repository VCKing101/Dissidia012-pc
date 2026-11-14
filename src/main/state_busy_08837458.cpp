struct Status08837458 {
    unsigned char unknown_00[4];
    unsigned char flags;
};

struct Actor08837458 {
    unsigned char unknown_000[0x190];
    Status08837458 status;
};

struct Check08837458 {
    static bool locked(const Status08837458 &status) {
        return (status.flags & 0x10) != 0;
    }
};

extern "C" {
int func_08837430(Actor08837458 *self);

unsigned char func_08837458(Actor08837458 *self) {
    int result = 0;
    Status08837458 *status;
    if (func_08837430(self) != 0 || (status = &self->status, Check08837458::locked(*status))) {
        result = 1;
    }
    return result;
}
}
