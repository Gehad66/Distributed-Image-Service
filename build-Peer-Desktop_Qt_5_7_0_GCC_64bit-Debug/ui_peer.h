/********************************************************************************
** Form generated from reading UI file 'peer.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PEER_H
#define UI_PEER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Peer
{
public:
    QWidget *centralWidget;
    QLineEdit *local_port_line_edit;
    QLineEdit *server_ip_line_edit;
    QLineEdit *server_port_line_edit;
    QLineEdit *username_line_edit;
    QLineEdit *password_line_edit;
    QPushButton *sign_up_push_button;
    QPushButton *sign_out_push_button;
    QPushButton *sign_in_push_button;
    QPushButton *bind_push_button;
    QPushButton *send_push_button;
    QLineEdit *receiver_ip_line_edit;
    QLineEdit *receiver_port_line_edit;
    QLineEdit *image_id_line_edit;
    QLabel *image_label;
    QListWidget *images_list_widget;
    QPushButton *show_image_push_button;
    QPushButton *get_online_push_button;
    QPushButton *request_quota_push_button;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Peer)
    {
        if (Peer->objectName().isEmpty())
            Peer->setObjectName(QStringLiteral("Peer"));
        Peer->resize(658, 453);
        centralWidget = new QWidget(Peer);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        local_port_line_edit = new QLineEdit(centralWidget);
        local_port_line_edit->setObjectName(QStringLiteral("local_port_line_edit"));
        local_port_line_edit->setGeometry(QRect(20, 10, 113, 25));
        server_ip_line_edit = new QLineEdit(centralWidget);
        server_ip_line_edit->setObjectName(QStringLiteral("server_ip_line_edit"));
        server_ip_line_edit->setGeometry(QRect(20, 50, 113, 25));
        server_port_line_edit = new QLineEdit(centralWidget);
        server_port_line_edit->setObjectName(QStringLiteral("server_port_line_edit"));
        server_port_line_edit->setGeometry(QRect(20, 90, 113, 25));
        username_line_edit = new QLineEdit(centralWidget);
        username_line_edit->setObjectName(QStringLiteral("username_line_edit"));
        username_line_edit->setGeometry(QRect(150, 50, 113, 25));
        password_line_edit = new QLineEdit(centralWidget);
        password_line_edit->setObjectName(QStringLiteral("password_line_edit"));
        password_line_edit->setGeometry(QRect(150, 90, 113, 25));
        sign_up_push_button = new QPushButton(centralWidget);
        sign_up_push_button->setObjectName(QStringLiteral("sign_up_push_button"));
        sign_up_push_button->setGeometry(QRect(280, 50, 89, 25));
        sign_out_push_button = new QPushButton(centralWidget);
        sign_out_push_button->setObjectName(QStringLiteral("sign_out_push_button"));
        sign_out_push_button->setGeometry(QRect(280, 70, 89, 25));
        sign_in_push_button = new QPushButton(centralWidget);
        sign_in_push_button->setObjectName(QStringLiteral("sign_in_push_button"));
        sign_in_push_button->setGeometry(QRect(280, 90, 89, 25));
        bind_push_button = new QPushButton(centralWidget);
        bind_push_button->setObjectName(QStringLiteral("bind_push_button"));
        bind_push_button->setGeometry(QRect(280, 10, 89, 25));
        send_push_button = new QPushButton(centralWidget);
        send_push_button->setObjectName(QStringLiteral("send_push_button"));
        send_push_button->setGeometry(QRect(410, 130, 89, 25));
        receiver_ip_line_edit = new QLineEdit(centralWidget);
        receiver_ip_line_edit->setObjectName(QStringLiteral("receiver_ip_line_edit"));
        receiver_ip_line_edit->setGeometry(QRect(20, 130, 113, 25));
        receiver_port_line_edit = new QLineEdit(centralWidget);
        receiver_port_line_edit->setObjectName(QStringLiteral("receiver_port_line_edit"));
        receiver_port_line_edit->setGeometry(QRect(150, 130, 113, 25));
        image_id_line_edit = new QLineEdit(centralWidget);
        image_id_line_edit->setObjectName(QStringLiteral("image_id_line_edit"));
        image_id_line_edit->setGeometry(QRect(280, 130, 113, 25));
        image_label = new QLabel(centralWidget);
        image_label->setObjectName(QStringLiteral("image_label"));
        image_label->setGeometry(QRect(60, 180, 291, 121));
        images_list_widget = new QListWidget(centralWidget);
        images_list_widget->setObjectName(QStringLiteral("images_list_widget"));
        images_list_widget->setGeometry(QRect(380, 180, 171, 131));
        show_image_push_button = new QPushButton(centralWidget);
        show_image_push_button->setObjectName(QStringLiteral("show_image_push_button"));
        show_image_push_button->setGeometry(QRect(380, 310, 171, 21));
        get_online_push_button = new QPushButton(centralWidget);
        get_online_push_button->setObjectName(QStringLiteral("get_online_push_button"));
        get_online_push_button->setGeometry(QRect(410, 70, 89, 25));
        request_quota_push_button = new QPushButton(centralWidget);
        request_quota_push_button->setObjectName(QStringLiteral("request_quota_push_button"));
        request_quota_push_button->setGeometry(QRect(150, 70, 121, 25));
        Peer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Peer);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 658, 19));
        Peer->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Peer);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Peer->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Peer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Peer->setStatusBar(statusBar);

        retranslateUi(Peer);

        QMetaObject::connectSlotsByName(Peer);
    } // setupUi

    void retranslateUi(QMainWindow *Peer)
    {
        Peer->setWindowTitle(QApplication::translate("Peer", "Peer", 0));
        local_port_line_edit->setPlaceholderText(QApplication::translate("Peer", "Local Port", 0));
        server_ip_line_edit->setPlaceholderText(QApplication::translate("Peer", "Server IP", 0));
        server_port_line_edit->setPlaceholderText(QApplication::translate("Peer", "Server Port", 0));
        username_line_edit->setPlaceholderText(QApplication::translate("Peer", "Username", 0));
        password_line_edit->setPlaceholderText(QApplication::translate("Peer", "Password", 0));
        sign_up_push_button->setText(QApplication::translate("Peer", "Sign Up", 0));
        sign_out_push_button->setText(QApplication::translate("Peer", "Sign Out", 0));
        sign_in_push_button->setText(QApplication::translate("Peer", "Sign In", 0));
        bind_push_button->setText(QApplication::translate("Peer", "Bind", 0));
        send_push_button->setText(QApplication::translate("Peer", "Send", 0));
        receiver_ip_line_edit->setPlaceholderText(QApplication::translate("Peer", "Receiver IP", 0));
        receiver_port_line_edit->setPlaceholderText(QApplication::translate("Peer", "Receiver Port", 0));
        image_id_line_edit->setPlaceholderText(QApplication::translate("Peer", "Image Id", 0));
        image_label->setText(QString());
        show_image_push_button->setText(QApplication::translate("Peer", "click", 0));
        get_online_push_button->setText(QApplication::translate("Peer", "Get Online", 0));
        request_quota_push_button->setText(QApplication::translate("Peer", "Request Quota", 0));
    } // retranslateUi

};

namespace Ui {
    class Peer: public Ui_Peer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PEER_H
