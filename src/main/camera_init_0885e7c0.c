typedef struct Vec4 { float x; float y; float z; float w; } Vec4;
typedef struct Camera { char base[0x40]; Vec4 position; Vec4 target; Vec4 scale; } Camera;

extern void func_089A3EBC(Camera *self);
extern void func_089A3890(Vec4 *v);

Camera *func_0885E7C0(Camera *self)
{
    Vec4 *position = &self->position;
    Vec4 *target = &self->target;
    Vec4 *scale = &self->scale;
    func_089A3EBC(self);
    func_089A3890(position);
    func_089A3890(target);
    func_089A3890(scale);
    self->position.z = 1.0f;
    self->scale.y = 1.0f;
    return self;
}
