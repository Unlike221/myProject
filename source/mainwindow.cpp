#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QString>
#include <QStandardItemModel>
#include <QTimer>
#include <QRandomGenerator>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QThread>
#include "twritethread.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUi();
    initConnection();
    initTableData();
    initTreeView();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initUi()
{
    this->setWindowTitle("界面程序-贾永哲-2023/10/22");
    this->setCentralWidget(this->centralWidget());
    ui->plainTextEdit_toolBar->setReadOnly(true);


    //顶部菜单栏添加选项
    QMenu *file = new QMenu("文件");
    QMenu *view = new QMenu("视图");
    QMenu *setting = new QMenu("设置");
    QMenu *help = new QMenu("帮助");

    QAction *openNew = new QAction(QIcon("../images/newWindow.svg"), "打开新页面");
    view->addAction(openNew);

    ui->menubar->addMenu(file);
    ui->menubar->addMenu(view);
    ui->menubar->addMenu(setting);
    ui->menubar->addMenu(help);
    connect(openNew, &QAction::triggered, this, &MainWindow::do_openNewWindow);


    //底部状态栏增加三个显示内容区域
    QLabel *label_1 = new QLabel(this);
    label_1->setMinimumWidth(200);
    label_1->setAlignment(Qt::AlignLeft);
    label_1->setText("欢迎使用本程序");
    ui->statusbar->addWidget(label_1);

    QLabel *label_2 = new QLabel(this);
    label_2->setMinimumWidth(200);
    label_2->setAlignment(Qt::AlignRight);
    label_2->setText("祝您每天好心情");
    ui->statusbar->addPermanentWidget(label_2);

    QLabel *label_3 = new QLabel(this);
    label_3->setMinimumWidth(200);
    label_3->setAlignment(Qt::AlignRight);
    label_3->setText("官网: uDDS.cn");
    ui->statusbar->addPermanentWidget(label_3);


}

//程序内的信号与槽连接
void MainWindow::initConnection()
{
    connect(ui->action_Output1, &QAction::triggered, this, &MainWindow::do_printString);
    connect(ui->action_Output2, &QAction::triggered, this, &MainWindow::do_printString);
    connect(ui->action_Output3, &QAction::triggered, this, &MainWindow::do_printString);
    connect(ui->action_Output4, &QAction::triggered, this, &MainWindow::do_printString);
    connect(ui->action_Output5, &QAction::triggered, this, &MainWindow::do_printString);
    connect(ui->btn_Reverse, &QPushButton::clicked, this, &MainWindow::do_reversalTable);
    connect(ui->btn_ChangeNum, &QPushButton::clicked, this, &MainWindow::do_numChange);
    connect(ui->btn_DeleteRow, &QPushButton::clicked, this, &MainWindow::do_deleteRows);
    connect(ui->btn_WriteFile, &QPushButton::clicked, this, &MainWindow::do_createAndWriteFile);
    connect(ui->btn_CreateSQL, &QPushButton::clicked, this, &MainWindow::do_createDataBase);
    connect(ui->btn_SortSQL, &QPushButton::clicked, this, &MainWindow::do_sortByNum2);
    connect(ui->btn_ClearSQL, &QPushButton::clicked, this, &MainWindow::do_clearSQL);
    connect(ui->btn_PrintSQL, &QPushButton::clicked, this, &MainWindow::do_deleteAndPrint);
}


//*************************************
//******功能1: 工具栏自定义输出打印*******
//*************************************

//工具栏的自定义输出打印
void MainWindow::do_printString()
{
    //多个信号连接一个槽函数
    QAction *action = (QAction*)sender();       //获取发送信号的QAction
    QString actName = action->objectName();
    int num = actName.last(1).toInt();          //判断是哪个Action并打印相应的字符串
    qDebug() << num;
    switch(num)
    {
    case 1:ui->plainTextEdit_toolBar->appendPlainText("hello world"); break;
    case 2:ui->plainTextEdit_toolBar->appendPlainText("你好，磐优"); break;
    case 3:ui->plainTextEdit_toolBar->appendPlainText("官网: uDDS.cn"); break;
        case 4:ui->plainTextEdit_toolBar->appendPlainText("很荣幸参加本次面试"); break;
    case 5:ui->plainTextEdit_toolBar->appendPlainText("I Love C++/QT"); break;

    }
}




//*************************************
//******功能2: 可以倒序显示的表格*********
//*************************************


void MainWindow::createItemsARow(int row, int id, QString name, int score, int rank, bool isParty)
{//添加一行数据
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置id
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(id), MainWindow::ctID);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, MainWindow::colID, item);
    //设置Name
    item = new QTableWidgetItem(name, MainWindow::ctName);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, MainWindow::colName, item);
    //设置score
    item = new QTableWidgetItem(QString::number(score), MainWindow::ctScore);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, MainWindow::colScore, item);
    //设置rank
    item = new QTableWidgetItem(QString::number(rank), MainWindow::ctRank);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, MainWindow::colRank, item);
    //设置isParty
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |Qt::ItemIsEnabled;
    item = new QTableWidgetItem("党员", MainWindow::ctIsParty);
    item->setFlags(flags);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if (isParty)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    ui->tableWidget->setItem(row, MainWindow::colIsParty, item);

}

