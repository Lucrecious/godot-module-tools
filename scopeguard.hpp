#pragma once

#include <functional>

class ScopeGuard {
    using func = std::function<void(void)>;
public:
    ScopeGuard(func on_scope_end) {
        _on_scope_end = on_scope_end;
    }

    ~ScopeGuard() { _on_scope_end(); }

private:
    func _on_scope_end;
};