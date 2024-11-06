#include <QApplication>
#include"LoginRegisterDlg.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    LoginRegisterDlg w;
    w.show();

    return a.exec();
}
