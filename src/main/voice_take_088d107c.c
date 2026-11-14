/* Takes a voice from the free list, resets and prepares it, and links it into the mixer. */
typedef struct Voice088D107C {
    int f0;
    int f4;
    int f8;
    char pad_0C[8];
    unsigned char flags;
    char pad_15[2];
    unsigned char state;
} Voice088D107C;

typedef struct Node088D107C {
    Voice088D107C *voice;
    struct Node088D107C *next;
} Node088D107C;

typedef struct Mixer088D107C {
    char pad_00[0x336C];
    Node088D107C *free;
} Mixer088D107C;

void func_088DA92C(Voice088D107C *v);
void func_088DB30C(Voice088D107C *v, int a, int b);
void func_088CEEC8(Mixer088D107C *m, Voice088D107C *v);
Voice088D107C *func_088D107C(Mixer088D107C *m, int a, int b)
{
    Voice088D107C *v = m->free->voice;
    m->free->voice = 0;
    m->free = m->free->next;
    func_088DA92C(v);
    v->f4 = 0;
    v->f0 = 0;
    v->f8 = 0;
    func_088DB30C(v, a, b);
    func_088CEEC8(m, v);
    v->state = 0;
    v->flags |= 1;
    return v;
}
