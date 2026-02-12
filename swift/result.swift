import Foundation

/// Вычисляет факториал числа.
/// - Parameter n: Число.
/// - Returns: Факториал.
/// ЭТОТ БЛОК (///) ДОЛЖЕН ОСТАТЬСЯ.
func factorial(n: Int) -> Int {

    if n <= 1 { return 1 }
    return n * factorial(n: n - 1)
}

/**
  Документация в стиле блока.
  (ОСТАВИТЬ)
 */
struct Vector {
    var x: Double
    var y: Double
}
