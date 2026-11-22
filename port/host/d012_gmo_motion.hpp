#pragma once

// The skeletons and animations of the game's GMO models (Sony's
// "OMG.00.1PSP"): a fighter's bones, its rest and bind poses and its
// motions, and one fighter's motion rebuilt for another fighter's skeleton.
// This is tools/gmo_motion.py and tools/move_retarget.py, operation for
// operation, so the bytes are the same as theirs and the same on every
// platform, which online play needs. Nothing here reads files, logs or
// needs PortableKit. MIT licence; see port/README.md.

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace d012::gmo_motion {

// A model that is not as expected, or a name it does not have.
struct Error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

using Vec3 = std::array<double, 3>;
using Quat = std::array<double, 4>;  // x, y, z, w

// A rotation and a translation: a bone's own values, or where it is in the
// model.
struct Placement {
    Quat rotate{0.0, 0.0, 0.0, 1.0};
    Vec3 translate{};
};

struct Bone {
    std::string name;  // the file's bytes
    int parent{-1};    // the parent's index; -1 for none
    Vec3 translate{};
    Quat rotate{0.0, 0.0, 0.0, 1.0};
};

// One curve of a motion: keys of a frame and `width` values each.
struct Curve {
    std::uint32_t interpolation{};  // 0 constant, 1 linear, 2 Hermite, 3 cubic, 4 spherical
    std::size_t width{};
    std::vector<double> frames;
    std::vector<double> values;     // `width` per key
};

struct Motion {
    // The curves a bone's rotation and translation follow: indices into
    // `curves`, -1 for none.
    struct Track {
        int rotate{-1};
        int translate{-1};
    };
    std::string name;
    std::optional<std::array<double, 2>> loop;  // the first and last frame
    std::vector<Curve> curves;
    std::vector<Track> tracks;  // by bone index
};

// gmo_motion.Skeleton: a fighter model's bones, rest and bind poses, and
// motions. It keeps its own copy of the model.
class Skeleton {
public:
    explicit Skeleton(std::span<const std::uint8_t> gmo);  // throws Error

    [[nodiscard]] const std::vector<Bone> &bones() const noexcept { return bones_; }
    // The bone called `name`: the last one, when several share it.
    [[nodiscard]] std::optional<std::size_t> find(std::string_view name) const;
    // Every bone's index, parents before their children.
    [[nodiscard]] const std::vector<std::size_t> &order() const noexcept { return order_; }
    // Where each bone is in the model at rest, and in the pose the mesh was
    // skinned in.
    [[nodiscard]] const std::vector<Placement> &rest() const noexcept { return rest_; }
    [[nodiscard]] const std::vector<Placement> &bind() const noexcept { return bind_; }

    [[nodiscard]] bool has_motion(std::string_view name) const;
    // The motion called `name`, read from the model; throws Error without one.
    [[nodiscard]] Motion motion(std::string_view name) const;
    // Each bone's own rotation and translation at a frame of a motion.
    [[nodiscard]] std::vector<Placement> pose(const Motion &motion, double frame) const;
    // Where each bone is in the model, from every bone's own values.
    [[nodiscard]] std::vector<Placement> world(const std::vector<Placement> &local) const;

private:
    struct Stored {
        std::string name;
        std::size_t begin{};  // its chunks, in file_
        std::size_t end{};
    };
    std::vector<std::uint8_t> file_;
    std::vector<Bone> bones_;
    std::vector<std::size_t> order_;
    std::vector<Placement> rest_;
    std::vector<Placement> bind_;
    std::vector<Stored> motions_;
};

// A bone's own values for the whole motion: how a weapon sits in the hand.
// A value left out is the bone's rest value.
struct Held {
    std::optional<std::array<double, 4>> rotate;
    std::optional<std::array<double, 3>> translate;
};

struct ScaleKey {
    double frame{};
    std::array<double, 3> value{};
};

// move_retarget.retarget's arguments. Frame k of the new motion shows the
// source's frame start + k * speed.
struct Retarget {
    std::string motion;      // the source's motion name
    int frames{};            // the new motion runs from frame 0 to this one
    bool mirror{};           // done with the other hand
    double speed{1.0}, start{0.0}, root_scale{1.0}, trans_scale{1.0};
    std::string name;        // the new motion's name; the source's when empty
    // In the recipe's order (a JSON object's order, which is Python's dict
    // order). A name given twice counts once, with its last value.
    std::vector<std::pair<std::string, std::string>> follow;  // target bone, the source bone it follows
    std::vector<std::pair<std::string, Held>> held;
    std::vector<std::pair<std::string, std::vector<ScaleKey>>> scales;  // scale keys, written as they are
};

// The new motion chunk's bytes: what
// gmo_motion.chunk_bytes(move_retarget.retarget(...)) returns. Throws Error.
[[nodiscard]] std::vector<std::uint8_t> retarget(const Skeleton &source, const Skeleton &target,
                                                 const Retarget &options);

} // namespace d012::gmo_motion
