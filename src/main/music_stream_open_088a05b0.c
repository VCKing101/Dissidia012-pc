/* Opens the music stream thread for a valid track while music is not
   paused, or resets the stream state. */
typedef struct SavedTrack {
    char name[0x80];
    int a;
    float volume;
    unsigned char loop;
} SavedTrack;

extern int D_089D6C60;
extern int D_089D6C3C;
extern int D_089D6C38;
extern int D_089D6C50;
extern SavedTrack D_09B4CEF0;

extern int func_08862F2C(int stream, int sema, int size, void (*fn)(void), int a, int loop);
extern void func_0886300C(int stream);
extern void func_088A065C(void);

void func_088A05B0(int track)
{
    if (track >= 0 && D_089D6C60 != 1) {
        if (func_08862F2C(D_089D6C3C, D_089D6C38, 0x18000, func_088A065C, 0, D_09B4CEF0.loop) == 0) {
            func_0886300C(D_089D6C3C);
            D_089D6C3C = -1;
            D_089D6C50 = 0;
        } else {
            D_089D6C50 = 3;
        }
    } else {
        D_089D6C3C = -1;
        D_089D6C50 = 0;
    }
}
