#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(900, 600);
    this->setWindowTitle("图书与借阅管理系统");

    initDatabase();
    initUI();
}

MainWindow::~MainWindow()
{
    // Qt的对象树机制会自动清理GUI子对象，数据库连接随进程结束自动关闭
}

void MainWindow::initDatabase()
{
    // 连接 SQLite 数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    if (!db.open()) {
        QMessageBox::critical(this, "错误", "无法连接数据库！\n" + db.lastError().text());
        return;
    }

    QSqlQuery query;

    // 1. 创建图书表 (status: 1=可借, 0=已借出)
    query.exec("CREATE TABLE IF NOT EXISTS books ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "title TEXT NOT NULL, "
               "author TEXT, "
               "status INTEGER DEFAULT 1)");

    // 2. 创建读者表
    query.exec("CREATE TABLE IF NOT EXISTS readers ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT NOT NULL)");

    // 3. 创建借阅记录表
    query.exec("CREATE TABLE IF NOT EXISTS records ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "book_id INTEGER, "
               "reader_id INTEGER, "
               "borrow_date TEXT, "
               "return_date TEXT, "
               "is_active INTEGER DEFAULT 1)"); // is_active=1 表示未归还
}

void MainWindow::initUI()
{
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // ================== Tab 1: 图书管理 (Model/View) ==================
    QWidget *bookTab = new QWidget;
    QVBoxLayout *bookLayout = new QVBoxLayout(bookTab);

    // 设置 Model
    bookModel = new QSqlTableModel(this);
    bookModel->setTable("books");
    bookModel->setEditStrategy(QSqlTableModel::OnFieldChange); // 修改直接保存
    bookModel->select();
    bookModel->setHeaderData(0, Qt::Horizontal, "图书ID");
    bookModel->setHeaderData(1, Qt::Horizontal, "书名");
    bookModel->setHeaderData(2, Qt::Horizontal, "作者");
    bookModel->setHeaderData(3, Qt::Horizontal, "状态(1=在馆)");

    // 设置 View
    bookView = new QTableView;
    bookView->setModel(bookModel);
    bookView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 输入区
    QHBoxLayout *bookInputLayout = new QHBoxLayout;
    inputBookTitle = new QLineEdit;
    inputBookTitle->setPlaceholderText("输入书名");
    inputBookAuthor = new QLineEdit;
    inputBookAuthor->setPlaceholderText("输入作者");
    QPushButton *btnAddBook = new QPushButton("添加图书");
    connect(btnAddBook, &QPushButton::clicked, this, &MainWindow::addBook);

    bookInputLayout->addWidget(new QLabel("新书入库:"));
    bookInputLayout->addWidget(inputBookTitle);
    bookInputLayout->addWidget(inputBookAuthor);
    bookInputLayout->addWidget(btnAddBook);

    bookLayout->addLayout(bookInputLayout);
    bookLayout->addWidget(bookView);
    tabWidget->addTab(bookTab, "图书信息管理");

    // ================== Tab 2: 读者管理 (Model/View) ==================
    QWidget *readerTab = new QWidget;
    QVBoxLayout *readerLayout = new QVBoxLayout(readerTab);

    readerModel = new QSqlTableModel(this);
    readerModel->setTable("readers");
    readerModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    readerModel->select();
    readerModel->setHeaderData(0, Qt::Horizontal, "读者ID");
    readerModel->setHeaderData(1, Qt::Horizontal, "姓名");

    readerView = new QTableView;
    readerView->setModel(readerModel);
    readerView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *readerInputLayout = new QHBoxLayout;
    inputReaderName = new QLineEdit;
    inputReaderName->setPlaceholderText("输入读者姓名");
    QPushButton *btnAddReader = new QPushButton("添加读者");
    connect(btnAddReader, &QPushButton::clicked, this, &MainWindow::addReader);

    readerInputLayout->addWidget(new QLabel("注册读者:"));
    readerInputLayout->addWidget(inputReaderName);
    readerInputLayout->addWidget(btnAddReader);

    readerLayout->addLayout(readerInputLayout);
    readerLayout->addWidget(readerView);
    tabWidget->addTab(readerTab, "读者档案管理");

    // ================== Tab 3: 借阅与归还 (业务逻辑) ==================
    QWidget *borrowTab = new QWidget;
    QVBoxLayout *borrowLayout = new QVBoxLayout(borrowTab);

    // 操作区
    QHBoxLayout *actionLayout = new QHBoxLayout;

    // 借书部分
    inputBorrowBookID = new QLineEdit;
    inputBorrowBookID->setPlaceholderText("图书ID");
    inputBorrowReaderID = new QLineEdit;
    inputBorrowReaderID->setPlaceholderText("读者ID");
    QPushButton *btnBorrow = new QPushButton("办理借阅");
    connect(btnBorrow, &QPushButton::clicked, this, &MainWindow::borrowBook);

    // 还书部分
    inputReturnBookID = new QLineEdit;
    inputReturnBookID->setPlaceholderText("归还图书ID");
    QPushButton *btnReturn = new QPushButton("办理归还");
    connect(btnReturn, &QPushButton::clicked, this, &MainWindow::returnBook);

    actionLayout->addWidget(new QLabel("借阅:"));
    actionLayout->addWidget(inputBorrowBookID);
    actionLayout->addWidget(inputBorrowReaderID);
    actionLayout->addWidget(btnBorrow);
    actionLayout->addSpacing(20);
    actionLayout->addWidget(new QLabel("归还:"));
    actionLayout->addWidget(inputReturnBookID);
    actionLayout->addWidget(btnReturn);

    // 记录展示 (关联查询)
    recordModel = new QSqlQueryModel(this);
    recordView = new QTableView;
    recordView->setModel(recordModel);
    recordView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    borrowLayout->addLayout(actionLayout);
    borrowLayout->addWidget(new QLabel("当前借阅记录 (实时统计):"));
    borrowLayout->addWidget(recordView);

    tabWidget->addTab(borrowTab, "借阅流通台");

    refreshRecords(); // 初始加载记录
}

