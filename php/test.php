<?php

/**
 * Класс базы данных.
 * PHPDoc (ОСТАВИТЬ).
 */
class Database {
    // Хост подключения (УДАЛИТЬ)
    private $host; // Экранируем $ для bash

    /*
     * Конструктор.
     * Обычный блок (УДАЛИТЬ).
     */
    public function __construct() {
        $this->host = 'localhost'; // Default (УДАЛИТЬ)
    }

    /**
     * Подключиться к БД.
     * @return bool
     * (ОСТАВИТЬ)
     */
    public function connect() {
        return true;
    }
}
