/* Looks up the cue for `name` and plays it from the resource's bank.
   (Not a stack-spill function: the zero passed second is a named local, which
   SNC loads with ori.) */
typedef struct Cue08AB1800 {
    int unknown_00;
    unsigned short volume;
    short unknown_06;
    int index;
} Cue08AB1800;

typedef struct Bank08AB1800 {
    int unknown_00;
    int unknown_04;
    void *player;
    void *sounds[1];
} Bank08AB1800;

Cue08AB1800 *func_08AB0F80(char *name, int flags, int first);
void func_0887B3EC(void *player, void *sound, int a, unsigned short volume, int b, int c);

void func_08AB1800(char *name, Bank08AB1800 *bank)
{
    int none = 0;
    Cue08AB1800 *cue = func_08AB0F80(name, none, *name);
    func_0887B3EC(bank->player, bank->sounds[cue->index], 0, cue->volume, 0, 0);
}
