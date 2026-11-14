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
struct Team { char data_00[0x38]; Unit *leaders[2]; Unit *helpers[2]; };
extern "C" {
void func_08888550(Team *team)
{
    for (int i = 0; i < 2; i++) {
        if (team->leaders[i] != 0) {
            team->leaders[i]->release();
            team->leaders[i] = 0;
        }
        if (team->helpers[i] != 0) {
            team->helpers[i]->release();
            team->helpers[i] = 0;
        }
    }
}
}
