#include "replacedialog.h"
#include "ui_replacedialog.h"
#include "QMessageBox"

ReplaceDialog::ReplaceDialog(QWidget *parent,QPlainTextEdit *textedit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    ptextedit=textedit;

    ui->rbdown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_pushButton_clicked()
{
    QString target=ui->searchtext->text();

    if(target==""||ptextedit==nullptr)
        return ;
    QString text = ptextedit->toPlainText();
    QTextCursor c = ptextedit->textCursor();
    int index=-1;

    if(ui->rbdown->isChecked()){
        index = text.indexOf(target, c.position(),
                             ui->cbCaseSensetive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index);
            c.setPosition(index +target.length(), QTextCursor::KeepAnchor);
            ptextedit->setTextCursor(c);
        }

    }
    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("记事本");
        msg.setText(QString("找不到")+target);
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}


void ReplaceDialog::on_btReplace_clicked()
{
    QString target = ui->searchtext->text();
    QString to = ui->targetext->text();
    if((ptextedit!=nullptr)&&(target!="")&&(to!="")){
        QString seltext=ptextedit->textCursor().selectedText();
        if(seltext==target ){
            ptextedit->insertPlainText(to);
        }
        on_pushButton_clicked();
    }
}


void ReplaceDialog::on_btReplaceAll_clicked()
{
    QString target = ui->searchtext->text();
    QString to = ui->targetext->text();
    if((ptextedit!=nullptr)&&(target!="")&&(to!="")){
        QString text=ptextedit->toPlainText();
        text.replace(target, to,
                     ui->cbCaseSensetive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        ptextedit->clear();
        ptextedit->insertPlainText(text);
    }
}


void ReplaceDialog::on_btCancel_clicked()
{
    accept();
}

