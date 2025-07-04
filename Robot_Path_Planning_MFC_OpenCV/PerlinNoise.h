#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

// 定义二维向量结构体
struct Vector2 {
    float x, y;
    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
};

// 定义梯度向量数组
const Vector2 gradients[] = {
    {1, 1}, {-1, 1}, {1, -1}, {-1, -1},
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};

// 随机数生成器
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(0, 7);

// 获取随机梯度向量
Vector2 getGradient(int x, int y) {
    int index = (x + y * 57) % 8;
    return gradients[index];
}

// 点积计算
float dot(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;
}

// 平滑函数
float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// 线性插值
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// 柏林噪声计算
float perlinNoise(float x, float y) {
    int xi = (int)std::floor(x) & 255;
    int yi = (int)std::floor(y) & 255;

    float xf = x - std::floor(x);
    float yf = y - std::floor(y);

    Vector2 topRight(xf - 1.0, yf - 1.0);
    Vector2 topLeft(xf, yf - 1.0);
    Vector2 bottomRight(xf - 1.0, yf);
    Vector2 bottomLeft(xf, yf);

    float dotTopRight = dot(getGradient(xi + 1, yi + 1), topRight);
    float dotTopLeft = dot(getGradient(xi, yi + 1), topLeft);
    float dotBottomRight = dot(getGradient(xi + 1, yi), bottomRight);
    float dotBottomLeft = dot(getGradient(xi, yi), bottomLeft);

    float u = fade(xf);
    float v = fade(yf);

    return lerp(
        lerp(dotBottomLeft, dotBottomRight, u),
        lerp(dotTopLeft, dotTopRight, u),
        v
    );
}
