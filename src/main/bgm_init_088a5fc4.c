/* Initialises a music player: cleared state, its parts, no tempo and full volume. */
typedef struct Player088A5FC4 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    char track[0xC0];
    char queue[0x50];
    char fader[8];
    float tempo;
    float volume;
} Player088A5FC4;

void func_0885E7C0(void *track);
void func_08873498(void *queue);
void func_088A64DC(void *fader, Player088A5FC4 *p);

Player088A5FC4 *func_088A5FC4(Player088A5FC4 *p)
{
    p->field_04 = 0;
    p->field_00 = 0;
    p->field_08 = 0;
    func_0885E7C0(p->track);
    func_08873498(p->queue);
    func_088A64DC(p->fader, p);
    p->tempo = -1.0f;
    p->volume = 1.0f;
    return p;
}
