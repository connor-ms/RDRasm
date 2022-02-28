#ifndef LAUNCHSCREEN_H
#define LAUNCHSCREEN_H

#include <QMainWindow>

namespace Ui {
class LaunchScreen;
}

class LaunchScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit LaunchScreen(QWidget *parent = nullptr);
    ~LaunchScreen();

private slots:
    void openFilePressed();
    void githubPressed();
    void exitPressed();

private:
    Ui::LaunchScreen *m_ui;
};

#endif // LAUNCHSCREEN_H
