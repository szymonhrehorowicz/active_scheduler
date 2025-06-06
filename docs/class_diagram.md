# ETL Investigations Class Diagram

This diagram shows the relationships between classes in the ETL Investigations project.

```plantuml
@startuml ETL Task Scheduler

' Base interfaces
interface "etl::task" as Task {
  + {abstract} task_request_work(): uint32_t
  + {abstract} task_process_work(): void
  + on_task_added(): void
  # task_running: bool
  # task_priority: uint8_t
}

interface "etl::ischeduler" as IScheduler {
  + start(): void
  + set_idle_callback(callback: etl::ifunction<void>&)
  + set_scheduler_running(running: bool)
  # scheduler_running: bool
  # scheduler_exit: bool
}

interface "etl::message_router" as MessageRouter {
  + {abstract} receive(msg: etl::imessage&)
  + {abstract} on_receive_unknown(msg: etl::imessage&)
}

interface "etl::imessage_bus" as IMessageBus {
  + subscribe(subscriber)
  + unsubscribe(subscriber)
  + receive(message)
}

' Policy class
class "etl::scheduler_policy_sequential_single" as SchedulerPolicy {
  + schedule_tasks(task_list: etl::ivector<etl::task*>&): bool
}

' Our implementation classes
class ActiveScheduler {
  + ActiveScheduler()
  + add_task(task: ActiveTask&)
  + get_router(): SchedulerRouter&
  + idle_callback(): void
  - callback: etl::function_mv
  - m_router: SchedulerRouter
}

class ActiveTask {
  + ActiveTask(priority: int, router_id: int)
  + task_request_work(): uint32_t
  + task_process_work(): void
  + on_receive(msg: IncrementWorkMessage)
  + on_receive(msg: DecrementWorkMessage)
  # m_work: int
}

class SchedulerRouter {
  + SchedulerRouter(router_id: int)
  + receive(msg: etl::imessage&)
  + process_queue()
  + get_internal_bus(): etl::imessage_bus&
  - m_queue: etl::queue_spsc_atomic<message_packet, 10>  
  - m_internal_bus: etl::message_bus<NUM_OF_TASKS>
}

class SomeTask {
  + SomeTask(priority: int, id: int)
  + task_process_work(): void
  - m_id: int
}

' Message types
class "etl::imessage" as IMessage {
  + get_message_id(): id_t
}

class IncrementWorkMessage {
  + static const id: uint8_t
}

class DecrementWorkMessage {
  + static const id: uint8_t
}

' Inheritance relationships
IScheduler <|-- ActiveScheduler
Task <|-- ActiveTask
MessageRouter <|-- ActiveTask
MessageRouter <|-- SchedulerRouter
ActiveTask <|-- SomeTask
IMessage <|-- IncrementWorkMessage
IMessage <|-- DecrementWorkMessage

' Usage relationships
SchedulerPolicy <-- ActiveScheduler : uses
ActiveScheduler *-- SchedulerRouter : owns >
SchedulerRouter o-- IMessageBus : manages >
ActiveTask o-- IMessageBus : subscribes to <
IMessageBus ..> IMessage : routes >

' Message flow
SchedulerRouter "1" --> "*" ActiveTask : routes messages to >

note top of IMessage
  Base interface for all messages
  flowing through the system
end note

note left of ActiveTask
  Base class for all tasks that can
  receive messages and do work
end note

note right of SchedulerRouter
  Handles thread-safe message passing
  between interrupts and main thread
  using atomic queue
end note

note bottom of ActiveScheduler
  Manages task scheduling and
  idle processing of queued messages.
  Uses router to distribute messages.
end note

@enduml
```

## Class Descriptions

### Core Components

- **SchedulerRouter**: Manages thread-safe message passing between interrupts and the main thread using an atomic queue
- **ActiveScheduler**: Coordinates task execution and manages message routing during idle time
- **ActiveTask**: Base class for tasks that can receive and process work messages
- **SomeTask**: Concrete task implementation with specific work processing behavior

### Message Infrastructure

- **etl::imessage**: Base interface for all messages in the system 
- **IncrementWorkMessage/DecrementWorkMessage**: Concrete message types for work control
- **etl::imessage_bus**: Message distribution system connecting routers to tasks
- **etl::queue_spsc_atomic**: Thread-safe queue for interrupt handling

### Key Relationships

1. Task Hierarchy:
   - Tasks inherit core functionality from `etl::task`
   - `ActiveTask` adds message handling capabilities
   - `SomeTask` provides concrete implementation

2. Message Flow:
   - Messages flow from interrupts through `SchedulerRouter`'s atomic queue
   - Router distributes messages via its internal message bus
   - Tasks receive messages through message bus subscription

3. Scheduler Integration:
   - `ActiveScheduler` coordinates task execution
   - Uses sequential single policy for task scheduling
   - Processes queued messages during idle time through router
