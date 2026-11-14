/* State callback: applies the standard slow-down rate to the fighter. */
class SlowActorBase {
public:
    unsigned char pad_000[0x544];

    virtual void slot_000();
    virtual void slot_001();
    virtual void slot_002();
    virtual void slot_003();
    virtual void slot_004();
    virtual void slot_005();
    virtual void slot_006();
    virtual void slot_007();
    virtual void slot_008();
    virtual void slot_009();
    virtual void slot_010();
    virtual void slot_011();
    virtual void slot_012();
    virtual void slot_013();
    virtual void slot_014();
    virtual void slot_015();
    virtual void slot_016();
    virtual void slot_017();
    virtual void slot_018();
    virtual void slot_019();
    virtual void slot_020();
    virtual void slot_021();
    virtual void slot_022();
    virtual void set_rate(float rate);
};

extern "C" float D_089C7C2C;

extern "C" void func_08A54E54(int unused, SlowActorBase *actor)
{
    actor->set_rate(D_089C7C2C);
}
