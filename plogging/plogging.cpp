#include <cstdio>


#include <QDir>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QTextStream>
#include <QTime>

#include <QDebug>

#include "plogging.h"

/**********************************************************************************************************************/
/* PLogManager Public API                                                                                                         */
/**********************************************************************************************************************/

PLogManager::PLogManager(QObject* parent) :
	QObject()
{
} // end PLogManager

PLogManager& PLogManager::instance()
{
	static PLogManager _instance;
	return _instance;
} // end instance

PLogger* PLogManager::logger(QString name)
{
	if(!_loggers.contains(name))
	{
		_loggers[name] = new PLogger(name);
	} // end if

	return _loggers[name];
} // end getLogger

PLogger& PLogManager::getLogger(QString name)
{
	return *PLogManager::instance().logger(name);
} // end getLogger


/**********************************************************************************************************************/
/* PLogger Public API                                                                                                         */
/**********************************************************************************************************************/

/**
 * @brief The default constructor.
 * @param parent The parent QObject.
 */
PLogger::PLogger(QString name) :
	QObject()
{
	this->name = name;
} // end PLogger


void PLogger::debug(QString message)
{
	log("DEBUG", message);
} // end debug

void PLogger::debug(QString message, QVariantList args)
{
	for (int i = 0; i < args.size(); ++i)
	{
		message = message.arg(args.at(i).toString());
	} // end for

	debug(message);
} // end debug

void PLogger::info(QString message)
{
	log("INFO", message);
} // end info

void PLogger::info(QString message, QVariantList args)
{
	for (int i = 0; i < args.size(); ++i)
	{
		message = message.arg(args.at(i).toString());
	} // end for

	info(message);
} // end info

void PLogger::notice(QString message)
{
	log("NOTICE", message);
} // end notice

void PLogger::notice(QString message, QVariantList args)
{
	for (int i = 0; i < args.size(); ++i)
	{
		message = message.arg(args.at(i).toString());
	} // end for

	notice(message);
} // end notice

void PLogger::notify(QString message)
{
	notice(message);
} // end notify

void PLogger::notify(QString message, QVariantList args)
{
	notice(message, args);
} // end notify

void PLogger::warning(QString message)
{
	log("WARNING", message);
} // end warning

void PLogger::warning(QString message, QVariantList args)
{
	for (int i = 0; i < args.size(); ++i)
	{
		message = message.arg(args.at(i).toString());
	} // end for

	warning(message);
} // end warning

void PLogger::warn(QString message)
{
	warning(message);
} // end warn

void PLogger::warn(QString message, QVariantList args)
{
	warning(message, args);
} // end warn

void PLogger::error(QString message)
{
	log("ERROR", message);
} // end error

void PLogger::error(QString message, QVariantList args)
{
	for (int i = 0; i < args.size(); ++i)
	{
		message = message.arg(args.at(i).toString());
	} // end for

	error(message);
} // end error

void PLogger::critical(QString message)
{
	log("CRITICAL", message);
} // end critical

void PLogger::critical(QString message, QVariantList args)
{
	for (int i = 0; i < args.size(); ++i)
	{
		message = message.arg(args.at(i).toString());
	} // end for

	critical(message);
} // end critical

void PLogger::fatal(QString message)
{
	log("FATAL", message);
	exit(EXIT_FAILURE);
} // end fatal

void PLogger::fatal(QString message, QVariantList args)
{
	for (int i = 0; i < args.size(); ++i)
	{
		message = message.arg(args.at(i).toString());
	} // end for

	fatal(message);
} // end fatal

/**********************************************************************************************************************/

QString PLogger::buildString(QString level, QString message)
{
	// Formats the log message into the format we want to use.
	return QString("%1 [%2] %3: %4").arg(QTime::currentTime().toString("h:mm:ss")).arg(level).arg(this->name).arg(message);
} // end buildString

void PLogger::log(QString level, QString message)
{
	message = buildString(level, message);
	const char* cMessage = message.toLatin1().constData();

	if(level == "ERROR" || level == "CRITICAL" || level == "FATAL")
	{

		fprintf(stderr, "%s\n", cMessage);
	}
	else
	{
		fprintf(stdout, "%s\n", cMessage);
	} // end if
} // end buildLogMessage

