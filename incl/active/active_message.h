#pragma once

#include "etl/message.h"
#include "etl/tuple.h"
#include "etl/type_traits.h"
#include <cstdint>
#include <cstring>

namespace Active
{

template <etl::message_id_t ID, typename... T_Payload> class Message;

template <etl::message_id_t ID> class Message<ID> : public etl::message<ID>
{
  public:
    Message() : etl::message<ID>()
    {
    }

    void to_buffer(uint8_t *buffer, std::uint32_t &size) const
    {
        buffer[0] = static_cast<uint8_t>(ID);
        size = 1;
    }

    static bool from_buffer(const uint8_t *buffer, std::uint32_t size, Message<ID> &)
    {
        return size >= 1 && buffer[0] == static_cast<uint8_t>(ID);
    }
};

template <etl::message_id_t ID, typename... T_Payload> class Message : public etl::message<ID>
{
    static_assert(sizeof...(T_Payload) > 0, "Use Message<ID> for messages without payload");
    using payload_tuple = etl::tuple<T_Payload...>;

  public:
    Message() : etl::message<ID>()
    {
    }

    explicit Message(const T_Payload &...payload) : etl::message<ID>(), m_payload(payload...)
    {
    }

    const payload_tuple &get_payload() const
    {
        return m_payload;
    }

    template <std::size_t I> const typename etl::tuple_element<I, payload_tuple>::type &get() const
    {
        static_assert(I < sizeof...(T_Payload), "Payload index out of bounds");
        return etl::get<I>(m_payload);
    }

    void to_buffer(uint8_t *buffer, std::uint32_t &size) const
    {
        uint8_t *out = buffer;
        *out++ = static_cast<uint8_t>(ID);
        serialize_tuple(out, m_payload, etl::make_index_sequence<sizeof...(T_Payload)>{});
        size = static_cast<std::uint32_t>(out - buffer);
    }

    static bool from_buffer(const uint8_t *buf, std::uint32_t sz, Message &out)
    {
        if (sz < 1 || buf[0] != static_cast<uint8_t>(ID))
            return false;

        constexpr std::size_t kPayloadSize = (sizeof(T_Payload) + ... + 0);
        if (sz < 1 + kPayloadSize)
            return false;

        const uint8_t *in = buf + 1;
        deserialize_tuple(in, out.m_payload, etl::make_index_sequence<sizeof...(T_Payload)>{});
        return true;
    }

  private:
    payload_tuple m_payload;

    template <typename Tuple, std::size_t... I>
    static void serialize_tuple(uint8_t *&out, const Tuple &tup, etl::index_sequence<I...>)
    {
        (..., copy_one(out, etl::get<I>(tup)));
    }

    template <typename T> static void copy_one(uint8_t *&out, const T &value)
    {
        static_assert(etl::is_trivially_copyable<T>::value, "Only trivially copyable types allowed");
        std::memcpy(out, &value, sizeof(T));
        out += sizeof(T);
    }

    template <typename Tuple, std::size_t... I>
    static void deserialize_tuple(const uint8_t *in, Tuple &t, etl::index_sequence<I...>)
    {
        ((copy_one_from_buffer(in, etl::get<I>(t)), in += sizeof(T_Payload)), ...);
    }

    template <typename T> static void copy_one_from_buffer(const uint8_t *ptr, T &dst)
    {
        std::memcpy(&dst, ptr, sizeof(T));
    }
};

} // namespace Active
