#ifndef ADMIN_MANAGER_LESSON_FINDER_H
#define ADMIN_MANAGER_LESSON_FINDER_H

#include <QWidget>

namespace Ui {
class admin_manager_lesson_finder;
}

class admin_manager_lesson_finder : public QWidget
{
    Q_OBJECT

public:
    explicit admin_manager_lesson_finder(QWidget *parent = 0);
    ~admin_manager_lesson_finder();

private:
    Ui::admin_manager_lesson_finder *ui;
};

#endif // ADMIN_MANAGER_LESSON_FINDER_H