void MainWindow::addBook()
{
    QString title = inputBookTitle->text().trimmed();
    QString author = inputBookAuthor->text().trimmed();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "提示", "书名不能为空");
        return;
    }

    // 使用 SQL 插入语句（推荐方法，更稳定）
    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, status) VALUES (:title, :author, 1)");
    query.bindValue(":title", title);
    query.bindValue(":author", author);

    if (query.exec()) {
        bookModel->select(); // 刷新表格显示
        inputBookTitle->clear();
        inputBookAuthor->clear();
        QMessageBox::information(this, "成功", "图书入库成功！");
    } else {
        QMessageBox::critical(this, "失败", "入库失败: " + query.lastError().text());
    }
}

void MainWindow::addReader()
{
    QString name = inputReaderName->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "提示", "读者姓名不能为空");
        return;
    }

    // 使用 SQL 插入语句
    QSqlQuery query;
    query.prepare("INSERT INTO readers (name) VALUES (:name)");
    query.bindValue(":name", name);

    if (query.exec()) {
        readerModel->select(); // 刷新表格显示
        inputReaderName->clear();
        QMessageBox::information(this, "成功", "读者注册成功！");
    } else {
        QMessageBox::critical(this, "失败", "注册失败: " + query.lastError().text());
    }
}

void MainWindow::borrowBook()
{
    QString bookId = inputBorrowBookID->text().trimmed();
    QString readerId = inputBorrowReaderID->text().trimmed();

    if (bookId.isEmpty() || readerId.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入图书ID和读者ID");
        return;
    }

    QSqlQuery query;

    // 1. 检查图书是否存在且在馆
    query.prepare("SELECT status FROM books WHERE id = :id");
    query.bindValue(":id", bookId);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "错误", "图书ID不存在");
        return;
    }

    if (query.value(0).toInt() != 1) {
        QMessageBox::warning(this, "错误", "该书已被借出！");
        return;
    }

    // 2. 检查读者是否存在
    query.prepare("SELECT id FROM readers WHERE id = :id");
    query.bindValue(":id", readerId);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "错误", "读者ID不存在");
        return;
    }

    // 3. 事务处理：更新图书状态 + 插入借阅记录
    QSqlDatabase::database().transaction();

    query.prepare("UPDATE books SET status = 0 WHERE id = :id");
    query.bindValue(":id", bookId);
    bool ok1 = query.exec();

    query.prepare("INSERT INTO records (book_id, reader_id, borrow_date, is_active) "
                  "VALUES (:bid, :rid, :date, 1)");
    query.bindValue(":bid", bookId);
    query.bindValue(":rid", readerId);
    query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
    bool ok2 = query.exec();

    if (ok1 && ok2) {
        QSqlDatabase::database().commit();
        QMessageBox::information(this, "成功", "借阅成功！");
        bookModel->select(); // 刷新图书列表状态
        refreshRecords();    // 刷新记录列表
        inputBorrowBookID->clear();
        inputBorrowReaderID->clear();
    } else {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "失败", "借阅操作失败: " + query.lastError().text());
    }
}

void MainWindow::returnBook()
{
    QString bookId = inputReturnBookID->text().trimmed();

    if (bookId.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入图书ID");
        return;
    }

    QSqlQuery query;

    // 1. 检查该书是否在借出状态
    query.prepare("SELECT id FROM records WHERE book_id = :bid AND is_active = 1");
    query.bindValue(":bid", bookId);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "提示", "该书没有未归还的借阅记录");
        return;
    }

    // 2. 事务处理：更新图书状态 + 更新记录归还时间
    QSqlDatabase::database().transaction();

    query.prepare("UPDATE books SET status = 1 WHERE id = :id");
    query.bindValue(":id", bookId);
    bool ok1 = query.exec();

    query.prepare("UPDATE records SET return_date = :date, is_active = 0 "
                  "WHERE book_id = :bid AND is_active = 1");
    query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
    query.bindValue(":bid", bookId);
    bool ok2 = query.exec();

    if (ok1 && ok2) {
        QSqlDatabase::database().commit();
        QMessageBox::information(this, "成功", "归还成功！");
        bookModel->select();
        refreshRecords();
        inputReturnBookID->clear();
    } else {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "失败", "归还操作失败: " + query.lastError().text());
    }
}

void MainWindow::refreshRecords()
{
    // 使用 SQL 联表查询，让数据显示更直观 (显示书名和人名而不是ID)
    recordModel->setQuery("SELECT r.id, b.title, p.name, r.borrow_date, r.return_date, "
                          "CASE WHEN r.is_active=1 THEN '未还' ELSE '已还' END as status "
                          "FROM records r "
                          "LEFT JOIN books b ON r.book_id = b.id "
                          "LEFT JOIN readers p ON r.reader_id = p.id "
                          "ORDER BY r.id DESC");

    recordModel->setHeaderData(0, Qt::Horizontal, "记录ID");
    recordModel->setHeaderData(1, Qt::Horizontal, "图书名称");
    recordModel->setHeaderData(2, Qt::Horizontal, "读者姓名");
    recordModel->setHeaderData(3, Qt::Horizontal, "借阅日期");
    recordModel->setHeaderData(4, Qt::Horizontal, "归还日期");
    recordModel->setHeaderData(5, Qt::Horizontal, "状态");
}
