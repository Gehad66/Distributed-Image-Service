#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <set>
#include <future>
#include <mutex>

#include <QMainWindow>
#include <QLineEdit>

#include "user.h"
#include "../socket.h"
#include "../message.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_bind_push_button_clicked();

    void on_start_push_button_clicked();

    void on_exit_push_button_clicked();

private:
    const static std::string USERS_FILE_NAME;
    // Visibility
    void set_widgets_visibility(const vector<QWidget*> _widgets, const bool _visibility);
    void set_logs_widgets_visibility(const bool _visibility);
    void set_bind_widgets_visibility(const bool _visibility);
    void set_start_widgets_visibility(const bool _visibility);
    void set_exit_widgets_visibility(const bool _visibility);

    // Auxiliary functions!
    static string get_string_from_line_edit(const QLineEdit * _text_edit, const bool _strict = true);
    void show_message_box(const string & _message_content, const string & _title = "Information");
    void log(const string & _log_message);
    void chunk_message_and_send(const string& photo_in_string, const string & user_ip, const int & user_port);
    string read_text_file(const string& file_name); //read text file and convert it to string, used in ViewRegistry process

    // Serve
    void listen();
    void save_users();
    void read_users();
    std::future<void> listening_thread;
    std::future<void> saving_user_DB;
    std::map<std::string, User> users;
    std::mutex users_mutex;

    // GUI
    Ui::MainWindow *ui;

    // My socket!
    Socket * socket = NULL;
};

#endif // MAINWINDOW_H
