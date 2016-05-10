#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
}

MainDialog::~MainDialog()
{
    delete ui;
}

#define OPEN_IMAGE(func, var, control, msg)\
void MainDialog::func()\
{\
    var = QFileDialog::getSaveFileName(this, QString(msg), QString("."), QString("*.bmp;;*.png;;*.jpg"));\
\
    if(! var.isEmpty())\
        ui->control->setText(var);\
\
    update();\
}

OPEN_IMAGE(on_img_button_clicked, img_fname, img, "image")
OPEN_IMAGE(on_a_button_clicked, a_fname, a, "A")
OPEN_IMAGE(on_dh_button_clicked, dh_fname, dh, "Dh")
OPEN_IMAGE(on_dv_button_clicked, dv_fname, dv, "Dv")
OPEN_IMAGE(on_dd_button_clicked, dd_fname, dd, "Dd")

void MainDialog::update()
{
    const bool is_button_enable = ! (img_fname.isEmpty() ||
            a_fname.isEmpty() || dh_fname.isEmpty() || dv_fname.isEmpty() || dd_fname.isEmpty());

    ui->up_button->setEnabled(is_button_enable);
    ui->down_button->setEnabled(is_button_enable);

    ui->jpeg_button->setEnabled(! img_fname.isEmpty());
}

void MainDialog::on_down_button_clicked()
{
    QImage img(img_fname);

    if(! img.isNull())
    {
        unsigned v, u;
        QImage a(img.size() / 2, QImage::Format_RGB888),
                dv(img.size() / 2, QImage::Format_RGB888),
                dh(img.size() / 2, QImage::Format_RGB888),
                dd(img.size() / 2, QImage::Format_RGB888);
        QColor img_pixel[4], a_pixel, dv_pixel, dh_pixel, dd_pixel;

        for(v = 0; v < a.height(); v++)
            for(u = 0; u < a.width(); u++)
            {
                img_pixel[0] = img.pixel(2 * u,     2 * v);
                img_pixel[1] = img.pixel(2 * u,     2 * v + 1);
                img_pixel[2] = img.pixel(2 * u + 1, 2 * v);
                img_pixel[3] = img.pixel(2 * u + 1, 2 * v + 1);

#define SET_PIXEL(pxl, set_func, get_func, c0, c1, c2, c3, c4)\
    pxl.set_func(\
        (\
            c0 * img_pixel[0].get_func() +\
            c1 * img_pixel[1].get_func() +\
            c2 * img_pixel[2].get_func() +\
            c3 * img_pixel[3].get_func()\
        ) / 4 + c4);

#define SET_ALL_PIXEL(pxl, c0, c1, c2, c3, c4)\
    SET_PIXEL(pxl, setBlue, blue, c0, c1, c2, c3, c4);\
    SET_PIXEL(pxl, setGreen, green, c0, c1, c2, c3, c4);\
    SET_PIXEL(pxl, setRed, red, c0, c1, c2, c3, c4);

                SET_ALL_PIXEL(a_pixel, 1, 1, 1, 1, 0);
                SET_ALL_PIXEL(dv_pixel, 1, -1, 1, -1, 127.5);
                SET_ALL_PIXEL(dh_pixel, 1, 1, -1, -1, 127.5);
                SET_ALL_PIXEL(dd_pixel, 1, -1, -1, 1, 127.5);

                a.setPixel(u, v, a_pixel.rgb());
                dv.setPixel(u, v, dv_pixel.rgb());
                dh.setPixel(u, v, dh_pixel.rgb());
                dd.setPixel(u, v, dd_pixel.rgb());
            }

        a.save(a_fname);
        dv.save(dv_fname);
        dh.save(dh_fname);
        dd.save(dd_fname);
    }
    else
        QMessageBox::critical(this, trUtf8("Ошибка"), trUtf8("Исходное изображение не существует"));
}

