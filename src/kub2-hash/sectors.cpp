#include "sectors.h"

#include <QStringList>

Sectors::Sectors()
{
    int col = -1;
    for (int index = 0; index < 6; ++index) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                matrics[index][i][j] = ++col;
            }
        }
    }
}

Sectors::Sectors(const Sectors &sec)
{
    for (int index = 0; index < 6; ++index) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                matrics[index][i][j] = sec.matrics[index][i][j];
            }
        }
    }
}

bool operator==(const Sectors &s1, const Sectors &s2)
{
    for (int i1 = 0; i1 < 6; ++i1) {
        for (int i2 = 0; i2 < 2; ++i2) {
            for (int i3 = 0; i3 < 2; ++i3) {
                if (s1.matrics[i1][i2][i3] != s2.matrics[i1][i2][i3]) {
                    return false;
                }
            }
        }
    }

    return true;
}

uint qHash(const Sectors &s)
{
    uint res = 0;

    for (int i1 = 0; i1 < 6; ++i1) {
        for (int i2 = 0; i2 < 2; ++i2) {
            for (int i3 = 0; i3 < 2; ++i3) {
                res *= 31;
                res += s.matrics[i1][i2][i3];
            }
        }
    }

    return res;
}

QString Sectors::toString()
{
    QString res;

    for (int i1 = 0; i1 < 6; ++i1) {
        for (int i2 = 0; i2 < 2; ++i2) {
            for (int i3 = 0; i3 < 2; ++i3) {
                res += QString::number(matrics[i1][i2][i3]) + " ";
            }
        }
    }

    return res;
}

Sectors Sectors::fromString(const QString &str)
{
    Sectors sec;
    int index = 0;
    auto split = str.split(" ");

    for (int i1 = 0; i1 < 6; ++i1) {
        for (int i2 = 0; i2 < 2; ++i2) {
            for (int i3 = 0; i3 < 2; ++i3) {
                sec.matrics[i1][i2][i3] = split[index].toInt();
                ++index;
            }
        }
    }

    return sec;
}

void Sectors::rotate(int n)
{
    switch (n) {
        case U:
            currentRotate(Up);

            for (int i = 0; i < 2; ++i) {
                adjacentRotate(matrics[1][0][i], matrics[2][0][i], matrics[3][0][i],
                               matrics[4][0][i]);
            }

            break;

        case U2:
            currentDoubleRotate(Up);

            for (int i = 0; i < 2; ++i) {
                adjacentDoubleRotate(matrics[1][0][i], matrics[2][0][i], matrics[3][0][i],
                                     matrics[4][0][i]);
            }

            break;

        case U3:
            currentReversRotate(Up);

            for (int i = 0; i < 2; ++i) {
                adjacentReversRotate(matrics[1][0][i], matrics[2][0][i], matrics[3][0][i],
                                     matrics[4][0][i]);
            }

            break;

        case R:
            currentRotate(Right);

            for (int i = 0; i < 2; ++i) {
                adjacentRotate(matrics[0][1 - i][1], matrics[2][1 - i][1], matrics[5][1 - i][1],
                               matrics[4][i][0]);
            }

            break;

        case R2:
            currentDoubleRotate(Right);

            for (int i = 0; i < 2; ++i) {
                adjacentDoubleRotate(matrics[0][1 - i][1], matrics[2][1 - i][1],
                                     matrics[5][1 - i][1], matrics[4][i][0]);
            }

            break;

        case R3:
            currentReversRotate(Right);

            for (int i = 0; i < 2; ++i) {
                adjacentReversRotate(matrics[0][1 - i][1], matrics[2][1 - i][1],
                                     matrics[5][1 - i][1], matrics[4][i][0]);
            }

            break;

        case F:
            currentRotate(Forward);

            for (int i = 0; i < 2; ++i) {
                adjacentRotate(matrics[0][1][i], matrics[1][1 - i][1], matrics[5][0][1 - i],
                               matrics[3][i][0]);
            }

            break;

        case F2:
            currentDoubleRotate(Forward);

            for (int i = 0; i < 2; ++i) {
                adjacentDoubleRotate(matrics[0][1][i], matrics[1][1 - i][1],
                                     matrics[5][0][1 - i], matrics[3][i][0]);
            }

            break;

        case F3:
            currentReversRotate(Forward);

            for (int i = 0; i < 2; ++i) {
                adjacentReversRotate(matrics[0][1][i], matrics[1][1 - i][1],
                                     matrics[5][0][1 - i], matrics[3][i][0]);
            }

            break;
    }
}

void Sectors::currentRotate(int n)
{
    adjacentRotate(matrics[n][0][0], matrics[n][1][0], matrics[n][1][1], matrics[n][0][1]);
}

void Sectors::currentDoubleRotate(int n)
{
    adjacentDoubleRotate(matrics[n][0][0], matrics[n][1][0], matrics[n][1][1],
                         matrics[n][0][1]);
}

void Sectors::currentReversRotate(int n)
{
    adjacentReversRotate(matrics[n][0][0], matrics[n][1][0], matrics[n][1][1],
                         matrics[n][0][1]);
}

void Sectors::adjacentRotate(int &n1, int &n2, int &n3, int &n4)
{
    std::swap(n1, n2);
    std::swap(n3, n4);
    std::swap(n1, n3);
}

void Sectors::adjacentDoubleRotate(int &n1, int &n2, int &n3, int &n4)
{
    std::swap(n1, n3);
    std::swap(n2, n4);
}

void Sectors::adjacentReversRotate(int &n1, int &n2, int &n3, int &n4)
{
    std::swap(n1, n2);
    std::swap(n3, n4);
    std::swap(n2, n4);
}
