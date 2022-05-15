#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButtonAnswer_clicked();
    void slotCurrentPic(QModelIndex index);

private:
    Ui::MainWindow *ui;
    Database        *db;
    QSqlTableModel  *model;

    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
};
#endif // MAINWINDOW_H
