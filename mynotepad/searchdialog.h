#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include "QPlainTextEdit"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr,QPlainTextEdit *textedit = nullptr);
    ~SearchDialog();

private slots:
    void on_btFindNext_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SearchDialog *ui;

    QPlainTextEdit *ptextedit;
};

#endif // SEARCHDIALOG_H
