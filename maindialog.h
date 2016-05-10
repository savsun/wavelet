#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include "all.hpp"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

    QString img_fname, a_fname, dh_fname, dv_fname, dd_fname;

    void update();

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void on_img_button_clicked();

    void on_a_button_clicked();

    void on_dh_button_clicked();

    void on_dv_button_clicked();

    void on_dd_button_clicked();

    void on_down_button_clicked();

    void on_up_button_clicked();

    void on_jpeg_button_clicked();

private:
    Ui::MainDialog *ui;
};

#endif // MAINDIALOG_H
