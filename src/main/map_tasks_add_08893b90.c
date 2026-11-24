/* Registers the map's update, draw, draw-after-characters and culling tasks
   with the given ids and priorities. */
extern char D_089CA7B8[];
extern char D_089CA7C8[];
extern char D_089CA7D8[];
extern char D_089CA7F0[];

extern void func_088823C4(int owner, int id, int priority, void (*fn)(void), const char *name);
extern void func_08893558(void);
extern void func_08893710(void);
extern void func_08893730(void);
extern void func_088935CC(void);

void func_08893B90(int owner, int update_id, int update_priority, int draw_id, int draw_priority,
                   int after_id, int after_priority, int cull_id, int cull_priority)
{
    func_088823C4(owner, update_id, update_priority, func_08893558, D_089CA7B8);
    func_088823C4(owner, draw_id, draw_priority, func_08893710, D_089CA7C8);
    func_088823C4(owner, after_id, after_priority, func_08893730, D_089CA7D8);
    func_088823C4(owner, cull_id, cull_priority, func_088935CC, D_089CA7F0);
}
