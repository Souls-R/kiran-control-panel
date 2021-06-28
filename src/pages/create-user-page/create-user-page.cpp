#include "create-user-page.h"
#include "accounts-global-info.h"
#include "kiran-tips.h"
#include "passwd-helper.h"
#include "src/pages/advance-settings-page/advance-settings.h"
#include "ui_create-user-page.h"
#include "user-name-validator.h"

#include <kiranwidgets-qt5/kiran-message-box.h>
#include <widget-property-helper.h>
#include <QDebug>
#include <QListView>
#include <QMessageBox>
#include <qt5-log-i.h>

CreateUserPage::CreateUserPage(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::CreateUserPage)
{
    ui->setupUi(this);
    initUI();
}

CreateUserPage::~CreateUserPage()
{
    delete ui;
}

void CreateUserPage::reset()
{
    ui->avatar->setDefaultImage();
    ui->edit_name->clear();
    ui->combo_userType->setCurrentIndex(0);
    ui->editcheck_passwd->resetVerificationStatus();
    ui->editcheck_passwd->clear();
    ui->editcheck_confirmPasswd->resetVerificationStatus();
    ui->editcheck_confirmPasswd->clear();
    m_errorTip->hideTip();

    m_advanceSettingsInfo.clear();
}

void CreateUserPage::setAvatarIconPath(const QString &iconPath)
{
    ui->avatar->setImage(iconPath);
}

//初始化界面
void CreateUserPage::initUI()
{
    /// 提示框
    m_errorTip = new KiranTips(this);
    m_errorTip->setShowPosition(KiranTips::POSITION_BOTTM);
    m_errorTip->setAnimationEnable(true);

    /// 用户头像
    ui->avatar->setHoverImage(":/kcp-account-images/change_user_icon.png");
    ui->avatar->setClickEnable(true);
    connect(ui->avatar, &UserAvatarWidget::pressed, [this]() {
        emit sigSetIconForNewUser(ui->avatar->iconPath());
    });

    /// 用户类型ComboBox
    QListView *view = new QListView(ui->combo_userType);
    ui->combo_userType->setView(view);
    ui->combo_userType->addItem(tr("standard"));
    ui->combo_userType->addItem(tr("administrator"));
    ui->combo_userType->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    /// 用户名输入框
    ui->edit_name->setValidator(new UserNameValidator(ui->edit_name));
    //NOTE:用户名不能超过32字符长
    ui->edit_name->setMaxLength(32);

    /// 密码输入框
    ui->editcheck_passwd->setMaxLength(24);
    ui->editcheck_passwd->setEchoMode(QLineEdit::Password);
    ui->editcheck_confirmPasswd->setMaxLength(24);
    ui->editcheck_confirmPasswd->setEchoMode(QLineEdit::Password);

    /// 高级设置按钮
    connect(ui->btn_advanceSetting, &QPushButton::clicked, [this]() {
        if (ui->edit_name->text().isEmpty())
        {
            m_errorTip->setText(tr("Please enter user name first"));
            m_errorTip->showTipAroundWidget(ui->edit_name);
            return;
        }
        AdvanceSettings advanceSettings(nullptr);
        advanceSettings.show();
        advanceSettings.setInfo(ui->edit_name->text(), m_advanceSettingsInfo);
        QEventLoop eventLoop;
        connect(&advanceSettings, &AdvanceSettings::sigClose, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
        if (advanceSettings.isConfirm())
        {
            m_advanceSettingsInfo = advanceSettings.getInfo();
        }
    });

    /// 确认按钮
    Kiran::WidgetPropertyHelper::setButtonType(ui->btn_confirm, Kiran::BUTTON_Default);
    connect(ui->btn_confirm, &QPushButton::clicked, this, &CreateUserPage::handlerCreateNewUser);

    /// 取消按钮
    connect(ui->btn_cancel, &QPushButton::clicked, [this]() {
        reset();
    });
}

void CreateUserPage::handlerCreateNewUser()
{
    //step1.检验用户名是否为空，是否重名
    KLOG_INFO() << "start check user name";
    QString userName = ui->edit_name->text();

    if (userName.isEmpty())
    {
        m_errorTip->setText(tr("Please enter your user name"));
        m_errorTip->showTipAroundWidget(ui->edit_name);
        return;
    }

    bool isPureDigital = true;
    for (QChar ch : userName)
    {
        if (!ch.isNumber())
        {
            isPureDigital = false;
            break;
        }
    }
    if (isPureDigital)
    {
        m_errorTip->setText(tr("user name cannot be a pure number"));
        m_errorTip->showTipAroundWidget(ui->edit_name);
        return;
    }

    if (!AccountsGlobalInfo::instance()->checkUserNameAvaliable(userName))
    {
        m_errorTip->setText(tr("user name already exists"));
        m_errorTip->showTipAroundWidget(ui->edit_name);
        return;
    }

    //step2.检验密码、确认密码是否为空，是否相等
    KLOG_INFO() << "start check passwd,confirm passwd";
    QString passwd = ui->editcheck_passwd->text();
    QString confirmPasswd = ui->editcheck_confirmPasswd->text();
    if (passwd.isEmpty())
    {
        ui->editcheck_passwd->setVerificationStatus(false);
        m_errorTip->setText(tr("Please enter your password"));
        m_errorTip->showTipAroundWidget(ui->editcheck_passwd);
        return;
    }
    if (confirmPasswd.isEmpty())
    {
        ui->editcheck_confirmPasswd->setVerificationStatus(false);
        m_errorTip->setText(tr("Please enter the password again"));
        m_errorTip->showTipAroundWidget(ui->editcheck_confirmPasswd);
        return;
    }
    if (passwd != confirmPasswd)
    {
        ui->editcheck_confirmPasswd->setVerificationStatus(false);
        m_errorTip->setText(tr("The password you enter must be the same as the former one"));
        m_errorTip->showTipAroundWidget(ui->editcheck_confirmPasswd);
        return;
    }

    //step3.调用crypt密码加密
    KLOG_INFO() << "start encrypted passwd";
    QString encryptedPasswd;
    if (!PasswdHelper::encryptPassword(passwd, encryptedPasswd))
    {
        QMessageBox::warning(this, tr("Error"), tr("Password encryption failed"));
        return;
    }

    qint64 uid = -1;
    if (!m_advanceSettingsInfo.uid.isEmpty())
    {
        bool toNumberOk = false;
        uid = m_advanceSettingsInfo.uid.toLongLong(&toNumberOk);
        if (!toNumberOk)
        {
            uid = -1;
        }
    }
    int accountType = ui->combo_userType->currentIndex();
    QString homeDir = m_advanceSettingsInfo.homeDir;
    QString shell = m_advanceSettingsInfo.shell;
    QString iconFile = ui->avatar->iconPath();

    emit sigIsBusyChanged(true);
    ui->btn_confirm->setBusy(true);
    emit sigCreateUser(userName, uid, accountType,
                       encryptedPasswd,
                       homeDir,
                       shell,
                       iconFile);
}

void CreateUserPage::handlerCreateNewUserIsDone(QString userPath,
                                                QString errMsg)
{
    emit sigIsBusyChanged(false);
    ui->btn_confirm->setBusy(false);
    if (!errMsg.isEmpty())
    {
        KiranMessageBox::message(nullptr, tr("Error"),
                                 errMsg, KiranMessageBox::Yes | KiranMessageBox::No);
    }
    if (!userPath.isEmpty())
    {
        emit sigRequestSetCurrentUser(userPath);
    }
}
