#ifndef LAUNCHSCREEN_H
#define LAUNCHSCREEN_H

#include <QWidget>

namespace Ui {
class LaunchScreen;
}

class LaunchScreen : public QWidget
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
    Ui::LaunchScreen *m_Ui;
};

#endif // LAUNCHSCREEN_H