void MainDialog::on_up_button_clicked()
{
    QImage a(a_fname), dv(dv_fname), dh(dh_fname), dd(dd_fname);

    if(! (a.isNull() || dv.isNull() || dh.isNull() || dd.isNull()))
    {
        unsigned v, u;
        QImage img(a.size() * 2, QImage::Format_RGB888);
        QColor img_pixel[4], a_pixel, dv_pixel, dh_pixel, dd_pixel;
        int ip[4][3], ap[3], dvp[3], dhp[3], ddp[3];

        for(v = 0; v < a.height(); v++)
            for(u = 0; u < a.width(); u++)
            {
                a_pixel = a.pixel(v, u);
                dv_pixel = dv.pixel(v, u);
                dh_pixel = dh.pixel(v, u);
                dd_pixel = dd.pixel(v, u);

                ap[0] = a_pixel.blue();
                ap[1] = a_pixel.green();
                ap[2] = a_pixel.red();

                dvp[0] = dv_pixel.blue() - 127.5;
                dvp[1] = dv_pixel.green() - 127.5;
                dvp[2] = dv_pixel.red() - 127.5;

                dhp[0] = dh_pixel.blue() - 127.5;
                dhp[1] = dh_pixel.green() - 127.5;
                dhp[2] = dh_pixel.red() - 127.5;

                ddp[0] = dd_pixel.blue() - 127.5;
                ddp[1] = dd_pixel.green() - 127.5;
                ddp[2] = dd_pixel.red() - 127.5;

                ip[0][0] = (ap[0] + dvp[0] + dhp[0] + ddp[0]) / 4;
                ip[0][1] = (ap[1] + dvp[1] + dhp[1] + ddp[1]) / 4;
                ip[0][2] = (ap[2] + dvp[2] + dhp[2] + ddp[2]) / 4;

                ip[1][0] = (ap[0] - dvp[0] + dhp[0] - ddp[0]) / 4;
                ip[1][1] = (ap[1] - dvp[1] + dhp[1] - ddp[1]) / 4;
                ip[1][2] = (ap[2] - dvp[2] + dhp[2] - ddp[2]) / 4;

                ip[2][0] = (ap[0] + dvp[0] - dhp[0] - ddp[0]) / 4;
                ip[2][1] = (ap[1] + dvp[1] - dhp[1] - ddp[1]) / 4;
                ip[2][2] = (ap[2] + dvp[2] - dhp[2] - ddp[2]) / 4;

                ip[3][0] = (ap[0] - dvp[0] - dhp[0] + ddp[0]) / 4;
                ip[3][1] = (ap[1] - dvp[1] - dhp[1] + ddp[1]) / 4;
                ip[3][2] = (ap[2] - dvp[2] - dhp[2] + ddp[2]) / 4;

                img_pixel[0].setBlue(ip[0][0]);
                img_pixel[0].setGreen(ip[0][1]);
                img_pixel[0].setRed(ip[0][2]);

                img_pixel[1].setBlue(ip[1][0]);
                img_pixel[1].setGreen(ip[1][1]);
                img_pixel[1].setRed(ip[1][2]);

                img_pixel[2].setBlue(ip[2][0]);
                img_pixel[2].setGreen(ip[2][1]);
                img_pixel[2].setRed(ip[2][2]);

                img_pixel[3].setBlue(ip[3][0]);
                img_pixel[3].setGreen(ip[3][1]);
                img_pixel[3].setRed(ip[3][2]);

                img.setPixel(2 * u,     2 * v, img_pixel[0].rgb());
                img.setPixel(2 * u + 1, 2 * v, img_pixel[1].rgb());
                img.setPixel(2 * u    , 2 * v + 1, img_pixel[2].rgb());
                img.setPixel(2 * u + 1, 2 * v + 1, img_pixel[3].rgb());
            }

        img.save(img_fname);
    }
    else
        QMessageBox::critical(this, trUtf8("Ошибка"), trUtf8("Изображения не существуют"));
}

void MainDialog::on_jpeg_button_clicked()
{
    QImage img(img_fname);

    if(! img.isNull())
        img.save(img_fname + ".jpeg");
    else
        QMessageBox::critical(this, trUtf8("Ошибка"), trUtf8("Исходное изображение не существует"));
}
