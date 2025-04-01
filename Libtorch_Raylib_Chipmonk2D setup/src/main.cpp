#include "raymath.h"

#include <torch/torch.h>
#include <iostream>
#include <random>
#include <algorithm>
#include "raylib.h"

constexpr int GRID_SIZE = 4;
constexpr int NUM_ACTIONS = 4;
constexpr int NUM_EPISODES = 1000;
constexpr float LEARNING_RATE = 0.1;
constexpr float DISCOUNT_FACTOR = 0.99;
constexpr float EPSILON_ = 0.1;
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 800;

// Physics-inspired constants
constexpr float MOVE_TIME = 0.2f; // Time to transition between cells (seconds)
constexpr float FRICTION_FACTOR = 0.9f; // Friction-like effect during movement

class GridWorld {
private:
    Vector2 agent_pos;      // Agent's current position in pixels
    Vector2 target_pos;     // Target position (center of the next grid cell)
    Vector2 goal_pos;       // Goal position (final state)
    bool is_moving;         // Whether the agent is currently moving
    float move_timer;       // Timer for smooth transitions

public:
    GridWorld() {
        reset();
    }

    void step(int action) {
        if (!is_moving) { // Only allow new moves if the agent is not already moving
            // Determine target position based on action
            float cell_size_x = SCREEN_WIDTH / GRID_SIZE;
            float cell_size_y = SCREEN_HEIGHT / GRID_SIZE;

            if (action == 0 && target_pos.y > cell_size_y / 2) {           // Up
                target_pos.y -= cell_size_y;
            } else if (action == 1 && target_pos.y < SCREEN_HEIGHT - cell_size_y / 2) { // Down
                target_pos.y += cell_size_y;
            } else if (action == 2 && target_pos.x > cell_size_x / 2) {   // Left
                target_pos.x -= cell_size_x;
            } else if (action == 3 && target_pos.x < SCREEN_WIDTH - cell_size_x / 2) { // Right
                target_pos.x += cell_size_x;
            }

            is_moving = true; // Start moving toward the new target position
            move_timer = MOVE_TIME; // Reset the movement timer
        }
    }

    void update() {
        if (is_moving) {
            // Calculate the fraction of time remaining for the move
            float t = std::clamp(1.0f - (move_timer / MOVE_TIME), 0.0f, 1.0f);

            // Smoothly interpolate between current position and target position
            agent_pos.x += (target_pos.x - agent_pos.x) * t * FRICTION_FACTOR;
            agent_pos.y += (target_pos.y - agent_pos.y) * t * FRICTION_FACTOR;

            move_timer -= GetFrameTime(); // Decrease the timer

            if (move_timer <= 0.0f) {
                // Snap to the exact target position when the timer ends
                agent_pos = target_pos;
                is_moving = false; // Stop moving
            }
        }
    }

    void reset() {
        // Reset agent position and velocity to start (top-left corner)
        agent_pos = {SCREEN_WIDTH / GRID_SIZE / 2, SCREEN_HEIGHT / GRID_SIZE / 2};
        target_pos = agent_pos; // Initially, the target is the same as the current position
        goal_pos = {SCREEN_WIDTH - SCREEN_WIDTH / GRID_SIZE / 2, SCREEN_HEIGHT - SCREEN_HEIGHT / GRID_SIZE / 2};
        is_moving = false;
        move_timer = 0.0f;
    }

    bool is_terminal() const {
        // Check if the agent has reached the goal position
        return Vector2Distance(agent_pos, goal_pos) < 1.0f; // Allow a small margin of error
    }

    Vector2 get_agent_position() const {
        return agent_pos;
    }

    Vector2 get_goal_position() const {
        return goal_pos;
    }
};

void draw_grid(const GridWorld& env) {
    ClearBackground(RAYWHITE);

    float cell_size_x = SCREEN_WIDTH / GRID_SIZE;
    float cell_size_y = SCREEN_HEIGHT / GRID_SIZE;

    // Draw grid lines
    for (int i = 0; i <= GRID_SIZE; ++i) {
        DrawLine(i * cell_size_x, 0, i * cell_size_x, SCREEN_HEIGHT, LIGHTGRAY);
        DrawLine(0, i * cell_size_y, SCREEN_WIDTH, i * cell_size_y, LIGHTGRAY);
    }

    // Draw goal state (green circle)
    Vector2 goal_pos = env.get_goal_position();
    DrawCircleV(goal_pos, AGENT_RADIUS * 1.5f, GREEN);

    // Draw agent (blue circle)
    Vector2 agent_pos = env.get_agent_position();
    DrawCircleV(agent_pos, AGENT_RADIUS, BLUE);
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GridWorld Q-learning with Discrete Physics");
    SetTargetFPS(60);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    auto q_table = torch::zeros({GRID_SIZE * GRID_SIZE, NUM_ACTIONS});
    GridWorld env;

    for (int episode = 0; episode < NUM_EPISODES; ++episode) {
        env.reset();
        float total_reward = 0;

        while (!env.is_terminal()) {
            int action;
            if (dis(gen) < EPSILON_) {
                action = std::uniform_int_distribution<>(0, NUM_ACTIONS - 1)(gen);
            } else {
                action = q_table[episode % (GRID_SIZE * GRID_SIZE)].argmax().item<int>();
            }

            env.step(action);   // Apply action to environment
            env.update();       // Update physics-inspired movement

            BeginDrawing();
            draw_grid(env);     // Render environment
            EndDrawing();

            WaitTime(0.05);     // Slow down for visualization
        }

        std::cout << "Episode --> " << episode << ", Total Reward: " << total_reward << std::endl;
    }

    CloseWindow();
    return 0;
}

