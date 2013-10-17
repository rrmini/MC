#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//Этот кусок кода позволяет в дальнейшем использовать tr() и нормально отображать
// кирилицу
//    QTextCodec *russianCodec = QTextCodec::codecForName("UTF-8");
//    QTextCodec::setCodecForLocale(russianCodec);

//    QTranslator qtTranslator;
//         qtTranslator.load("qt_" + QLocale::system().name(),
//                 QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//         a.installTranslator(&qtTranslator);

//         QTranslator appTranslator;
//         appTranslator.load("myapp_" + QLocale::system().name()/*, qmPath*/);
//         a.installTranslator(&appTranslator);
// ---------------------------------------------------------------------------------
    MainWindow w;
   // if (!w.createDBConnection()) return 0;

    w.show();
    
    return app.exec();
}
