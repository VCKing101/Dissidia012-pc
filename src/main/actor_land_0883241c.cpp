/* Starts an actor's landing: drops it to the ground unless it lands
   standing, then faces it, plays the landing motion and resets the stance. */
struct Vec4_0883241C {
    float x, y, z, w;
};

struct Motion0883241C {
    char data[0x50];
};

struct Target0883241C {
    char pad_00[0x28];
    unsigned int flags;
};

struct Actor0883241C {
    char pad_000[0x84];
    float height;
    char pad_088[0x130 - 0x88];
    Motion0883241C motion;
    char pad_180[0x384 - 0x130 - sizeof(Motion0883241C)];
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

void func_08834C10(Actor0883241C *self, int id, unsigned short param);
void func_08832150(Actor0883241C *self, Target0883241C *target, float time);
void func_089A3890(Vec4_0883241C *v);
void func_08A58A5C(Actor0883241C *self, Vec4_0883241C *pos);
void func_0882FA10(Motion0883241C *motion, int id, int mode, float time, float start);
void func_08834934(Actor0883241C *self, int a, int b);

void func_0883241C(Actor0883241C *self, Target0883241C *target, unsigned char standing,
                   float y, float w, float time)
{
    int motion;
    if (standing) {
        motion = 600;
    } else {
        self->drop((self->height - self->ground) - self->height);
        self->stance |= 0x200;
        self->stance &= ~0x100;
        self->stance &= ~0x800;
        func_08834C10(self, 0, 0);
        motion = 601;
    }
    func_08832150(self, target, time);
    Vec4_0883241C p;
    func_089A3890(&p);
    p.y = y;
    p.w = w;
    func_08A58A5C(self, &p);
    target->flags |= 1;
    func_0882FA10(&self->motion, motion, 0, 0.0f, 0.0f);
    func_08834934(self, -1, 0x66);
}

}
