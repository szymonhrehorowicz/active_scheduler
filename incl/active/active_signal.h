/**
 * @file signal.h
 * @author Szymon Hrehorowicz
 * @brief
 * @version 0.1
 * @date 2026-08-20
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once

#include <functional>
#include <map>
#include <utility>

namespace Active
{

/**
 * @brief Signals are a way to notify listeners when something happens.
 *
 * @tparam Args
 */
template <typename... Args> class Signal
{
  public:
    Signal() = default;
    ~Signal() = default;

    Signal(Signal const & /*unused*/) {};

    Signal &operator=(Signal const &other)
    {
        if (this != &other)
        {
            disconnect_all();
        }

        return *this;
    }

    Signal(Signal &&other) noexcept : m_slots(std::move(other.m_slots)), m_current_id(other.m_current_id) {};

    Signal &operator=(Signal &&other) noexcept
    {
        if (this != &other)
        {
            m_slots = std::move(other.m_slots);
            m_current_id = other.m_current_id;
        }

        return *this;
    }

    /**
     * @brief Connect std::function to the signal.
     *
     * @param slot
     * @return int - id which can be used to disconnect the function
     */
    int connect(std::function<void(Args...)> const &slot) const
    {
        m_slots.insert(std::make_pair(++m_current_id, slot));

        return m_current_id;
    }

    /**
     * @brief Connect member function to the signal.
     *
     *
     * @tparam T
     * @param inst - instance of a class
     * @param func - member function of an instance
     * @return int - id which can be used to disconnect the member function
     */
    template <typename T> int connect(T *inst, void (T::*func)(Args...))
    {
        return connect([=](Args... args) { (inst->*func)(args...); });
    }

    /**
     * @brief Connect const member function to the signal.
     *
     * @tparam T
     * @param inst - instance of a class
     * @param func - member function of an instance
     * @return int - id which can be used to disconnect the const member function
     */
    template <typename T> int connect(T *inst, void (T::*func)(Args...) const)
    {
        return connect([=](Args... args) { (inst->*func)(args...); });
    }

    /**
     * @brief Disconnect single function from the signal.
     *
     * @param id - id of a function
     */
    void disconnect(int id) const
    {
        m_slots.erase(id);
    }

    /**
     * @brief Disconnect all previously connected functions.
     *
     */
    void disconnect_all() const
    {
        m_slots.clear();
    }

    /**
     * @brief Emit a signal to all connected functions.
     *
     * @param p
     */
    void emit(Args... p)
    {
        for (auto const &slot : m_slots)
        {
            slot.second(p...);
        }
    }

    /**
     * @brief Emit a signal to a function with given id.
     *
     * @param id - id of a function
     * @param p
     */
    void emit(int id, Args... p)
    {
        auto const &slot = m_slots.find(id);
        if (slot != m_slots.end())
        {
            slot->second(p...);
        }
    }

  private:
    mutable std::map<int, std::function<void(Args...)>> m_slots;
    mutable int m_current_id{0};
};

} // namespace Active