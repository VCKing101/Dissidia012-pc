/* First voice of the mixer that matches the group and id, activated; 0 when none. */
typedef struct Voice088D0608 {
    char pad_00[0x6C];
    struct Voice088D0608 *next;
} Voice088D0608;

typedef struct Mixer088D0608 {
    char pad_00[0xC4F4];
    Voice088D0608 *voices;
} Mixer088D0608;

int func_088DCDF8(Voice088D0608 *v, unsigned char group, int id);
void func_088DCD50(Voice088D0608 *v);
Voice088D0608 *func_088D0608(Mixer088D0608 *m, unsigned char group, int id)
{
    Voice088D0608 *v;
    for (v = m->voices; v != 0; v = v->next) {
        if (func_088DCDF8(v, group, id) != 0) {
            func_088DCD50(v);
            break;
        }
    }
    return v;
}
