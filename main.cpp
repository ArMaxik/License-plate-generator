#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 1);
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);

//    // Load an application style
//    QFile styleFile( "D:/qss/ConsoleStyle.qss" );
//    styleFile.open( QFile::ReadOnly );

//    // Apply the loaded stylesheet
//    QString style( styleFile.readAll() );
//    a.setStyleSheet( style );

//    qDebug() << QString("sffsdf     + wef-fsdfsdf*(31234) /dsfgse + 32/1")
//                .remove(QRegularExpression("\\s"))
//                .replace(QRegularExpression("\\w+"), "<ид>");

    MainWindow w;
    w.show();

    return a.exec();
}
