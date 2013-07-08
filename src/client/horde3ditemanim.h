#ifndef HORDE3DITEMANIM_H
#define HORDE3DITEMANIM_H

#include <QtCore/QAbstractAnimation>


class Horde3DItem;


class Horde3DItemAnimation : public QAbstractAnimation
{
    Q_OBJECT

public:
    Horde3DItemAnimation(Horde3DItem* parent = 0);
    ~Horde3DItemAnimation();

    int duration() const;

protected:
    void updateCurrentTime(int currentTime);

private:
    Horde3DItem* m_item;
}; // end Horde3DItemAnimation

#endif // HORDE3DITEM_H
