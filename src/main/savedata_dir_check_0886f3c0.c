extern const char *D_089D6010;         /* the game's save directory prefix */
extern char D_089C9628[];              /* "ms0:/PSP/SAVEDATA/%s%s" */
extern void *func_08935828(void *dst, int c, unsigned int n);         /* memset */
extern int func_0893476C(char *dst, const char *format, ...);         /* sprintf */
extern int func_089A5674(const char *path);

/* Look up a save directory by name ("ms0:/PSP/SAVEDATA/<prefix><name>"); 0 without a name. */
int func_0886F3C0(void *self, const char *name)
{
    char path[0x40];

    if (name != 0) {
        func_08935828(path, 0, 0x40);
        func_0893476C(path, D_089C9628, D_089D6010, name);
        return func_089A5674(path);
    }
    return 0;
}
