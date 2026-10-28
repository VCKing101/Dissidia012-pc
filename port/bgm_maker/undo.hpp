#pragma once

// Undo and redo for the mod being edited, as whole snapshots: a mod is a few
// kilobytes, and a snapshot can never miss a field. The editor commits a
// snapshot when an edit is over (a field left, a drag released), so typing a
// word or dragging a marker is one step. MIT licence; see port/README.md.

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

namespace d012::bgm_maker {

template <class State>
class UndoStack {
public:
    explicit UndoStack(std::size_t limit = 500u) : limit_(limit) {}

    // Forgets the history; `state` is where it starts (a mod just opened or saved).
    void reset(State state) {
        done_.clear();
        undone_.clear();
        done_.push_back(std::move(state));
    }

    // Records `state` as the next step, when it differs from the last one.
    // Returns whether it did.
    bool commit(const State &state) {
        if (!done_.empty() && done_.back() == state) return false;
        done_.push_back(state);
        undone_.clear();
        if (done_.size() > limit_ + 1u) done_.erase(done_.begin());
        return true;
    }

    [[nodiscard]] bool can_undo() const noexcept { return done_.size() > 1u; }
    [[nodiscard]] bool can_redo() const noexcept { return !undone_.empty(); }

    // The state before the last step, or nothing.
    std::optional<State> undo() {
        if (!can_undo()) return std::nullopt;
        undone_.push_back(std::move(done_.back()));
        done_.pop_back();
        return done_.back();
    }

    std::optional<State> redo() {
        if (!can_redo()) return std::nullopt;
        done_.push_back(std::move(undone_.back()));
        undone_.pop_back();
        return done_.back();
    }

private:
    std::size_t limit_;
    std::vector<State> done_;    // the last one is the current state
    std::vector<State> undone_;
};

} // namespace d012::bgm_maker
