//
// Created by James Pickering on 3/19/25.
//

#pragma once

struct Event {
    auto accept() {
        _accepted = true;
    }

    [[nodiscard]] auto isAccepted() const {
        return _accepted;
    }

private:
    bool _accepted{false};
};