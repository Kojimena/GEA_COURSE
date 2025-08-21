struct EnemyAIComponent {
    float speed  = 60.0f;  // px/seg
    float leftX  = 80.0f;  // límite izq
    float rightX = 320.0f; // límite der
    int dir = 1;           // 1=derecha, -1=izquierda
};