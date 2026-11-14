/* Lands an actor that is close enough to the ground: drops it onto the
   ground, resets the stance and starts the landing move. */
struct Target08832D9C {
    char pad_00[0x28];
    unsigned int flags;
};

struct Actor08832D9C {
    char pad_000[0x84];
    float height;
    char pad_088[0x384 - 0x88];
    unsigned int stance;
    char pad_388[0x3C0 - 0x388];
    float ground;
    char pad_3C4[0x544 - 0x3C4];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void drop(float dy);
};

extern "C" {

extern float D_089C7EC8;

void func_08834C10(Actor08832D9C *self, int id, unsigned short param);
void func_08A51750(Actor08832D9C *self, float x, float y, void *path, float z, int mode, float w);

bool func_08832D9C(Actor08832D9C *self, Target08832D9C *target, void *path,
                   float x, float y, float z, float w)
{
    float ground = self->ground;
    if (D_089C7EC8 < ground) {
        return false;
    }
    self->drop((self->height - ground) - self->height);
    self->stance |= 0x200;
    self->stance &= ~0x100;
    self->stance &= ~0x800;
    func_08834C10(self, 0, 0);
    func_08A51750(self, x, y, path, z, 0, w);
    target->flags |= 1;
    return true;
}

}
