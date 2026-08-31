#include "active/active_protection.h"

namespace Active
{

Protection make_protection_t0(uint32_t code, Protection_Events level, Protection_Function function)
{
    auto &bus = Protection_Manager::instance().get_bus_t0();
    auto &reset_signal = Protection_Manager::instance().get_reset_signal();

    return Protection{code, bus, reset_signal, level, function};
}

} // namespace Active