# Active Object Framework Class Diagram

```plantuml
@startuml Active Object Framework

' External ETL classes
abstract class "etl::task" as EtlTask
abstract class "etl::message<ID>" as EtlMessage
abstract class "etl::imessage_router" as EtlMessageRouter
class "etl::imessage_bus" as EtlMessageBus
class "etl::queue_spsc_atomic<T,Size>" as EtlQueue

' Active Framework Interfaces
namespace Active {
    abstract class Object_Interface {
        + Object_Interface(priority: uint32_t)
        + {abstract} task_request_work(): uint32_t
        + {abstract} task_process_work(): void
        + {abstract} get_internal_router(): Router_Interface&
    }

    abstract class Router_Interface {
        + Router_Interface(id: message_router_id_t, owner: Object_Interface&)
        + {abstract} process_queue(): void
        + {abstract} get_queue_size(): uint32_t
        # m_owner: Object_Interface&
    }

    struct Router_Stats {
        + overflow_counter: uint32_t
        + unknown_message_counter: uint32_t
    }

    ' Template Classes
    class "Object<RouterType>" as Object {
        + Object(priority: uint32_t, router_id: message_router_id_t, public_bus: etl::imessage_bus&)
        # m_router: RouterType
        # m_public_bus: etl::imessage_bus&
    }

    class "Router<Queue_Size,T_Derived,T_Message_Types...>" as Router {
        + Router(id: message_router_id_t, owner: Object_Interface&)
        + receive(msg: etl::imessage&): void
        + process_queue(): void
        + get_queue_size(): uint32_t
        + accepts(id: message_id_t): bool
        # on_receive_unknown(msg: etl::imessage&): void
        - m_router: Router_Impl
        - m_queue: etl::queue_spsc_atomic
        - m_router_stats: Router_Stats
    }

    class "Message<ID>" as MessageNoPayload {
        + Message()
    }

    class "Message<ID,T_Payload...>" as MessageWithPayload {
        + Message()
        + Message(payload: T_Payload...)
        + get_payload(): payload_tuple&
        + get<I>(): auto&
        - m_payload: payload_tuple
    }
}

' Inheritance relationships
EtlTask <|-- Active.Object_Interface
EtlMessageRouter <|-- Active.Router_Interface
Active.Object_Interface <|-- Active.Object
Active.Router_Interface <|-- Active.Router
EtlMessage <|-- Active.MessageNoPayload
EtlMessage <|-- Active.MessageWithPayload

' Composition/Association relationships
Active.Object *-- Active.Router : owns >
Active.Router *-- Active.Router_Stats : has >
Active.Router o-- EtlQueue : uses >
Active.Object o-- EtlMessageBus : uses >

note "Queue_Size: 0 < size <= 256\nT_Message_Types: max 16 types" as N1
N1 .. Active.Router

note "Template specialization for\nmessages with/without payload" as N2
N2 .. Active.MessageWithPayload

@enduml
```

## Key Components

1. **Object System**
   - `Object_Interface`: Base class for all active objects, inherits from ETL task system
   - `Object<RouterType>`: Template implementation that ties an active object to its router

2. **Router System**
   - `Router_Interface`: Base class for message routing, inherits from ETL message router
   - `Router<Queue_Size,T_Derived,T_Message_Types...>`: Template implementation with atomic message queue
   - `Router_Stats`: Statistics tracking for overflow and unknown messages

3. **Message System**
   - `Message<ID>`: Specialization for messages without payload
   - `Message<ID,T_Payload...>`: Specialization for messages with typed payload

## Design Features

- **Type Safety**: Template parameters ensure type-safe message handling
- **Memory Safety**: Fixed-size queues and compile-time size checks
- **Real-Time**: Lock-free queue implementation for ISR safety
- **Monitoring**: Built-in statistics for queue overflow and unknown messages
- **Flexibility**: Support for both payload and non-payload messages