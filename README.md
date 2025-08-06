# Breakout ECS 

Un clon de **Breakout** implementado con **ECS** (EnTT), **Raylib** y ImGUI en C++.

---

## Integración con ImGui
Con la integración de ImGui ahora se tienen 3 paneles: entidades, componentes y controles.
En el panel izquierdo se observan todos las entidades, al seleccionar una nos muestra los componentes de dicha entidad en el panel derecho y en el panel de controles se puede realizar una pausa del juego, en este modo también es posible editar los valores de los paneles.


## Descripción 

- El jugador controla un **paddle** que solo se mueve horizontalmente.
- Hay una **pelota** que rebota contra paredes, paddle y bloques.
- Cada rebote contra paredes o paddle acelera ligeramente la pelota.
- Si la pelota toca el suelo, pierdes.
- Cuando destruyes todos los bloques, ganas.

---

## Demostraciones

| Jugando                                      | 
|---------------------------------------------|
| ![Gameplay](gameplay.gif)                    |

|You Win                                    |
|--------------------------------------------|
| ![You Win](you_win.gif)                    | 

| You Lost                 |
|--------------------------|
| ![You Lose](you_lost.gif) |






## Estructura del Proyecto
```plaintext
Breakout-ECS/
├── CMakeLists.txt
├── external/
│   └── entt/…            # headers de EnTT
├── src/
│   ├── Game/…            # Clase base Game
│   ├── Breakout/…        # Breakout, Systems
│   ├── ECS/…             # Components, Entity, System
│   └── Scene/…           # Scene.h/.cpp
│   └── main.cpp
└── README.md
```
