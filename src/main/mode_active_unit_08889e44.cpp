/* Returns the first of the mode's two units whose actor reports true from
   virtual 39, or 0. */
struct ModeActor {
    char data_000[0x544];
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
    virtual int v39();
};

struct ModeCtl {
    char pad_00[0x38];
    ModeActor *units[2];
};

extern "C" {

extern ModeCtl *D_089D6A10;

ModeActor *func_08889E44(void)
{
    for (int i = 0; i < 2; i++) {
        ModeActor *unit = D_089D6A10->units[i];
        if (unit != 0) {
            if (unit->v39()) {
                return unit;
            }
        }
    }
    return 0;
}

}
