#include "kublonwindow.h"

#include <QMouseEvent>
#include <QPainter>

#include <iostream>
#include <thread>

#include "ui_kublonwindow.h"

KubLonWindow::KubLonWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::KubLonWindow), countThread(0)
{
    ui->setupUi(this);
    reset();
}

KubLonWindow::~KubLonWindow()
{
    delete ui;
}

int KubLonWindow::row(int index)
{
    return index == 0 ? 0 : (index == 5 ? 2 : 1);
}

int KubLonWindow::column(int index)
{
    return (index == 0 || index == 5) ? 1 : index - 1;
}

int KubLonWindow::index(int row, int column)
{
    switch (row) {
        case 0:
            if (column == 1) {
                return 0;
            }

            break;

        case 1:
            if (column <= 3) {
                return column + 1;
            }

            break;

        case 2:
            if (column == 1) {
                return 5;
            }

            break;
    }

    return -1;
}

void KubLonWindow::addThread(int n1, int n2, int countRotate)
{
    KubThread(matrics, n1, n2, 1, countRotate, 2);

    --countThread;

    if (countThread == 0) {
        ui->findBtn->setEnabled(true);
        std::cout << std::endl;
    }
}

void KubLonWindow::rotate(int n)
{
    int buf = matrics[n][0][0];
    matrics[n][0][0] = matrics[n][2][0];
    matrics[n][2][0] = matrics[n][2][2];
    matrics[n][2][2] = matrics[n][0][2];
    matrics[n][0][2] = buf;

    buf = matrics[n][0][1];
    matrics[n][0][1] = matrics[n][1][0];
    matrics[n][1][0] = matrics[n][2][1];
    matrics[n][2][1] = matrics[n][1][2];
    matrics[n][1][2] = buf;

    switch (n) {
        case 0:
            for (int i = 0; i < KubThread::COUNT_SECTION; ++i) {
                buf = matrics[1][0][i];
                matrics[1][0][i] = matrics[2][0][i];
                matrics[2][0][i] = matrics[3][0][i];
                matrics[3][0][i] = matrics[4][0][i];
                matrics[4][0][i] = buf;
            }
            break;

        case 1:
            for (int i = 0; i < KubThread::COUNT_SECTION; ++i) {
                buf = matrics[0][i][0];
                matrics[0][i][0] = matrics[4][2 - i][2];
                matrics[4][2 - i][2] = matrics[5][i][0];
                matrics[5][i][0] = matrics[2][i][0];
                matrics[2][i][0] = buf;
            }
            break;

        case 2:
            for (int i = 0; i < KubThread::COUNT_SECTION; ++i) {
                buf = matrics[0][2][i];
                matrics[0][2][i] = matrics[1][2 - i][2];
                matrics[1][2 - i][2] = matrics[5][0][2 - i];
                matrics[5][0][2 - i] = matrics[3][i][0];
                matrics[3][i][0] = buf;
            }
            break;

        case 3:
            for (int i = 0; i < KubThread::COUNT_SECTION; ++i) {
                buf = matrics[0][2 - i][2];
                matrics[0][2 - i][2] = matrics[2][2 - i][2];
                matrics[2][2 - i][2] = matrics[5][2 - i][2];
                matrics[5][2 - i][2] = matrics[4][i][0];
                matrics[4][i][0] = buf;
            }
            break;

        case 4:
            for (int i = 0; i < KubThread::COUNT_SECTION; ++i) {
                buf = matrics[0][0][2 - i];
                matrics[0][0][2 - i] = matrics[3][2 - i][2];
                matrics[3][2 - i][2] = matrics[5][2][i];
                matrics[5][2][i] = matrics[1][i][0];
                matrics[1][i][0] = buf;
            }
            break;

        case 5:
            for (int i = 0; i < KubThread::COUNT_SECTION; ++i) {
                buf = matrics[4][2][i];
                matrics[4][2][i] = matrics[3][2][i];
                matrics[3][2][i] = matrics[2][2][i];
                matrics[2][2][i] = matrics[1][2][i];
                matrics[1][2][i] = buf;
            }
            break;
    }
}

