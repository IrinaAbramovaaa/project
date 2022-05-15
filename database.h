#ifndef DATABASE_H
#define DATABASE_H

#include "./include.h"
#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>

#define DATABASE_NAME       "./../../db/DBName.db"

#define TABLE                   "MathTasks"       // Название таблицы
#define TABLE_NUM                "numtask"                // Первая колонка
#define TABLE_PIC              "contenttask"              // Вторая колонка
#define TABLE_ANSWER               "answer"               // Третья колонка

class DatabaseData;

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);
    ~Database();
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBase();

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase db;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    bool createTable();         // Создание таблицы в базе данных

public slots:
    bool insertIntoTable(const QVariantList &data);      // Добавление записей в таблицу
    bool insertIntoTable(const QString &name, const QByteArray &pic);
};

#endif // DATABASE_H