void MainWindow::initTableData()
{//初始化表格数据
    ui->tableWidget->clearContents();
    //初始化表格行列数
    ui->tableWidget->setColumnCount(5);//设置列数
    ui->tableWidget->setRowCount(8);//设置行数

    //设置表头
    QStringList HHead = {};
    HHead << "学号" << "姓名" << "总分" << "排名" << "是否党员";
    ui->tableWidget->setHorizontalHeaderLabels(HHead);

    //设置表格样式
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);                            //设置表格不可编辑
    ui->tableWidget->setAlternatingRowColors(true);

    //设置表头样式
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#679cba;}");   //设置表头颜色
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    //添加数据
    createItemsARow(0, 123, "xiaohong",     456, 23, false);
    createItemsARow(1, 124, "xiaoqiang",    545, 20, true);
    createItemsARow(2, 125, "zhangsan",     768, 1,  true);
    createItemsARow(3, 126, "lisi",         394, 45, false);
    createItemsARow(4, 127, "luoxiang",     289, 56, true);
    createItemsARow(5, 128, "fanbing",      766, 2,  false);
    createItemsARow(6, 129, "zhangyi",      455, 22, true);
    createItemsARow(7, 130, "wujing",       203, 78, true);

    ui->tableWidget->show();
    m_orderType = false;
}

void MainWindow::do_reversalTable()
{//表格倒序显示
    m_orderType = !m_orderType;
    if(m_orderType)
        ui->tableWidget->sortByColumn(0, Qt::DescendingOrder);
    else
        ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
}



//*************************************
//******功能3: model/view树形图*********
//*************************************

void MainWindow::initTreeView()
{//初始化TreeView
    m_model = new QStandardItemModel(ui->treeView);
    m_model->setHorizontalHeaderLabels(QStringList()<<"选手ID"<<"得分1"<<"得分2" << "得分3" << "得分4");
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->setStyleSheet("QHeaderView::section {background-color:#679cba;}"
                                "QTreeView::item{height: 25px;width:20px}");

    //一级节点
    QList<QStandardItem*> items1;
    QStandardItem *item1 = new QStandardItem("中国队");
    items1 << item1;
    m_model->appendRow(items1);
    //二级节点

    for(int i=0; i<8; ++i){
        QList<QStandardItem*> items2;
        for(int j=0; j<5; ++j)
        {
            QStandardItem *aitem = new QStandardItem(QString::number(QRandomGenerator::global()->bounded(0,51)));
            aitem->setTextAlignment(Qt::AlignCenter);
            items2 << aitem;
        }
        item1->appendRow(items2);
    }

    ui->treeView->setModel(m_model);
    ui->treeView->show();
}

void MainWindow::do_numChange()
{//将TreeView中小于25的数据变为0
    //遍历所有item，由于只有一个一级节点，直接遍历该一级节点下的所有二级节点
    QStandardItem *rootItem = m_model->item(0);
    for(int i=0; i < rootItem->rowCount(); ++i)
    {
        for(int j=0; j<rootItem->columnCount(); ++j)
        {
            QStandardItem *childItem = rootItem->child(i,j);
            if(childItem->text().toInt() <= 25)
                childItem->setText("0");
        }
    }

}

void MainWindow::do_deleteRows()
{//TreeView过滤单数行号
    QStandardItem *rootItem = m_model->item(0);
    for(int i= rootItem->rowCount()-1; i >=0; --i)
    {
        if((i+1)%2) //认为行号为1~8而不是0~7
        {
            rootItem->removeRow(i);
        }
    }
}



//****************************************
//*功能4: 创建文件hello.txt并以多线程方式写入*
//****************************************

void MainWindow::do_createAndWriteFile()
{//创建并写入文件
    auto curDir = QDir::currentPath();
    auto path = QFileDialog::getExistingDirectory(this, "选择目标文件夹", curDir);
    qDebug() << path;

    TWriteThread *athread = new TWriteThread();
    athread->m_fileName = path + "/hello.txt";
    connect(athread, &QThread::finished, this, &MainWindow::do_threadFinished);
    connect(athread, &TWriteThread::writing, this, &MainWindow::do_threadIsWriting);

    athread->start();
}

void MainWindow::do_threadFinished()
{//写文件线程结束后在状态栏进行提示
    ui->statusbar->showMessage("写文件线程已结束");
}

void MainWindow::do_threadIsWriting(int schedule)
{//线程正在写入时在状态栏显示进度
    ui->statusbar->showMessage(QString("线程正在写入文件，进度: %1%").arg(schedule));
}



//*************************************
//********功能5: 数据库操作**************
//*************************************

