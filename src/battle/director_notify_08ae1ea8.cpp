class SceneDirector {
public:
    char pad_00[0x24];
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual void slot_040();
    virtual void slot_048(void *owner);
};

extern "C" {
extern SceneDirector *D_08AE7CA0;
extern void func_08AE31F8(void *self, int id);

void func_08AE1EA8(void *self)
{
    func_08AE31F8(self, 0x8C);
    D_08AE7CA0->slot_030();
}

}
