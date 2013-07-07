#ifndef MATH3D_H
#define MATH3D_H

#include <QtGui/QMatrix4x4>
#include <QtGui/QQuaternion>
#include <QtGui/QVector3D>


QQuaternion eulerToQuat(qreal heading, qreal pitch, qreal roll);
qreal quatToHeading(QQuaternion quat);
qreal quatToPitch(QQuaternion quat);
qreal quatToRoll(QQuaternion quat);

void matrixToEuler(QMatrix4x4 mat, qreal *heading, qreal *pitch, qreal *roll);
qreal matrixToHeading(QMatrix4x4 mat);
qreal matrixToPitch(QMatrix4x4 mat);
qreal matrixToRoll(QMatrix4x4 mat);

#endif // MATH3D_H
