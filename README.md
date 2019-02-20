# ecs
easy, concise, succulent "entity-component-system"

// world apparently cannot be forward declared
// 1) a type alias cannot be forward declared
//    - makes types from 'ecs::make_component' not possible to forward declare
//    - makes an alias like 'world = ecs::world<Ts...>' unable to be used
// 2) a template funciton cannot be defined in a .cpp file
//    - makes forward declaring into a .cpp file impossible