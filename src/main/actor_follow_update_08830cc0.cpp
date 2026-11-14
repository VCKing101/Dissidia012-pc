/* Updates an actor's follow camera target: follows the actor's lock-on
   point while it is in stance 1 or 2, otherwise resets. */
struct Vec4_08830CC0 {
    float x, y, z, w;
};

struct Target08830CC0 {
    char pad_000[0x100];
    Vec4_08830CC0 point;
};

struct ActorBase08830CC0 {
    char data_000[0x90];
    Vec4_08830CC0 pos;
    char data_0A0[0x100 - 0xA0];
    float matrix[16];
    char data_140[0x180 - 0x140];
    short stance;
    char data_182[0x300 - 0x182];
    void *lock;
    char data_304[0x544 - 0x304];
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
    virtual Target08830CC0 *lock_target();
};

struct Actor08830CC0 : ActorBase08830CC0 {
    char data_548[0x840 - 0x548];
    char follow[1];
};

extern "C" {

void func_08830938(char *follow);
void func_089A38E0(Vec4_08830CC0 *dst, Vec4_08830CC0 *a, float *m);
float func_08838300(Actor08830CC0 *actor);
void func_08830954(char *follow, Vec4_08830CC0 *from, Vec4_08830CC0 *to, float dt);

void func_08830CC0(Actor08830CC0 *self)
{
    char *follow = self->follow;
    if (self->lock == 0 || (self->stance != 1 && self->stance != 2)) {
        func_08830938(follow);
    } else {
        Vec4_08830CC0 to;
        func_089A38E0(&to, &self->lock_target()->point, self->matrix);
        func_08830954(follow, &self->pos, &to, func_08838300(self));
    }
}

}
