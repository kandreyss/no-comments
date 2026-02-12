import Foundation

/// Вычисляет факториал числа.
/// - Parameter n: Число.
/// - Returns: Факториал.
/// ЭТОТ БЛОК (///) ДОЛЖЕН ОСТАТЬСЯ.
func factorial(n: Int) -> Int {
    // Базовый случай (УДАЛИТЬ)
    if n <= 1 { return 1 }
    return n * factorial(n: n - 1)
}

/*
  Многострочный комментарий.
  (УДАЛИТЬ)
*/

/**
  Документация в стиле блока.
  (ОСТАВИТЬ)
 */
struct Vector {
    var x: Double // Координата X (УДАЛИТЬ)
    var y: Double
}
