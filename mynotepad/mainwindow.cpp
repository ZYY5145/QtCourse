#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include "qfiledialog.h"
#include "QMessageBox"
#include "QTextStream"
#include "QColorDialog"
#include "QFontDialog"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChanged=false;
    on_actionNew_triggered();

    ui->statusbar;
    statusLabel.setMaximumWidth(180);
    statusLabel.setText("length:"+QString::number(0)+" lines:"+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);


    statusCursorLabel.setMaximumWidth(180);
    statusCursorLabel.setText("Ln: "+QString::number(0)+" Col:"+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);


    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("朱育毅"));
    ui->statusbar->addPermanentWidget(author);


    ui->actionCopy->setEnabled(false);
    ui->actionredo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->action_L->setChecked(false);

    QPlainTextEdit::LineWrapMode mode=ui->textedit->lineWrapMode();

    if(mode==QTextEdit::NoWrap){
        ui->textedit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionWrap->setChecked(false);
    }
    else{
        ui->textedit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionWrap->setChecked(true);
    }

    ui->actionToolbar->setChecked(true);
    ui->actionStatusbar->setChecked(true);

    connect(ui->action_L,SIGNAL(triggered(bool)),ui->textedit,SLOT(hideLineNumberArea(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dlg(this, ui->textedit);
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg(this,ui->textedit);
    dlg.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed()){
        return ;
    }
    ui->textedit->clear();
    this->setWindowTitle(tr("新建"));

    textChanged = false;
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed()){
        return ;
    }

    filePath="";
    QString filename = QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files (*.txt);;All(*.*)"));
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开文件失败");
        return ;
    }

    filePath=filename;
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textedit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
}


void MainWindow::on_actionSave_triggered()
{
    if(filePath ==""){
        QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files (*.txt)"));
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this,"..","打开保存文件失败");
            return ;
        }
        file.close();
        filePath=filename;
    }

    QFile file(filePath);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开保存文件失败");
        return ;
    }

    QTextStream out(&file);
    QString text =ui->textedit->toPlainText();
    out << text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged =false;
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files (*.txt)"));
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开保存文件失败");
        return ;
    }
    filePath=filename;
    QTextStream out(&file);
    QString text =ui->textedit->toPlainText();
    out << text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}


void MainWindow::on_textedit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*"+this->windowTitle());
        textChanged=true;
    }

    statusLabel.setText("length:"+QString::number(ui->textedit->toPlainText().length())+" lines:"
                        +QString::number(ui->textedit->document()->lineCount()));
}

bool MainWindow::userEditConfirmed()
{
    if(textChanged){
        QString path =(filePath != "")?filePath:"无标题.txt";
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更新保存到\n")+"\""+path+"\"?");
        msg.setStandardButtons(QMessageBox::Yes |QMessageBox::No |QMessageBox::Cancel);
        int r=msg.exec();
        switch (r) {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged=false;
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
    return true;
}




void MainWindow::on_actionUndo_triggered()
{
    ui->textedit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textedit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textedit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textedit->paste();
}


void MainWindow::on_actionredo_triggered()
{
    ui->textedit->redo();
}


void MainWindow::on_textedit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_textedit_redoAvailable(bool b)
{
    ui->actionredo->setEnabled(b);
}


void MainWindow::on_textedit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color =QColorDialog::getColor(Qt::black, this,"选择颜色");
    if(color.isValid()){
        ui->textedit->setStyleSheet(QString("QPlainTextEdit {color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionBlackFontGroundColor_triggered()
{
    QColor color =QColorDialog::getColor(Qt::black, this,"选择颜色");
    if(color.isValid()){
        ui->textedit->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionBlackGroudColor_triggered()
{
    QColor color =QColorDialog::getColor(Qt::black, this,"选择颜色");
    if(color.isValid()){
        ui->textedit->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode=ui->textedit->lineWrapMode();

    if(mode==QTextEdit::NoWrap){
        ui->textedit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionWrap->setChecked(true);
    }
    else{
        ui->textedit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionWrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok=false;
    QFont font=QFontDialog::getFont(&ok,this);
    if(ok){
        ui->textedit->setFont(font);
    }
}


void MainWindow::on_actionToolbar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolbar->setChecked(!visible);
}


void MainWindow::on_actionStatusbar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusbar->setChecked(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->textedit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if(userEditConfirmed()){
        exit(0);
    }
}


void MainWindow::on_textedit_cursorPositionChanged()
{
    int col = 0;
    int ln = 0;
    int flg=-1;
    int pos=ui->textedit->textCursor().position();
    QString text = ui->textedit->toPlainText();

    for (int i = 0; i < pos; ++i) {
        if(text[i]=="\n"){
            ln++;
            flg=i;
        }
    }

    flg++;
    col=pos-flg;
    statusCursorLabel.setText("Ln:"+QString::number(ln+1)+"col:"+QString::number(col+1));
}

void MainWindow::on_action_L_triggered(bool checked)
{
    ui->textedit->hideLineNumberArea(!checked);
}

