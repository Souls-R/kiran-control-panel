#include "kiran-control-center-window.h"
#include "stdio.h"
#include <QApplication>
#include <QTranslator>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QDebug>

using namespace std;
extern QString gLocaleName;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();
    gLocaleName = locale;//当前翻译类型,必须在加载模块数据之前赋值.模块的名称将根据此变量选择翻译类型.
    QString qmFile = QString("%1.%2.qm").arg(TRANSLATE_PREFIX).arg(locale);
    QTranslator translator;
    if(translator.load(qmFile) == false)
        qDebug() << "load qm: " << qmFile <<  " error.";
    else
        a.installTranslator(&translator);

    //
    QCommandLineParser parser;
    parser.setApplicationDescription("kiran control center");  // 设置应用程序描述信息
    parser.addHelpOption();  // 添加帮助选项 （"-h" 或 "--help"）
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);  // 举例说明：将 "-adb" 当成一个选项来看，而不是看成 "-a -b -c"
    QCommandLineOption printPluginsPath("print-plugins-dir", QCoreApplication::translate("main", "Print plugins dir"));
    parser.addOption(printPluginsPath);
    parser.process(a);

    bool print = parser.isSet(printPluginsPath);
    if(print)
    {
        printf("*********************Print plugins dir**********************\n\n");
        printf("Plugin desktop dir: %s\n", PLUGIN_DESKTOP_DIR);
        printf("Plugin icon dir:    %s\n", PLUGIN_DESKTOP_ICON_DIR);
        printf("\n************************************************************\n");
        return 0;
    }

    //窗口屏幕居中显示.
    KiranControlCenterWindow w;

    const QRect &screenRect = QApplication::desktop()->screenGeometry();
    w.move(screenRect.center()-QPoint(w.rect().width()/2, w.rect().height()/2));
    w.show();
    w.setContentWrapperMarginBottom(0);
    return a.exec();
}
