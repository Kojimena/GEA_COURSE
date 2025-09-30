#include "ColliderRenderSystem.h"

void ColliderRenderSystem::render() {
    auto view = scene->r.view<TransformComponent, ColliderComponent>();

    for (auto e : view) {
        const auto& tf = view.get<TransformComponent>(e);
        const auto& col = view.get<ColliderComponent>(e);

        float x = tf.position.x + col.ox;
        float y = tf.position.y + col.oy;
        DrawRectangleLines((int)x, (int)y, (int)col.w, (int)col.h, BLUE);
    }
}
