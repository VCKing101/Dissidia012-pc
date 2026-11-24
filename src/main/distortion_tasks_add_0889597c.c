/* Registers the map distortion manager's update and draw tasks. */
extern char D_089CA8D0[];
extern char D_089CA8F0[];

extern void func_088823C4(int owner, int id, int priority, void (*fn)(void), const char *name);
extern void func_088959E8(void);
extern void func_08895A08(void);

void func_0889597C(int owner, int update_id, int update_priority, int draw_id, int draw_priority)
{
    func_088823C4(owner, update_id, update_priority, func_088959E8, D_089CA8D0);
    func_088823C4(owner, draw_id, draw_priority, func_08895A08, D_089CA8F0);
}
