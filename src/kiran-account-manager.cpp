#include "kiran-account-manager.h"
#include "account-itemwidget.h"
#include "accounts-global-info.h"
#include "auth-manager-page/auth-manager-page.h"
#include "config.h"
#include "create-user-page/create-user-page.h"
#include "hard-worker.h"
#include "mask-widget.h"
#include "select-avatar-page/select-avatar-page.h"
#include "user-info-page/user-info-page.h"

#include <kiran-sidebar-widget.h>
#include <kiran-style-public-define.h>
#include <QDebug>
#include <QHBoxLayout>
#include <QIcon>
#include <QStackedWidget>
#include <QtWidgets/QListWidgetItem>

#define ITEM_USER_OBJ_PATH_ROLE Qt::UserRole + 1

enum StackWidgetPageEnum
{
    PAGE_CREATE_USER,
    PAGE_USER_INFO,
    PAGE_SELECT_AVATAR,
    PAGE_AUTH_MANAGER
};

KiranAccountManager::KiranAccountManager()
    : KiranTitlebarWindow()
{
    m_workThread.start();
    m_hardworker = new HardWorker();
    m_hardworker->moveToThread(&m_workThread);
    initUI();
}

KiranAccountManager::~KiranAccountManager()
{
    if (m_workThread.isRunning())
    {
        m_workThread.quit();
        m_workThread.wait();
    }
    delete m_hardworker;
};

void KiranAccountManager::setCurrentUser(const QString &userPath)
{
    int findIdx = -1;
    for (int i = 0; i < m_tabList->count(); i++)
    {
        if (m_tabList->item(i)->data(Qt::UserRole) != userPath)
        {
            continue;
        }
        findIdx = i;
        break;
    }
    Q_ASSERT(findIdx != -1);
    m_tabList->setCurrentRow(findIdx);
}

void KiranAccountManager::appendSiderbarItem(const QString &userPath)
{
    UserInterface interface(userPath, QDBusConnection::systemBus());
    auto          item = new QListWidgetItem(interface.user_name(), m_tabList);
    item->setIcon(QPixmap(interface.icon_file()));
    item->setData(Kiran::ItemStatus_Role, interface.locked() ? tr("disable") : tr("enable"));
    item->setData(Kiran::ItemStatusColor_Role, interface.locked() ? QColor("#fa4949") : QColor("#43a3f2"));
    item->setData(ITEM_USER_OBJ_PATH_ROLE, userPath);

    m_tabList->addItem(item);
}

void KiranAccountManager::setDefaultSiderbarItem()
{
    //设置默认侧边栏项
    if (m_tabList->count() > 1)
    {
        m_tabList->setCurrentRow(1);
    }
    else
    {
        m_tabList->setCurrentRow(0);
    }
}

void KiranAccountManager::initUI()
{
    setObjectName("KiranAccountManager");
    setTitle(tr("User Manager"));
    setIcon(QIcon(DESKTOP_ICON_PATH));

    /* 遮罩,用于繁忙时屏蔽用户操作 */
    m_maskWidget = new MaskWidget(this);
    m_maskWidget->setVisible(false);

    /* 初始化界面主布局 */
    auto contentLayout = new QHBoxLayout(getWindowContentWidget());
    contentLayout->setSpacing(0);
    contentLayout->setObjectName("AccountContentLayout");
    contentLayout->setContentsMargins(0, 0, 0, 0);

    /* 侧边栏 */
    auto siderbar = new QWidget(getWindowContentWidget());
    contentLayout->addWidget(siderbar);
    siderbar->setObjectName("siderWidget");
    siderbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    siderbar->setFixedWidth(328);

    auto vLayout = new QVBoxLayout(siderbar);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setObjectName("SiderbarVLayout");

    m_tabList = new KiranSidebarWidget(siderbar);
    m_tabList->setObjectName("tabList");
    m_tabList->setIconSize(QSize(40, 40));
    vLayout->addWidget(m_tabList);
    initUserList();

    /* 内容区域 */
    m_stackWidget = new QStackedWidget(getWindowContentWidget());
    contentLayout->addWidget(m_stackWidget);
    m_stackWidget->setObjectName("StackWidget");

    m_page_createUser = new CreateUserPage(m_stackWidget);
    m_stackWidget->insertWidget(PAGE_CREATE_USER, m_page_createUser);
    initPageCreateUser();

    m_page_userinfo = new UserInfoPage(m_stackWidget);
    m_stackWidget->insertWidget(PAGE_USER_INFO, m_page_userinfo);
    initPageUserInfo();

    m_page_selectAvatar = new SelectAvatarPage(m_stackWidget);
    m_stackWidget->insertWidget(PAGE_SELECT_AVATAR, m_page_selectAvatar);
    initPageSelectAvatar();

    m_page_authManager = new AuthManagerPage(m_stackWidget);
    m_stackWidget->insertWidget(PAGE_AUTH_MANAGER, m_page_authManager);
    initPageAuthManager();

    connectToInfoChanged();
    setDefaultSiderbarItem();

    resize(800, 600);
}

