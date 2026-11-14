/* Restarts the player's voice with the cue of its record. */
typedef struct Player088CC77C {
    int sound;
    int bank;
    int voice;
    int id;
} Player088CC77C;

typedef struct Rec088CC77C {
    short id;
    short field_02;
    short cue;
} Rec088CC77C;

Rec088CC77C *func_088B29D8(int id);
void func_0887B944(int voice);
void func_0887BA5C(int voice);
int func_0887AD78(int bank, int a, int cue, int b, int c, int d);
void func_0887B924(int voice);
void func_088CC77C(Player088CC77C *p)
{
    Rec088CC77C *rec = func_088B29D8(p->id);
    if (rec != 0 && rec->cue != -1) {
        if (p->voice != 0) {
            func_0887B944(p->voice);
            func_0887BA5C(p->voice);
            p->voice = 0;
        }
        if (p->bank != 0) {
            p->voice = func_0887AD78(p->bank, 0, rec->cue, 0, 0, 0);
            if (p->voice != 0) {
                func_0887B924(p->voice);
            }
        }
    }
}
