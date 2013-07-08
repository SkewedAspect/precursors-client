#include <QCoreApplication>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "preutil.h"

PreUtil::PreUtil() : QObject(), logger(PLogManager::getLogger("preutil")) {} // end PreUtil

QString PreUtil::getClientVersion()
{
    return QCoreApplication::instance()->applicationVersion();
} // end getClientVersion

/**
 * @brief Get the single instance of the utility class.
 * @return Returns the static instance of the utility class.
 */
PreUtil& PreUtil::instance()
{
    static PreUtil _instance;
    return _instance;
} // end instance


QList<QVariant> PreUtil::getClientVersionAsList()
{
    // Get our version from the application
    QString versionString = QCoreApplication::instance()->applicationVersion();
    QRegularExpression versionRe = QRegularExpression("(?<major>\\d)\\.(?<minor>\\d)\\.(?<micro>\\d)(?:-(?<tag>\\w+))?(?: (?<revision>.*))?");
    QRegularExpressionMatch matches = versionRe.match(versionString);
    QList<QVariant> version;

    if(matches.hasMatch())
    {
        version.append(matches.captured("major").toInt());
        version.append(matches.captured("minor").toInt());
        version.append(matches.captured("micro").toInt());
        version.append(matches.captured("tag"));
    }
    else
    {
        logger.warning("Failed to parse application version number.");
        version << 0 << 0 << 0 << "unknown_version";
    } // end if

    return version;
} // end getClientVersionAsList
