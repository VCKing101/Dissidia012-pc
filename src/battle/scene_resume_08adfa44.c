typedef struct BattleScene {
    char pad_000[0x92C];
    unsigned char paused;
} BattleScene;

extern BattleScene D_09B6B830;

void func_08ADFA44(void)
{
    BattleScene *scene = &D_09B6B830;
    scene->paused = 0;
}
