struct Unit {
    char data_000[0x544];
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29();
    virtual void release();   /* 0xF8 */
};
struct Mode { static bool online(signed char *m) { return *m == 1; } };
extern "C" {
extern signed char *D_089D85E8;
void func_08AD0A94(void);

void func_088E5B58(Unit **units)
{
    int i;
    if (Mode::online(D_089D85E8)) {
        func_08AD0A94();
    }
    for (i = 0; i < 32; i++, units++) {
        if (*units != 0) {
            (*units)->release();
            *units = 0;
        }
    }
}
}
