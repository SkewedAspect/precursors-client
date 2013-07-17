#include <cmath>
#include <limits>

#include "math3d.h"


QQuaternion eulerToQuat(qreal heading, qreal pitch, qreal roll)
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

template<typename T> void quatToHPR(QQuaternion quat, T* heading, T* pitch, T* roll)
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
} // end quatToRoll

// Explicit template instantiations
template void quatToHPR<float>(QQuaternion quat, float* heading, float* pitch, float* roll);
template void quatToHPR<qreal>(QQuaternion quat, qreal* heading, qreal* pitch, qreal* roll);

qreal quatToHeading(QQuaternion quat)
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

qreal quatToPitch(QQuaternion quat)
{
    return asin(2 * quat.x() * quat.y() + 2 * quat.z() * quat.scalar());
} // end quatToPitch

qreal quatToRoll(QQuaternion quat)
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

qreal matrixToHeading(QMatrix4x4 mat)
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

qreal matrixToPitch(QMatrix4x4 mat)
{
    return asin(mat(1, 2));
} // end eulerToQuat

qreal matrixToRoll(QMatrix4x4 mat)
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

void matrixToEuler(QMatrix4x4 mat, qreal *heading, qreal *pitch, qreal *roll)
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
