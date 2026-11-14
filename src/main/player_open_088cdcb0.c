/* Opens the player's stream on the data; -1 for no data, -2 when the stream fails. */
typedef struct Player088CDCB0 {
    char pad_00[0x20];
    unsigned char paused;
    char pad_21[7];
    unsigned char loaded;
    char pad_29[3];
    int stream;
} Player088CDCB0;

typedef struct Config088CDCB0 {
    char pad_00[0xC];
    int heap;
} Config088CDCB0;

extern Config088CDCB0 *D_089D6998;
void func_088CDC60(Player088CDCB0 *p);
int func_0887AC70(int *stream, void *data, int size, int heap);
int func_088CDCB0(Player088CDCB0 *p, void *data, int size)
{
    func_088CDC60(p);
    if (data == 0 || size <= 0) {
        return -1;
    }
    if (func_0887AC70(&p->stream, data, size, D_089D6998->heap) == 0) {
        return -2;
    }
    p->paused = 1;
    p->loaded = 1;
    return 0;
}
