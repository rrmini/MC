#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//Этот кусок кода позволяет в дальнейшем использовать tr() и нормально отображать
// кирилицу
    QTextCodec *russianCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(russianCodec);

    QTranslator qtTranslator;
         qtTranslator.load("qt_" + QLocale::system().name(),""
                 /*QLibraryInfo::location(QLibraryInfo::TranslationsPath)*/);
         a.installTranslator(&qtTranslator);

         QTranslator myappTranslator;
         myappTranslator.load("myapp_" + QLocale::system().name());
         a.installTranslator(&myappTranslator);
// ---------------------------------------------------------------------------------
    MainWindow w;
    if (!w.createDBConnection()) return 0;

    w.show();
    
    return a.exec();
}
