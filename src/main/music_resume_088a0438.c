/* After a power resume, restarts the saved music track when music was
   playing, otherwise clears the pending flag. */
typedef struct SavedTrack {
    char name[0x80];
    int a;
    float volume;
    unsigned char loop;
} SavedTrack;

extern int D_089D6C60;
extern int D_089D6C50;
extern SavedTrack D_09B4CEF0;

extern unsigned int func_089C5BC0(void);
extern int func_0889FD38(float volume, int a, const char *name, int b, int loop);

void func_088A0438(void)
{
    if (func_089C5BC0() & 0x20) {
        if (D_089D6C60 == 2) {
            func_0889FD38(D_09B4CEF0.volume, D_09B4CEF0.a, D_09B4CEF0.name, 1, D_09B4CEF0.loop);
        } else {
            D_089D6C50 = 0;
        }
    }
}
