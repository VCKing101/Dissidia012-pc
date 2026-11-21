/* A GMO chunk header (type 0x8000 = short header). */
struct GmoChunk {
    unsigned short type;
    unsigned short header_size;
    unsigned int size;
    unsigned int unk_08;
    unsigned int data_offset;
};

struct GameState08892AC0 {
    int active;                        /* +0x00 */
};

extern "C" {
extern char D_089CA778[];              /* "" */
extern char D_089CA788[];              /* "INS_" */
extern char D_089CA790[];              /* "DES_" */
void *func_08940490(void *file, int size);                       /* gmo_file_body */
GmoChunk *func_08940528(void *chunk, int type, int index);       /* gmo_chunk_find */
int func_089404A8(void *chunk, int type);                        /* gmo_chunk_count */
int func_08935B50(const char *a, const char *b, int n);          /* strncmp */
GameState08892AC0 *func_08899300(void);
void func_088989F4(GameState08892AC0 *state, const char *name, int bone);
}

struct StageBones {
    static const char *name_of(GmoChunk *bone) {
        if (bone->type & 0x8000) {
            return D_089CA778;
        }
        return (const char *)bone + 0x10;
    }
    static bool active() { return func_08899300()->active != 0; }
};

extern "C" {

/* stage_bones_register: hand the bones (GMO block 4) named "INS_*" or "DES_*" of the first
 * model (block 3) of a stage file to the game state. */
void func_08892AC0(void *self, void *file, int size)
{
    void *body = func_08940490(file, size);

    if (body != 0) {
        GmoChunk *model = func_08940528(body, 3, 0);
        if (model != 0) {
            int count = func_089404A8(model, 4);
            for (int i = 0; i < count; i++) {
                const char *name = StageBones::name_of(func_08940528(model, 4, i));

                if (func_08935B50(name, D_089CA788, 4) == 0) {
                    if (StageBones::active()) {
                        func_088989F4(func_08899300(), name, i);
                    }
                } else if (func_08935B50(name, D_089CA790, 4) == 0) {
                    if (StageBones::active()) {
                        func_088989F4(func_08899300(), name, i);
                    }
                }
            }
        }
    }
}

}
