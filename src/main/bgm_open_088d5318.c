// Sets up a BGM load record: id, the track entry from func_0889F3E4, its size from "bgm/<name>" and reads the file into buffer if it fits.
typedef struct { int id; int track; int buffer; int size; } Bgm44;
extern int D_089D6C30;
extern char D_089CC0A4[];
extern int func_0889F3E4(int);
extern int func_0893476C(char *, const char *, ...);
extern int func_08862C40(const char *, int);
extern void func_08862854(const char *, int, int, int);
void func_088D5318(Bgm44 *b, int id, int capacity)
{
    char path[256];
    int track, size;
    b->id = id;
    b->track = func_0889F3E4(D_089D6C30);
    track = b->track;
    if (track != 0) {
        func_0893476C(path, D_089CC0A4, track + 0x1C);
        b->size = func_08862C40(path, 1);
        size = b->size;
        if (size <= capacity && b->buffer != 0) {
            func_08862854(path, b->buffer, size, 1);
        } else {
            b->buffer = 0;
            b->size = 0;
        }
    }
}
