/* First voice of the mixer that matches the group and id, activated; 0 when none. */
typedef struct Voice088D0684 {
    char pad_00[0xC];
    struct Voice088D0684 *next;
} Voice088D0684;

typedef struct Mixer088D0684 {
    char pad_00[0xEBC8];
    Voice088D0684 *voices;
} Mixer088D0684;

int func_088DE1E8(Voice088D0684 *v, unsigned char group, int id);
void func_088DE198(Voice088D0684 *v);
Voice088D0684 *func_088D0684(Mixer088D0684 *m, unsigned char group, int id)
{
    Voice088D0684 *v;
    for (v = m->voices; v != 0; v = v->next) {
        if (func_088DE1E8(v, group, id) != 0) {
            func_088DE198(v);
            break;
        }
    }
    return v;
}
