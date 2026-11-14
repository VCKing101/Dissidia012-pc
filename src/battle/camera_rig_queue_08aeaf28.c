typedef struct CameraRig {
    char pad_000[0x914];
    float offset_z;
    char pad_918[0x14];
    unsigned char enabled;
} CameraRig;

extern unsigned char D_08AEB9C0;
extern char D_08AEB028[];
extern void func_0885C3BC(void *list, CameraRig *rig);

/* Queue the rig when the camera is on, the rig enabled and lifted. */
void func_08AEAF28(CameraRig *self)
{
    if (D_08AEB9C0 == 1 && self->enabled == 1 && self->offset_z > 0.0f) {
        func_0885C3BC(D_08AEB028, self);
    }
}
