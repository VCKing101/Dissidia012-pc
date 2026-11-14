/* Restarts a ready, idle or finished player; -1 when not ready, -2 when busy. */
typedef struct Player088D727C {
    char pad_00[0x10];
    int state;
    char pad_14[8];
    unsigned char done;
    char pad_1D[3];
    int time;
    char pad_24[0x38];
    int stream;
} Player088D727C;

int func_088D723C(Player088D727C *p);
void func_088D9518(int *stream);
int func_088D727C(Player088D727C *p)
{
    if (func_088D723C(p) == 0) {
        return -1;
    }
    if (p->state != 0 && p->state != 5) {
        return -2;
    }
    func_088D9518(&p->stream);
    p->state = 1;
    p->done = 0;
    p->time = 0;
    return 0;
}
