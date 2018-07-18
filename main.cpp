#include <QApplication>
#include "myaudioinput.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyAudioInput w;
    w.show();

    return a.exec();
}
