#ifndef MATH3D_H
#define MATH3D_H

#include <QtGui/QMatrix4x4>
#include <QtGui/QQuaternion>
#include <QtGui/QVector3D>


class Math3D : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE static QQuaternion eulerToQuat(qreal heading, qreal pitch, qreal roll);
	Q_INVOKABLE static QList<qreal> quatToHPR(const QQuaternion& quat);
	template<typename T> static void quatToHPR(const QQuaternion& quat, T* heading, T* pitch, T* roll);

	Q_INVOKABLE static float quatScalar(const QQuaternion& quat);
	Q_INVOKABLE static float quatX(const QQuaternion& quat);
	Q_INVOKABLE static float quatY(const QQuaternion& quat);
	Q_INVOKABLE static float quatZ(const QQuaternion& quat);

	Q_INVOKABLE static qreal matrixToHeading(const QMatrix4x4& mat);
	Q_INVOKABLE static qreal matrixToPitch(const QMatrix4x4& mat);
	Q_INVOKABLE static qreal matrixToRoll(const QMatrix4x4& mat);
	static void matrixToEuler(const QMatrix4x4& mat, qreal *heading, qreal *pitch, qreal *roll);

	Q_INVOKABLE static QQuaternion quatFromScalarAndVector(float scalar, const QVector3D& vector);
	Q_INVOKABLE static QVector3D quatRotatedVector(const QQuaternion& quat, const QVector3D& vector);
	Q_INVOKABLE static QVector3D quatVector(const QQuaternion& quat);

	Q_INVOKABLE static QQuaternion quatNormalized(const QQuaternion& quat);
	Q_INVOKABLE static QQuaternion quatConjugate(const QQuaternion& quat);
	Q_INVOKABLE static QQuaternion quatReciprocal(const QQuaternion& quat);
	Q_INVOKABLE static QQuaternion quatLog(const QQuaternion& quat);

	Q_INVOKABLE static QQuaternion quatMult(const QQuaternion& q1, const QQuaternion& q2);
	Q_INVOKABLE static QQuaternion quatMult(const QQuaternion& q1, const float q2);
	Q_INVOKABLE static QQuaternion quatMult(const float q1, const QQuaternion& q2);
	Q_INVOKABLE static QQuaternion quatSubtract(const QQuaternion& q1, const QQuaternion& q2);

	Q_INVOKABLE static QQuaternion nlerp(const QQuaternion& q1, const QQuaternion& q2, float t);
	Q_INVOKABLE static QQuaternion slerp(const QQuaternion& q1, const QQuaternion& q2, float t);

	Q_INVOKABLE static float vectorLength(const QVector3D& vec);
}; // end Math3D

#endif // MATH3D_H
