#ifndef FONTS_H
#define FONTS_H

#include <QWidget>

namespace Ui {
class Fonts;
}

class ComKylinsecKiranSessionDaemonAppearanceInterface;
class Fonts : public QWidget
{
    Q_OBJECT

public:
    explicit Fonts(QWidget *parent = 0);
    ~Fonts();

private:
    bool initUI();
    void getCurrentFontInfo(int fontType);
    QStringList getFont(int fontType);
    void setFont(int fontType , QStringList fontInfoList);
    void connectSignals();

private:
    Ui::Fonts *ui;
    ComKylinsecKiranSessionDaemonAppearanceInterface *m_appearanceInterface;
    QStringList m_applicationFontInfo;
    QStringList m_windowTitleFontInfo;
    QStringList m_monospaceFontInfo;

};

#endif // FONTS_H