void KubLonWindow::reset()
{
    for (int index = 0; index < 6; ++index) {
        for (int i = 0; i < KubThread::COUNT_SECTION; ++i) {
            for (int j = 0; j < KubThread::COUNT_SECTION; ++j) {
                matrics[index][i][j] = index;
            }
        }
    }
}

void KubLonWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(QBrush(Qt::black), 3.));

    for (int index = 0; index < 6; ++index) {
        for (int i = 0; i < KubThread::COUNT_SECTION; ++i) {
            for (int j = 0; j < KubThread::COUNT_SECTION; ++j) {
                switch (matrics[index][i][j]) {
                    case -1: painter.setBrush(QBrush(Qt::gray)); break;
                    case 0: painter.setBrush(QBrush(Qt::yellow)); break;
                    case 1: painter.setBrush(QBrush(QColor(255, 127, 39))); break;
                    case 2: painter.setBrush(QBrush(QColor(0, 158, 210))); break;
                    case 3: painter.setBrush(QBrush(Qt::red)); break;
                    case 4: painter.setBrush(QBrush(Qt::green)); break;
                    case 5: painter.setBrush(QBrush(Qt::white)); break;
                }

                painter.drawRect(
                    j * SIZE_SECTION + SHIFT +
                        column(index) * (KubThread::COUNT_SECTION * SIZE_SECTION + APERTURE),
                    i * SIZE_SECTION + SHIFT +
                        row(index) * (KubThread::COUNT_SECTION * SIZE_SECTION + APERTURE),
                    SIZE_SECTION, SIZE_SECTION);
            }
        }

        painter.drawText(
            QRectF(SIZE_SECTION + SHIFT +
                       column(index) * (KubThread::COUNT_SECTION * SIZE_SECTION + APERTURE),
                   SIZE_SECTION + SHIFT +
                       row(index) * (KubThread::COUNT_SECTION * SIZE_SECTION + APERTURE),
                   SIZE_SECTION, SIZE_SECTION),
            names[index], QTextOption(Qt::AlignCenter));
    }
}

void KubLonWindow::mousePressEvent(QMouseEvent *ev)
{
    if (ev->x() >= SHIFT && ev->y() >= SHIFT) {
        int n = index((ev->y() - SHIFT) / (KubThread::COUNT_SECTION * SIZE_SECTION + APERTURE),
                      (ev->x() - SHIFT) / (KubThread::COUNT_SECTION * SIZE_SECTION + APERTURE));

        if (n != -1) {
            int x = ev->x() -
                    (SHIFT + column(n) * (KubThread::COUNT_SECTION * SIZE_SECTION + APERTURE));
            int y = ev->y() -
                    (SHIFT + row(n) * (KubThread::COUNT_SECTION * SIZE_SECTION + APERTURE));
            int row = y / SIZE_SECTION;
            int column = x / SIZE_SECTION;

            if (row != 3 && column != 3) {
                if (row == 1 && column == 1) {
                    switch (ev->button()) {
                        case Qt::LeftButton: rotate(n); break;
                        case Qt::RightButton:
                            rotate(n);
                            rotate(n);
                            rotate(n);

                            break;
                    }

                } else {
                    switch (ev->button()) {
                        case Qt::LeftButton:
                            ++matrics[n][row][column];

                            if (matrics[n][row][column] == 6) {
                                matrics[n][row][column] = -1;
                            }

                            break;
                        case Qt::RightButton:
                            --matrics[n][row][column];

                            if (matrics[n][row][column] == -2) {
                                matrics[n][row][column] = 5;
                            }

                            break;
                    }
                }

                repaint();
            }
        }
    }
}

void KubLonWindow::on_findBtn_clicked()
{
    ui->findBtn->setEnabled(false);

    for (int i = 0; i < 18; ++i) {
        int n = ((i % 6) < 3) ? 15 : 12;
        for (int j = 1; j <= n; ++j) {
            ++countThread;
            std::thread th(&KubLonWindow::addThread, this, i, j, ui->spinBox->value() - 1);
            th.detach();
        }
    }
}

void KubLonWindow::on_resetBtn_clicked()
{
    reset();
    repaint();
}
