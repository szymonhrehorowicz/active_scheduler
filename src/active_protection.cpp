#include "active/active_protection.h"
#include <memory>

namespace Active
{

std::unique_ptr<Protection> make_protection_t0(uint32_t code, Protection_Events level, Protection_Function function)
{
    auto &bus = Protection_Manager::instance().get_bus_t0();
    auto &reset_signal = Protection_Manager::instance().get_reset_signal();

    return std::make_unique<Protection>(code, bus, reset_signal, level, function);
}

} // namespace Active