// ---------------------------------------------------------------------------------------------------------------------
// Logging wrapper module.
//
// This module wraps the C++ logging class, allowing Javascript/QML to use a more familiar syntax, instead of building a
// list to pass the arguments in.
//
// @module logging.js
// ---------------------------------------------------------------------------------------------------------------------

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

Logger.prototype.debug = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.debug(message, args);
} // end debug

Logger.prototype.info = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.info(message, args);
} // end info

Logger.prototype.notice = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.notice(message, args);
} // end notice

Logger.prototype.notify = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.notify(message, args);
} // end notify

Logger.prototype.warning = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.warning(message, args);
} // end warning

Logger.prototype.warn = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.warn(message, args);
} // end warn

Logger.prototype.error = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.error(message, args);
} // end error

Logger.prototype.critical = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.critical(message, args);
} // end critical

Logger.prototype.fatal = function()
{
    var message = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);

    this.logger.fatal(message, args);
} // end fatal

// ---------------------------------------------------------------------------------------------------------------------
