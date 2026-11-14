/* Scene speed: the override object's value when the override flag is set, else the default. */
typedef struct SpeedOverride {
    char pad_00[0x18];
    float speed;
} SpeedOverride;

typedef struct SceneSpeed {
    char pad_0000[0x17BC];
    unsigned char overridden;
} SceneSpeed;

extern SpeedOverride *D_089D69E0;
extern float D_089D5EAC;

float func_088E04C4(SceneSpeed *scene)
{
    if (scene->overridden != 0) {
        return D_089D69E0->speed;
    }
    return D_089D5EAC;
}
