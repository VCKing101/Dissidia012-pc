/* Stops the music player (fading out when it is playing) and resets the track. */
typedef struct Player0886FF1C {
    char pad_00[0x14];
    int fade;
    char pad_18[0x88];
    unsigned char playing;
} Player0886FF1C;

extern Player0886FF1C D_09B26B18;
extern int D_089D5FBC;

int func_088707F0(Player0886FF1C *p);
void func_0886FDA4(Player0886FF1C *p);
void func_089C6000(int arg);

int func_0886FF1C(void)
{
    Player0886FF1C *p = &D_09B26B18;
    if (p->playing) {
        p->fade = func_088707F0(p);
    } else {
        func_0886FDA4(p);
    }
    D_089D5FBC = -1;
    func_089C6000(0);
    return 0;
}
