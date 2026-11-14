/* Moves a value towards its limit by speed * dt and stops at the limit. */
typedef struct Mover088A1B68 {
    float value;
    float limit;
    float speed;
} Mover088A1B68;

void func_088A1B68(Mover088A1B68 *m, float dt)
{
    float speed = m->speed;
    float zero = 0.0f;
    if (!(speed <= zero)) {
        m->value = m->value + m->speed * dt;
        if (!(m->value < m->limit)) {
            m->value = m->limit;
            m->speed = zero;
        }
    } else if (speed < zero) {
        m->value = m->value + m->speed * dt;
        if (m->value <= m->limit) {
            m->value = m->limit;
            m->speed = zero;
        }
    }
}
