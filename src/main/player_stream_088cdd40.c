/* The player's stream when it is open; 0 otherwise. */
typedef struct Player088CDD40 {
    char pad_00[0x2C];
    int stream;
} Player088CDD40;

int func_0887AFB0(int *stream);

int *func_088CDD40(Player088CDD40 *p)
{
    int *stream = &p->stream;
    if (func_0887AFB0(stream) == 0) {
        return 0;
    }
    return stream;
}
