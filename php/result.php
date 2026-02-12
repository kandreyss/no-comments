<?php

/**
 * Класс базы данных.
 * PHPDoc (ОСТАВИТЬ).
 */
class Database {

    private $host;

    public function __construct() {
        $this->host = 'localhost';
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
