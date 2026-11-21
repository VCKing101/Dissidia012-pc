/* Model slot used by the stage object for its "SUPER SKILL" models (0x130 bytes each). */
typedef struct StageSkillModel {
    char data[0x130];
} StageSkillModel;

typedef struct Stage08892A78 {
    char unk_000[0x140];
    StageSkillModel skill_models[1];   /* +0x140 */
} Stage08892A78;

extern char D_089CA77C[];              /* "SUPER SKILL" */

/* stage_model_setup(slot, name, data, size): holder flags 1 | 8 (textures go to VRAM). */
extern void func_08894F38(StageSkillModel *slot, const char *name, void *data, int size);

/* stage_super_skill_load: set up the index-th "SUPER SKILL" model from a GMO file in memory. */
void func_08892A78(Stage08892A78 *stage, void *data, int size, int index)
{
    func_08894F38(&stage->skill_models[index], D_089CA77C, data, size);
}