void MainWindow::do_createDataBase()
{//创建数据表1并插入数据
    //1.创建数据库
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("myDataBase.db");
    }
    if(!db.open())
    {
        QMessageBox::critical(this, "警告", "数据库创建失败");
        return;
    }

    //2.若数据库存在则删除旧数据
    QSqlQuery query(db);
    query.exec("drop table if exists '数据库表1'");
    if(!query.isActive())
    {
        QMessageBox::critical(this, "警告", "删除原数据失败");
        return;
    }

    //3.创建数据库表1
    query.exec("create table if not exists '数据库表1' "
               " ('序号'   integer primary key NOT NULL, "
               " '随机数1' integer not null, "
               " '随机数2' integer not null, "
               " '随机数3' integer not null);");

    if(!query.isActive())
    {
        QMessageBox::critical(this, "警告", "数据表创建失败");
        return;
    }

    //4.插入第一条数据
    query.prepare("insert into '数据库表1' values(:id, :num1, :num2, :num3);");
    query.bindValue(":id", 0);
    query.bindValue(":num1", QRandomGenerator::global()->bounded(0,3000));
    query.bindValue(":num2", QRandomGenerator::global()->bounded(0,3000));
    query.bindValue(":num3", QRandomGenerator::global()->bounded(0,3000));
    query.exec();

    //5.插入999条数据
    for(int i=0; i<999; ++i)
    {
        query.prepare("insert into '数据库表1' values(null, :num1, :num2, :num3);");
        query.bindValue(":num1", QRandomGenerator::global()->bounded(0,3000));
        query.bindValue(":num2", QRandomGenerator::global()->bounded(0,3000));
        query.bindValue(":num3", QRandomGenerator::global()->bounded(0,3000));
        query.exec();
    }
    db.close();
    QMessageBox::information(this, "提示", "数据库创建成功");
}

void MainWindow::do_sortByNum2()
{//根据随机数2进行排序
    //1.判断数据库是否已创建
    QString dbName = QDir::currentPath() + "/myDataBase.db";
    QFileInfo fileinfo(dbName);
    if(!fileinfo.exists())
    {
        QMessageBox::warning(this, "警告", "数据库未创建");
        return;
    }


    //2.根据随机数2对数据进行排序
    ui->plainTextEdit_SQL->clear();
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("myDataBase.db");
    }

    if(db.open())
    {
        QSqlQuery query(db);
        query.exec("select * from '数据库表1' order by \"随机数2\" desc;");
        if(query.isActive())
        {
            ui->plainTextEdit_SQL->appendPlainText("序号 随机数1 随机数2 随机数3");
            while (query.next()) {
                QString str = QString("%1   %2   %3   %4").arg(query.value(0).toInt(), 4, 10, QLatin1Char('0'))
                                                          .arg(query.value(1).toInt(), 4, 10, QLatin1Char('0'))
                                                          .arg(query.value(2).toInt(), 4, 10, QLatin1Char('0'))
                                                          .arg(query.value(3).toInt(), 4, 10, QLatin1Char('0'));
                ui->plainTextEdit_SQL->appendPlainText(str);
            }
            ui->plainTextEdit_SQL->appendPlainText("以上为依据随机数2降序排序后的结果....");
        }
    }

    db.close();
}

void MainWindow::do_deleteAndPrint()
{//打印随机数3为奇数的行且删除随机数3为偶数的行
    //1.判断数据库是否已创建
    QString dbName = QDir::currentPath() + "/myDataBase.db";
    QFileInfo fileinfo(dbName);
    if(!fileinfo.exists())
    {
        QMessageBox::warning(this, "警告", "数据库未创建");
        return;
    }


    //2.打印随机数3为奇数的行且删除随机数3为偶数的行
    ui->plainTextEdit_SQL->clear();
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("myDataBase.db");
    }

    if(db.open())
    {
        QSqlQuery query(db);
        query.exec("delete from '数据库表1' where \"随机数3\"%2=0;");
        query.exec("select * from '数据库表1' order by \"序号\" asc;");
        if(query.isActive())
        {
            ui->plainTextEdit_SQL->appendPlainText("序号 随机数1 随机数2 随机数3");
            while (query.next()) {
                QString str = QString("%1   %2   %3   %4").arg(query.value(0).toInt(), 4, 10, QLatin1Char('0'))
                                  .arg(query.value(1).toInt(), 4, 10, QLatin1Char('0'))
                                  .arg(query.value(2).toInt(), 4, 10, QLatin1Char('0'))
                                  .arg(query.value(3).toInt(), 4, 10, QLatin1Char('0'));
                ui->plainTextEdit_SQL->appendPlainText(str);
            }
            ui->plainTextEdit_SQL->appendPlainText("以上为删除后的结果....");
        }
    }

    db.close();
}

void MainWindow::do_clearSQL()
{//清空查询结果
    ui->plainTextEdit_SQL->clear();
}



//*************************************
//********功能6: 打开新的空白界面*********
//*************************************
void MainWindow::do_openNewWindow()
{
    MainWindow *one = new MainWindow;
    one->show();
}
