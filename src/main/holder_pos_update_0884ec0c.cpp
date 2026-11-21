struct Vec4_0884EC0C {
    float x, y, z, w;
};

struct Obj0884EC0C {
    float x, y, z;                     /* +0x00 */
    unsigned char unk_0C[0x34 - 0xC];
    unsigned int flags;                /* +0x34 bit 9: position pending */
};

struct Holder0884EC0C {
    int id;                            /* +0x00 */
    Obj0884EC0C *obj;                  /* +0x04 */
};

extern "C" Vec4_0884EC0C func_08838D14(int id);

extern "C" void func_0884EC0C(Holder0884EC0C *self)
{
    if (self->obj != 0) {
        Obj0884EC0C *obj = self->obj;
        Vec4_0884EC0C v;

        v = func_08838D14(self->id);
        float x = v.x;
        float y = v.y;
        float z = v.z;
        unsigned int flags = obj->flags;
        obj->x = x;
        obj->y = y;
        obj->z = z;
        obj->flags = flags & ~0x200;
    }
}
