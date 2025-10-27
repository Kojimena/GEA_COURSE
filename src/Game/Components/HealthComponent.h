#pragma once

struct HealthComponent {
    float maxHealth = 100.0f;
    float currentHealth = 100.0f;
    float poisonDamageRate = 10.0f;
    float lastDamageTime = 0.0f;
    float damageCooldown = 1.0f;
};
