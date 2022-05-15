#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{

}

Database::~Database()
{

}

/* Методы для подключения к базе данных
 * */
void Database::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile(DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

/* Методы восстановления базы данных
 * */
bool Database::restoreDataBase()
{
    // Если база данных открылась ...
    if(this->openDataBase()){
        // Производим восстановление базы данных
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

/* Метод для открытия базы данных
 * */
bool Database::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void Database::closeDataBase()
{
    db.close();
}

/* Метод для создания таблицы в базе данных
 * */
bool Database::createTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                            TABLE_NUM      " TEXT            NOT NULL,"
                            TABLE_ANSWER     " TEXT            NOT NULL,"
                            TABLE_PIC      " BLOB            NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/* Метод для вставки записи в базу данных
 * */
bool Database::insertIntoTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " TABLE " ( " TABLE_NUM " , " TABLE_ANSWER ", "
                                             TABLE_PIC " ) "
                  "VALUES (:numtask, :answer, :contenttask)");
    query.bindValue(":numtask",      data[0].toString());
    query.bindValue(":answer",        data[1].toString());
    query.bindValue(":contenttask",         data[2].toByteArray());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/* Второй метод для вставки записи в базу данных
 * */
bool Database::insertIntoTable(const QString &name, const QByteArray &pic)
{
    QVariantList data;
    data.append(name);
    data.append(pic);

    if(insertIntoTable(data))
        return true;
    else
        return false;
}
