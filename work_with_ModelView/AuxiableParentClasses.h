#ifndef AUXIABLEPARENTCLASSES_H
#define AUXIABLEPARENTCLASSES_H

#include <QTableView>
#include <QLineEdit>

namespace auxProperties
{
    class ParentTableView : public QTableView
    {
        Q_OBJECT

    public:
        ParentTableView(QWidget *parent = nullptr);

    protected:
        virtual void mousePressEvent(QMouseEvent *ev) override;

    signals:
        void MouseButtonPressed();
    };
}


#endif // AUXIABLEPARENTCLASSES_H
