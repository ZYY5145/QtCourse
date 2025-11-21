#include "searchdialog.h"
#include "ui_searchdialog.h"
#include "QMessageBox"

SearchDialog::SearchDialog(QWidget *parent,QPlainTextEdit *textedit)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    ptextedit=textedit;
    ui->rbup->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btFindNext_clicked()
{
    QString target=ui->searchtext->text();
    if(target==""||ptextedit==nullptr)
        return ;

    QString text = ptextedit->toPlainText();
    QTextCursor c = ptextedit->textCursor();
    int index=-1;

    if(ui->rbdown->isChecked()){
        index = text.indexOf(target, c.position(),
                             ui->checkBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
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


void SearchDialog::on_pushButton_2_clicked()
{
    accept();
}

