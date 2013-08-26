#include <cmath>
#include <limits>

#include "math3d.h"


QQuaternion Math3D::eulerToQuat(qreal heading, qreal pitch, qreal roll)
{
    qreal hy = heading / 2;
    qreal hp = pitch / 2;
    qreal hr = roll / 2;

    return QQuaternion(
            cos(hp) * cos(hr) * cos(hy) + sin(hp) * sin(hr) * sin(hy),
            sin(hp) * cos(hr) * cos(hy) - cos(hp) * sin(hr) * sin(hy),
            cos(hp) * sin(hr) * cos(hy) + sin(hp) * cos(hr) * sin(hy),
            cos(hp) * cos(hr) * sin(hy) - sin(hp) * sin(hr) * cos(hy)
            );
} // end eulerToQuat

qreal Math3D::quatToHeading(const QQuaternion& quat)
{
    qreal halfSinePitch = quat.x() * quat.y() + quat.z() * quat.scalar();

    if(halfSinePitch == 0.5) // north pole
    {
        return 2 * atan2(quat.x(), quat.scalar());
    }
    else if(halfSinePitch == -0.5) // south pole
    {
        return -2 * atan2(quat.x(), quat.scalar());
    }
    else
    {
        return atan2(2 * quat.y() * quat.scalar() - 2 * quat.x() * quat.z(),
                1 - 2 * pow(quat.y(), 2) - 2 * pow(quat.z(), 2));
    } // end if
} // end quatToHeading

qreal Math3D::quatToPitch(const QQuaternion& quat)
{
    return asin(2 * quat.x() * quat.y() + 2 * quat.z() * quat.scalar());
} // end quatToPitch

qreal Math3D::quatToRoll(const QQuaternion& quat)
{
    qreal halfSinePitch = quat.x() * quat.y() + quat.z() * quat.scalar();

    if(halfSinePitch == 0.5) // north pole
    {
        return 0;
    }
    else if(halfSinePitch == -0.5) // south pole
    {
        return 0;
    }
    else
    {
        return atan2(2 * quat.x() * quat.scalar() - 2 * quat.y() * quat.z(),
                1 - 2 * pow(quat.x(), 2) - 2 * pow(quat.z(), 2));
    } // end if
} // end quatToRoll

template<typename T> void Math3D::quatToHPR(const QQuaternion& quat, T* heading, T* pitch, T* roll)
{
	qreal scalarSquared = quat.scalar() * quat.scalar();
	qreal xSquared = quat.x() * quat.x();
	qreal ySquared = quat.y() * quat.y();
	qreal zSquared = quat.z() * quat.z();

	// If quaternion is normalised the unit is one, otherwise it is the correction factor
	qreal unit = scalarSquared + xSquared + ySquared + zSquared;

    qreal halfSinePitch = quat.x() * quat.z() - quat.y() * quat.scalar();

	*pitch = asin(2 * halfSinePitch);

    if(halfSinePitch + std::numeric_limits<double>::epsilon() >= 0.5 * unit) // north pole
    {
        *heading = 2 * atan2(quat.y(), quat.scalar());

		*roll = 0;
    }
    else if(halfSinePitch - std::numeric_limits<double>::epsilon() <= -0.5 * unit) // south pole
    {
        *heading = -2 * atan2(quat.y(), quat.scalar());

		*roll = 0;
    }
    else
    {
		*heading = atan2(2 * (quat.x() * quat.scalar() + quat.y() * quat.z()), 1 - 2 * (zSquared + scalarSquared));

		*roll = atan2(2 * (quat.x() * quat.y() + quat.z() * quat.scalar()), 1 - 2 * (ySquared + zSquared));
    } // end if
} // end quatToHPR

// Explicit template instantiations
template void Math3D::quatToHPR<float>(const QQuaternion& quat, float* heading, float* pitch, float* roll);
template void Math3D::quatToHPR<qreal>(const QQuaternion& quat, qreal* heading, qreal* pitch, qreal* roll);


qreal Math3D::matrixToHeading(const QMatrix4x4& mat)
{
    if(mat(1, 2) > 0.9999999998 || mat(1, 2) < -0.9999999998)
    {
        return atan2(-mat(2, 0), mat(0, 0));
    }
    else
    {
        return atan2(mat(0, 2), mat(2, 2));
    } // end if
} // end eulerToQuat

qreal Math3D::matrixToPitch(const QMatrix4x4& mat)
{
    return asin(mat(1, 2));
} // end eulerToQuat

qreal Math3D::matrixToRoll(const QMatrix4x4& mat)
{
    if(mat(1, 2) > 0.9999999998 || mat(1, 2) < -0.9999999998)
    {
        return 0;
    }
    else
    {
        return atan2(mat(1, 0), mat(1, 1));
    } // end if
} // end eulerToQuat

void Math3D::matrixToEuler(const QMatrix4x4& mat, qreal *heading, qreal *pitch, qreal *roll)
{
    if(mat(1, 2) > 0.9999999998 || mat(1, 2) < -0.9999999998)
    {
        *heading = atan2(-mat(2, 0), mat(0, 0));
        *roll = 0;
    }
    else
    {
        *heading = atan2(mat(0, 2), mat(2, 2));
        *roll = atan2(mat(1, 0), mat(1, 1));
    } // end if

    *pitch = asin(mat(1, 2));
} // end eulerToQuat


QQuaternion Math3D::quatNormalized(const QQuaternion& quat)
{
	return quat.normalized();
} // end quatNormalized

QQuaternion Math3D::quatConjugate(const QQuaternion& quat)
{
    return quat.conjugate();
} // end quatConjugate

QQuaternion Math3D::quatReciprocal(const QQuaternion& quat)
{
    return quat.conjugate() / quat.lengthSquared();
} // end quatReciprocal

QQuaternion Math3D::quatLog(const QQuaternion& quat)
{
    float norm = quat.length();
    QVector3D vec = quat.vector();
    return QQuaternion(log(norm), ((vec / vec.length()) * acos(quat.scalar() / norm)));
} // end quatLog


QQuaternion Math3D::quatMult(const QQuaternion& q1, const QQuaternion& q2)
{
    return q1 * q2;
} // end quatMult

QQuaternion Math3D::quatMult(const QQuaternion& q1, const float q2)
{
    return q1 * q2;
} // end quatMult

QQuaternion Math3D::quatMult(const float q1, const QQuaternion& q2)
{
    return q1 * q2;
} // end quatMult

QQuaternion Math3D::quatSubtract(const QQuaternion& q1, const QQuaternion& q2)
{
    return q1 - q2;
} // end quatSubtract


QQuaternion Math3D::nlerp(const QQuaternion& q1, const QQuaternion& q2, float t)
{
	return QQuaternion::nlerp(q1, q2, t);
} // end nlerp

QQuaternion Math3D::slerp(const QQuaternion& q1, const QQuaternion& q2, float t)
{
	return QQuaternion::slerp(q1, q2, t);
} // end slerp
