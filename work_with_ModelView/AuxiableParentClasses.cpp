#include "AuxiableParentClasses.h"

/*ParentTableView*/
auxProperties::ParentTableView::ParentTableView(QWidget *parent) : QTableView(parent)
{

}

void auxProperties::ParentTableView::mousePressEvent(QMouseEvent *ev)
{
    QTableView::mousePressEvent(ev);

    emit MouseButtonPressed();
}
