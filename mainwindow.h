#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QHeaderView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 槽函数：处理按钮点击
    void addBook();
    void addReader();
    void borrowBook();
    void returnBook();
    void refreshRecords(); // 刷新借阅记录

private:
    // 数据库初始化
    void initDatabase();
    // 界面初始化
    void initUI();

    // 核心数据模型 (Model/View 核心)
    QSqlTableModel *bookModel;
    QSqlTableModel *readerModel;
    QSqlQueryModel *recordModel; // 用于只读展示借阅记录

    // 界面控件
    QTabWidget *tabWidget;

    // 图书管理页控件
    QTableView *bookView;
    QLineEdit *inputBookTitle;
    QLineEdit *inputBookAuthor;

    // 读者管理页控件
    QTableView *readerView;
    QLineEdit *inputReaderName;

    // 借阅/归还页控件
    QTableView *recordView;
    QLineEdit *inputBorrowBookID;
    QLineEdit *inputBorrowReaderID;
    QLineEdit *inputReturnBookID;
};

#endif // MAINWINDOW_H
