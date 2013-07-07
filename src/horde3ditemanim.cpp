#include "horde3ditemanim.h"
#include "horde3ditem.h"


Horde3DItemAnimation::Horde3DItemAnimation(Horde3DItem* parent) :
	QAbstractAnimation(parent), m_item(parent)
{
	// Do nothing.
}

Horde3DItemAnimation::~Horde3DItemAnimation()
{
	// Do nothing.
}

int Horde3DItemAnimation::duration() const
{
	// Duration is undefined (infinite).
	return -1;
}

void Horde3DItemAnimation::updateCurrentTime(int currentTime)
{
	// Schedule redraw.
	m_item->update();
}
