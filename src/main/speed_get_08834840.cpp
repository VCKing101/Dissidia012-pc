struct Actor08834840 {
    unsigned char unknown_000[0x384];
    unsigned int flags;
    unsigned char unknown_388[0x14];
    int motion;
    unsigned char unknown_3A0[0x38];
    float scale;

    static bool fast(Actor08834840 *self) {
        return (self->flags & 0x80) != 0;
    }
};

extern "C" {
float func_0884E4C4(int motion);
float func_0884E504(int motion);

float func_08834840(Actor08834840 *self) {
    float speed = func_0884E4C4(self->motion) * self->scale;
    if (Actor08834840::fast(self)) {
        speed = func_0884E504(self->motion) * self->scale;
    }
    return speed;
}
}
