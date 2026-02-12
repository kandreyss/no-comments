/**
 * @file test.cpp
 * @brief Тест для C++.
 * ЭТОТ БЛОК ДОЛЖЕН ОСТАТЬСЯ.
 */

#include <iostream>

// Обычный комментарий (УДАЛИТЬ)
int globalVar = 0;

/// @brief Функция сложения (ОСТАВИТЬ, стиль ///)
int add(int a, int b) {
    return a + b; // Сложение (УДАЛИТЬ ХВОСТ)
}

/*
 * Старый код, который больше не нужен.
 * (УДАЛИТЬ ВЕСЬ БЛОК)
 */

//! @brief Функция вычитания (ОСТАВИТЬ, стиль //!)
int sub(int a, int b) {
    return a - b;
}

/*!
 * @class Calculator
 * @brief Класс калькулятора (ОСТАВИТЬ, стиль /*!)
 */
class Calculator {};
