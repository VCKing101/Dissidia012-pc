extern char D_089C8F5C[];              /* "dlc_%03doe.bin" */
extern char D_089C8F6C[];
extern char D_089C8F78[];              /* "dlc/obj/dlc_%03doe.bin" */
extern char D_089C8F10[];              /* "%s/%s.edat" */
extern int func_08862678(char *dst, const char *a, const char *b);   /* game_path */
extern const char *func_08861B84(const char *path);
extern int func_0893476C(char *dst, const char *format, ...);         /* sprintf */

/* The .edat path of DLC file number `index` (objects); 0 when the number is out of range. */
int func_08862288(void *self, char *out, int index)
{
    char dir[0x100];
    char name[0x100];
    char path[0x40];

    if (index < 0x100) {
        func_0893476C(name, D_089C8F5C, index);
        func_08862678(dir, D_089C8F6C, name);
        func_0893476C(path, D_089C8F78, index);
        func_0893476C(out, D_089C8F10, dir, func_08861B84(path));
        return 1;
    }
    return 0;
}