void KiranAccountManager::initUserList()
{
    QListWidgetItem *item;

    connect(m_tabList, &KiranSidebarWidget::itemSelectionChanged, [this]() {
        QList<QListWidgetItem *> selecteds = m_tabList->selectedItems();
        if (selecteds.size() != 1)
        {
            qFatal("tabList: selecteds size != 1");
            return;
        }
        QListWidgetItem *item = selecteds.at(0);
        if (item == m_createUserItem)
        {
            //重置创建用户页面
            m_page_createUser->reset();
            //切换到创建用户
            m_stackWidget->setCurrentIndex(PAGE_CREATE_USER);
        }
        else
        {
            QString usrObjPath = item->data(ITEM_USER_OBJ_PATH_ROLE).toString();
            //更新用户信息页面
            m_page_userinfo->setCurrentShowUserPath(usrObjPath);
            //切换到用户信息
            m_stackWidget->setCurrentIndex(PAGE_USER_INFO);
        }
    });

    ///创建用户按钮
    m_createUserItem = new QListWidgetItem(tr("Create new account"), m_tabList);
    m_createUserItem->setIcon(QIcon(":/images/add_icon.png"));
    m_tabList->addItem(item);

    //加载非系统用户
    QList<QString> userObjList;
    userObjList = AccountsGlobalInfo::instance()->getUserList();
    for (auto &iter : userObjList)
    {
        appendSiderbarItem(iter);
    }
}

void KiranAccountManager::initPageCreateUser()
{
    //用户头像点击
    connect(m_page_createUser, &CreateUserPage::sigSetIconForNewUser, [this](QString iconPath) {
        //设置选择头像模式，为了缓存选择头像之前的页面，方便之后的返回
        m_page_selectAvatar->setMode(SelectAvatarPage::SELECT_AVATAR_FOR_NEW_USER);
        //设置头像，切换到选择头像页面
        m_page_selectAvatar->setCurrentAvatar(iconPath);
        m_stackWidget->setCurrentIndex(PAGE_SELECT_AVATAR);
    });
    connect(m_page_createUser, &CreateUserPage::sigRequestSetCurrentUser, [this](const QString &userPath) {
        //保证在设置当前行时,新用户已在侧边栏创建节点
        QTimer::singleShot(0, this, [=]() {
            int findIdx = -1;
            for (int i = 0; i < m_tabList->count(); i++)
            {
                if (m_tabList->item(i)->data(ITEM_USER_OBJ_PATH_ROLE) != userPath)
                {
                    continue;
                }
                findIdx = i;
                break;
            }
            Q_ASSERT(findIdx != -1);
            m_tabList->setCurrentRow(findIdx);
        });
    });
    connect(m_page_createUser, &CreateUserPage::sigCreateUser,
            m_hardworker, &HardWorker::doCreateUser);

    connect(m_hardworker, &HardWorker::sigCreateUserDnoe,
            m_page_createUser, &CreateUserPage::handlerCreateNewUserIsDone);

    connect(m_page_createUser, &CreateUserPage::sigIsBusyChanged,
            this, &KiranAccountManager::setMaskVisible);
}

void KiranAccountManager::initPageUserInfo()
{
    //用户信息页面处理头像点击
    connect(m_page_userinfo, &UserInfoPage::sigUserChangeIcon, [this](const QString &iconPath) {
        m_page_selectAvatar->setMode(SelectAvatarPage::CHANGE_AVATAR_FOR_USER);
        m_page_selectAvatar->setCurrentAvatar(iconPath);
        m_stackWidget->setCurrentIndex(PAGE_SELECT_AVATAR);
    });

    //用户信息页面，认证管理点击时请求跳转至认证管理页面
    connect(m_page_userinfo, &UserInfoPage::sigAuthManager, [this](const QString &userObj) {
        m_page_authManager->setCurrentUser(userObj);
        m_stackWidget->setCurrentIndex(PAGE_AUTH_MANAGER);
    });

    /// 修改属性
    connect(m_page_userinfo, &UserInfoPage::sigUpdateUserProperty,
            m_hardworker, &HardWorker::doUpdateUserProperty);

    connect(m_hardworker, &HardWorker::sigUpdateUserPropertyDone,
            m_page_userinfo, &UserInfoPage::handlerUpdateUserPropertyDone);

    /// 修改密码
    connect(m_page_userinfo, &UserInfoPage::sigUpdatePasswd,
            m_hardworker, &HardWorker::doUpdatePasswd);
    connect(m_hardworker, &HardWorker::sigUpdatePasswdDone,
            m_page_userinfo, &UserInfoPage::handlerUpdatePasswdDone);

    /// 删除用户
    connect(m_page_userinfo, &UserInfoPage::sigDeleteUser,
            m_hardworker, &HardWorker::doDeleteUser);
    connect(m_hardworker, &HardWorker::sigDeleteUserDone,
            m_page_userinfo, &UserInfoPage::handlerDeleteUserDone);

    /// 忙碌显示/隐藏遮罩
    connect(m_page_userinfo, &UserInfoPage::sigIsBusyChanged,
            this, &KiranAccountManager::setMaskVisible);
}

