#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QBuffer>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new Database();
    db->connectToDataBase();
    this->setupModel(TABLE,
                         QStringList() << "id"
                                       << "Имя изображения"
                                       << "изображение"
                         );

    this->createUI();
    /*db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./../../db/DBName.db");

    if (db.open()) {
        ui->statusbar->showMessage("SUCCESS connect to database" + db.databaseName());
    } else {
       ui->statusbar->showMessage("ERROR to connect DataBase" + db.lastError().databaseText());
    }*/
}
    void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
    {
        /* Производим инициализацию модели представления данных
         * с установкой имени таблицы в базе данных, по которому
         * будет производится обращение в таблице
         * */
        model = new QSqlTableModel(this);
        model->setTable(tableName);

        /* Устанавливаем названия колонок в таблице с сортировкой данных
         * */
        for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
            model->setHeaderData(i,Qt::Horizontal,headers[j]);
        }
    }

    void MainWindow::createUI() {
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(0, true);    // Hide the column id Records
        ui->tableView->setColumnHidden(1, true);
        ui->tableView->setColumnHidden(2, true);    // Hide the column with image
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // editing is not allowed
        ui->tableView->horizontalHeader()->setStretchLastSection(true);     // Stretch the last column of around tableView

        /* Connect the signal to change the selection of the current row in the table
        * to the slot to set the image picLabel
        * */
        connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                    this, SLOT(slotCurrentPic(QModelIndex)));

        model->select();
    }

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("%");
        db.setUserName("andrey");
        db.setPassword("a1sdf34!A");
        db.setDatabaseName("task");

        if (db.open()) {
            QMessageBox::information(this, "Connection", "Database Connection Successfull");
        } else {
            QMessageBox::information(this, "Not Connection", "Database Connection is false");
        }
}*/


void MainWindow::on_pushButtonAnswer_clicked()
{
    QString inp_answer;
    ui->lineEdit_answer->setValidator( new QDoubleValidator);
    inp_answer = ui->lineEdit_answer->text();
    QString numtask_now;
    numtask_now = ui->label_2->text();
    if (!db) {
        qDebug() << "Failing open database";
        return;
    }

    QSqlQuery qry;
    if (qry.exec("select * from MathTasks where numtask='"+numtask_now+"' and answer='"+inp_answer+"'")) {
        size_t count = 0;

        while (qry.next()) {
            ++count;
        }

        if (count == 1) {
            QMessageBox::information(this, "Answer", "Answer is good!");
            ui->lineEdit_answer->clear();
        } else {
            QMessageBox::information(this, "Answer", "Answer is bad!");
            ui->lineEdit_answer->clear();
        }
    }
}

void MainWindow::slotCurrentPic(QModelIndex index)
{
    QPixmap outPixmap = QPixmap(); // Создаём QPixmap, который будет помещаться в picLabel
        /* Забираем данные об изображении из таблицы в качестве QByteArray
         * и помещаем их в QPixmap
         * */
    outPixmap.loadFromData(model->data(model->index(index.row(), 2)).toByteArray());
        // Устанавливаем изображение в picLabel
    ui->pic_label->setPixmap(outPixmap.scaled(400,300));
}
