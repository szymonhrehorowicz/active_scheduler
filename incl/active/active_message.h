#pragma once

#include "etl/message.h"
#include "etl/tuple.h"
#include "etl/type_traits.h"

namespace Active
{

template <etl::message_id_t ID, typename... T_Payload> class Message;

template <etl::message_id_t ID> class Message<ID> : public etl::message<ID>
{
  public:
    Message() : etl::message<ID>()
    {
    }
};

template <etl::message_id_t ID, typename... T_Payload> class Message : public etl::message<ID>
{
    static_assert(sizeof...(T_Payload) > 0, "Use Message<ID> for messages without payload");

  public:
    using payload_tuple = etl::tuple<T_Payload...>;

    Message() : etl::message<ID>()
    {
    }

    explicit Message(const T_Payload &...payload) : etl::message<ID>(), m_payload(payload...)
    {
    }

    const payload_tuple &get_payload() const &
    {
        return m_payload;
    }

    template <size_t I> const typename etl::tuple_element<I, payload_tuple>::type &get() const
    {
        static_assert(I < sizeof...(T_Payload), "Payload index out of bounds");
        return etl::get<I>(m_payload);
    }

  private:
    payload_tuple m_payload;
};

} // namespace Active