void KiranAccountManager::initPageSelectAvatar()
{
    //选择头像页面处理返回
    connect(m_page_selectAvatar, &SelectAvatarPage::sigReturnToPrevPage,
            [this](SelectAvatarPage::SelectAvatarMode mode, bool isConfirm) {
                switch (mode)
                {
                case SelectAvatarPage::SELECT_AVATAR_FOR_NEW_USER:
                    if (isConfirm)
                    {
                        m_page_createUser->setAvatarIconPath(m_page_selectAvatar->currentSelectAvatar());
                    }
                    m_stackWidget->setCurrentIndex(PAGE_CREATE_USER);
                    break;
                case SelectAvatarPage::CHANGE_AVATAR_FOR_USER:
                    if (isConfirm)
                    {
                        m_page_userinfo->setAvatarIconPath(m_page_selectAvatar->currentSelectAvatar());
                    }
                    m_stackWidget->setCurrentIndex(PAGE_USER_INFO);
                    break;
                }
            });
}

void KiranAccountManager::initPageAuthManager()
{
    //TODO:init auth manager page
    connect(m_page_authManager, &AuthManagerPage::sigReturn, [this]() {
        m_stackWidget->setCurrentIndex(PAGE_USER_INFO);
    });
}

void KiranAccountManager::connectToInfoChanged()
{
    //处理用户新增、删除
    connect(AccountsGlobalInfo::instance(), &AccountsGlobalInfo::UserAdded,
            [this](const QDBusObjectPath &obj) {
                qInfo() << "siderbar add item:" << obj.path();
                appendSiderbarItem(obj.path());
            });

    connect(AccountsGlobalInfo::instance(), &AccountsGlobalInfo::UserDeleted,
            [this](const QDBusObjectPath &obj) {
                qInfo() << "siderbar delete item:" << obj.path();
                int findIdx = -1;

                QString userPath = obj.path();
                for (int i = 0; i < m_tabList->count(); i++)
                {
                    if (m_tabList->item(i)->data(Qt::UserRole) == userPath)
                    {
                        findIdx = i;
                        break;
                    }
                }
                if (findIdx == -1)
                {
                    return;
                }
                bool             needResetSidebarItem = m_tabList->item(findIdx)->isSelected();
                QListWidgetItem *widgetItem           = m_tabList->takeItem(findIdx);
                delete widgetItem;
                if (needResetSidebarItem)
                {
                    setDefaultSiderbarItem();
                }
            });

    //处理用户属性变更
    connect(AccountsGlobalInfo::instance(), &AccountsGlobalInfo::UserPropertyChanged,
            [this](QString userPath, QString propertyName, QVariant value) {
                //侧边栏
                if ((propertyName == "locked") || (propertyName == "icon_file"))
                {
                    for (int i = 0; i < m_tabList->count(); i++)
                    {
                        QListWidgetItem *item         = m_tabList->item(i);
                        QString          itemUserPath = item->data(Qt::UserRole).toString();
                        if (itemUserPath != userPath)
                        {
                            continue;
                        }
                        UserInterface userInterface(itemUserPath, QDBusConnection::systemBus());
                        QString       userName = userInterface.user_name();
                        QString       iconFile = userInterface.icon_file();
                        bool          isLocked = userInterface.locked();
                        item->setText(userName);
                        item->setIcon(QIcon(iconFile));
                        //TODO:写入颜色
                        //                        item->setData(Kiran::ItemStatus_Role,isLocked?"disable":"enable");
                        break;
                    }
                }
                //用户详情页面
                QString currentUserPath = m_page_userinfo->getCurrentShowUserPath();
                if (userPath == currentUserPath)
                {
                    m_page_userinfo->updateInfo();
                }
            });
}

void KiranAccountManager::setMaskVisible(bool visible)
{
    if (visible)
    {
        this->stackUnder(m_maskWidget);
        m_maskWidget->show();
    }
    else
    {
        m_maskWidget->hide();
    }
}