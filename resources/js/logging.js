// ---------------------------------------------------------------------------------------------------------------------
// Logging wrapper module.
//
// This module wraps the C++ logging class, allowing Javascript/QML to use a more familiar syntax, instead of building a
// list to pass the arguments in.
//
// @module logging.js
// ---------------------------------------------------------------------------------------------------------------------

Qt.include("./vendor/lodash.js");

function Logger(name)
{
    this.logger = logMan.logger(name);
} // end Logger

Logger.prototype = {
    get name()
    {
        return this.logger.name;
    },
    set name(val)
    {
        this.logger.name = val;
    }
} // end prototype

Logger.prototype.debug = function(message)
{
    this.logger.debug(message, _.rest(arguments));
} // end debug

Logger.prototype.info = function(message)
{
    this.logger.info(message, _.rest(arguments));
} // end info

Logger.prototype.notice = function(message)
{
    this.logger.notice(message, _.rest(arguments));
} // end notice

Logger.prototype.notify = function(message)
{
    this.logger.notify(message, _.rest(arguments));
} // end notify

Logger.prototype.warning = function(message)
{
    this.logger.warning(message, _.rest(arguments));
} // end warning

Logger.prototype.warn = function(message)
{
    this.logger.warn(message, _.rest(arguments));
} // end warn

Logger.prototype.error = function(message)
{
    this.logger.error(message, _.rest(arguments));
} // end error

Logger.prototype.critical = function(message)
{
    this.logger.critical(message, _.rest(arguments));
} // end critical

Logger.prototype.fatal = function(message)
{
    this.logger.fatal(message, _.rest(arguments));
} // end fatal

// ---------------------------------------------------------------------------------------------------------------------
