#include <QDir>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QTextStream>

#include <QDebug>

#include "psettings.h"

/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

/**
 * @brief The default constructor.
 * @param parent The parent QObject.
 */
PSettingsManager::PSettingsManager(QObject *parent) :
	QObject(parent)
{
	// Get the writable location for config files
	settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);

	// We always work in our own directory
	settingsPath += "/skewedaspect";

	// Make sure the path exists
	if(!QDir().mkpath(settingsPath))
	{
		qCritical("Failed to make settings directory.");
	} // end if

	// load our files initially
	reload();
} // end PSettingsManager

/**
 * @brief Get the single instance of the settings manager.
 * @return Returns the static instance of the settings manager.
 */
PSettingsManager& PSettingsManager::instance()
{
	static PSettingsManager _instance;
	return _instance;
} // end instance

void PSettingsManager::reload()
{
	bool updated = false;

	QStringList settingsFiles = QStandardPaths::locateAll(QStandardPaths::ConfigLocation,
			"skewedaspect/precursors.config.json");

	// Apply all files to our settings.
	for (int i = 0; i < settingsFiles.size(); ++i)
	{
		QFile file(settingsFiles[i]);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			break;
		} // end if

		// Setup a text stream
		QTextStream in(&file);

		// Get a QVariantMap of our settings
		QVariantMap settings = QJsonDocument::fromJson(in.readAll().toUtf8()).toVariant().toMap();

		// Iterate over all settings, and only if the key is not set do we apply it. (This is because our files are
		// sorted higest priority to lowest.
		QMapIterator<QString, QVariant> settingsIt(settings);
		while (settingsIt.hasNext())
		{
			settingsIt.next();
			if(!this->_settings.contains(settingsIt.key()))
			{
				this->_settings[settingsIt.key()] = settingsIt.value();
				updated = true;
			} // end if
		} // end while
	} // end for

	// If we updated our settings, then we fire the `loaded` signal.
	if(updated)
	{
		emit loaded();
	} // end if
} // end reload

QVariant PSettingsManager::get(QString key, QVariant defaultValue)
{
	return this->_settings.value(key, defaultValue);
} // end get

void PSettingsManager::set(QString key, QVariant value)
{
	this->_settings[key] = value;
} // end set

void PSettingsManager::del(QString key)
{
	this->_settings.remove(key);
} // end set

void PSettingsManager::save()
{
	// Convert our settings map into a json string
	QByteArray configJson = QJsonDocument::fromVariant(this ->_settings).toJson();

	QFile settingsFile(this->settingsPath + "/precursors.config.json");
	if (!settingsFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qCritical("Unable to save settings file!");
	} // end if

	settingsFile.write(configJson);
} // end save
