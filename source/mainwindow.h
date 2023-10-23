#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QStandardItemModel;
class QThread;
class QTcpServer;
class QTcpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    //  自定义单元格Type的类型，在创建单元格的Item时使用
    enum    CellType{ctID=1000,ctName,ctScore,ctRank,ctIsParty}; //各单元格的类型
    //  各字段在表格中的列号
    enum    FieldColNum{colID=0,colName,colScore,colRank,colIsParty};

    bool m_orderType = true;
    QStandardItemModel *m_model;
    QTcpServer *m_server;
    QTcpSocket *m_clientSocket;



    void initUi();
    void initConnection();
    void initTreeView();
    void initTableData();
    void createItemsARow(int row, int id, QString name, int score, int rank, bool isParty);

private slots:
    void  do_printString();
    void  do_reversalTable();
    void  do_numChange();
    void  do_deleteRows();
    void  do_createAndWriteFile();
    void  do_threadFinished();
    void  do_threadIsWriting(int schedule);
    void  do_createDataBase();
    void  do_sortByNum2();
    void  do_clearSQL();
    void  do_deleteAndPrint();
    void  do_openNewWindow();

};
#endif // MAINWINDOW_H
