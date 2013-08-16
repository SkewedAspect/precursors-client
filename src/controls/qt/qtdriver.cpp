#include <cmath>

#include <QMouseEvent>
#include <QScreen>
#include <QWindow>

#include "controls/devices/genericdevice.h"
#include "controls/signals/axisinputsignal.h"
#include "controls/signals/buttoninputsignal.h"
#include "qtdriver.h"


QtDriver::QtDriver() :
		InputDriver(),
		_keyboardDevice(NULL),
		_mouseDevice(NULL),
		_logger(PLogManager::getLogger("QtDriver")),
		_settings(PSettingsManager::instance())
{
} // end QtDriver

QString QtDriver::name()
{
	return "Qt Input Driver";
} // end name

void QtDriver::setWindow(QWindow* window)
{
	_logger.debug("setWindow");

	if(!_keyboardDevice)
	{
		_keyboardDevice = new GenericDevice(this, InputDevice::KEYBOARD, "Keyboard");

#define KEY(KEYNAME, NAME, DESC)	_keyboardButtons[Qt::KEYNAME] = new ButtonInputSignal(_keyboardDevice, NAME, DESC)
		KEY(Key_Escape, "Escape", "The Escape key");
		KEY(Key_Tab, "Tab", "The Tab key");
		KEY(Key_Backtab, "Backtab", "The Backtab key");
		KEY(Key_Backspace, "Backspace", "The Backspace key");
		KEY(Key_Return, "Return", "The Return key");
		KEY(Key_Enter, "Enter", "Typically located on the keypad.");
		KEY(Key_Insert, "Insert", "The Insert key");
		KEY(Key_Delete, "Delete", "The Delete key");
		KEY(Key_Pause, "Pause", "The Pause/Break key (Note: Not anything to do with pausing media)");
		KEY(Key_Print, "Print", "The Print key");
		KEY(Key_SysReq, "SysReq", "The SysReq key");
		KEY(Key_Clear, "Clear", "The Clear key");
		KEY(Key_Home, "Home", "The Home key");
		KEY(Key_End, "End", "The End key");
		KEY(Key_Left, "Left", "The Left key");
		KEY(Key_Up, "Up", "The Up key");
		KEY(Key_Right, "Right", "The Right key");
		KEY(Key_Down, "Down", "The Down key");
		KEY(Key_PageUp, "PageUp", "The PageUp key");
		KEY(Key_PageDown, "PageDown", "The PageDown key");
		KEY(Key_Shift, "Shift", "The Shift key");
		KEY(Key_Control, "Control", "On Mac OS X, this corresponds to the Command keys.");
		KEY(Key_Meta, "Meta", "On Mac OS X, this corresponds to the Control keys. On Windows keyboards, this key is mapped to the Windows key.");
		KEY(Key_Alt, "Alt", "The Alt key");
		KEY(Key_AltGr, "AltGr", "On Windows, when the KeyDown event for this key is sent, the Ctrl+Alt modifiers are also set.");
		KEY(Key_CapsLock, "CapsLock", "The CapsLock key");
		KEY(Key_NumLock, "NumLock", "The NumLock key");
		KEY(Key_ScrollLock, "ScrollLock", "The ScrollLock key");
		KEY(Key_F1, "F1", "The F1 key");
		KEY(Key_F2, "F2", "The F2 key");
		KEY(Key_F3, "F3", "The F3 key");
		KEY(Key_F4, "F4", "The F4 key");
		KEY(Key_F5, "F5", "The F5 key");
		KEY(Key_F6, "F6", "The F6 key");
		KEY(Key_F7, "F7", "The F7 key");
		KEY(Key_F8, "F8", "The F8 key");
		KEY(Key_F9, "F9", "The F9 key");
		KEY(Key_F10, "F10", "The F10 key");
		KEY(Key_F11, "F11", "The F11 key");
		KEY(Key_F12, "F12", "The F12 key");
		KEY(Key_F13, "F13", "The F13 key");
		KEY(Key_F14, "F14", "The F14 key");
		KEY(Key_F15, "F15", "The F15 key");
		KEY(Key_F16, "F16", "The F16 key");
		KEY(Key_F17, "F17", "The F17 key");
		KEY(Key_F18, "F18", "The F18 key");
		KEY(Key_F19, "F19", "The F19 key");
		KEY(Key_F20, "F20", "The F20 key");
		KEY(Key_F21, "F21", "The F21 key");
		KEY(Key_F22, "F22", "The F22 key");
		KEY(Key_F23, "F23", "The F23 key");
		KEY(Key_F24, "F24", "The F24 key");
		KEY(Key_F25, "F25", "The F25 key");
		KEY(Key_F26, "F26", "The F26 key");
		KEY(Key_F27, "F27", "The F27 key");
		KEY(Key_F28, "F28", "The F28 key");
		KEY(Key_F29, "F29", "The F29 key");
		KEY(Key_F30, "F30", "The F30 key");
		KEY(Key_F31, "F31", "The F31 key");
		KEY(Key_F32, "F32", "The F32 key");
		KEY(Key_F33, "F33", "The F33 key");
		KEY(Key_F34, "F34", "The F34 key");
		KEY(Key_F35, "F35", "The F35 key");
		KEY(Key_Super_L, "Super_L", "The Super_L key");
		KEY(Key_Super_R, "Super_R", "The Super_R key");
		KEY(Key_Menu, "Menu", "The Menu key");
		KEY(Key_Hyper_L, "Hyper_L", "The Hyper_L key");
		KEY(Key_Hyper_R, "Hyper_R", "The Hyper_R key");
		KEY(Key_Help, "Help", "The Help key");
		KEY(Key_Direction_L, "Direction_L", "The Direction_L key");
		KEY(Key_Direction_R, "Direction_R", "The Direction_R key");
		KEY(Key_Space, "Space", "The Space key");
		KEY(Key_Any, "Any", "The Any key");
		KEY(Key_Exclam, "Exclam", "The Exclam key");
		KEY(Key_QuoteDbl, "QuoteDbl", "The QuoteDbl key");
		KEY(Key_NumberSign, "NumberSign", "The NumberSign key");
		KEY(Key_Dollar, "Dollar", "The Dollar key");
		KEY(Key_Percent, "Percent", "The Percent key");
		KEY(Key_Ampersand, "Ampersand", "The Ampersand key");
		KEY(Key_Apostrophe, "Apostrophe", "The Apostrophe key");
		KEY(Key_ParenLeft, "ParenLeft", "The ParenLeft key");
		KEY(Key_ParenRight, "ParenRight", "The ParenRight key");
		KEY(Key_Asterisk, "Asterisk", "The Asterisk key");
		KEY(Key_Plus, "Plus", "The Plus key");
		KEY(Key_Comma, "Comma", "The Comma key");
		KEY(Key_Minus, "Minus", "The Minus key");
		KEY(Key_Period, "Period", "The Period key");
		KEY(Key_Slash, "Slash", "The Slash key");
		KEY(Key_0, "0", "The 0 key");
		KEY(Key_1, "1", "The 1 key");
		KEY(Key_2, "2", "The 2 key");
		KEY(Key_3, "3", "The 3 key");
		KEY(Key_4, "4", "The 4 key");
		KEY(Key_5, "5", "The 5 key");
		KEY(Key_6, "6", "The 6 key");
		KEY(Key_7, "7", "The 7 key");
		KEY(Key_8, "8", "The 8 key");
		KEY(Key_9, "9", "The 9 key");
		KEY(Key_Colon, "Colon", "The Colon key");
		KEY(Key_Semicolon, "Semicolon", "The Semicolon key");
		KEY(Key_Less, "Less", "The Less key");
		KEY(Key_Equal, "Equal", "The Equal key");
		KEY(Key_Greater, "Greater", "The Greater key");
		KEY(Key_Question, "Question", "The Question key");
		KEY(Key_At, "At", "The At key");
		KEY(Key_A, "A", "The A key");
		KEY(Key_B, "B", "The B key");
		KEY(Key_C, "C", "The C key");
		KEY(Key_D, "D", "The D key");
		KEY(Key_E, "E", "The E key");
		KEY(Key_F, "F", "The F key");
		KEY(Key_G, "G", "The G key");
		KEY(Key_H, "H", "The H key");
		KEY(Key_I, "I", "The I key");
		KEY(Key_J, "J", "The J key");
		KEY(Key_K, "K", "The K key");
		KEY(Key_L, "L", "The L key");
		KEY(Key_M, "M", "The M key");
		KEY(Key_N, "N", "The N key");
		KEY(Key_O, "O", "The O key");
		KEY(Key_P, "P", "The P key");
		KEY(Key_Q, "Q", "The Q key");
		KEY(Key_R, "R", "The R key");
		KEY(Key_S, "S", "The S key");
		KEY(Key_T, "T", "The T key");
		KEY(Key_U, "U", "The U key");
		KEY(Key_V, "V", "The V key");
		KEY(Key_W, "W", "The W key");
		KEY(Key_X, "X", "The X key");
		KEY(Key_Y, "Y", "The Y key");
		KEY(Key_Z, "Z", "The Z key");
		KEY(Key_BracketLeft, "BracketLeft", "The BracketLeft key");
		KEY(Key_Backslash, "Backslash", "The Backslash key");
		KEY(Key_BracketRight, "BracketRight", "The BracketRight key");
		KEY(Key_AsciiCircum, "AsciiCircum", "The AsciiCircum key");
		KEY(Key_Underscore, "Underscore", "The Underscore key");
		KEY(Key_QuoteLeft, "QuoteLeft", "The QuoteLeft key");
		KEY(Key_BraceLeft, "BraceLeft", "The BraceLeft key");
		KEY(Key_Bar, "Bar", "The Bar key");
		KEY(Key_BraceRight, "BraceRight", "The BraceRight key");
		KEY(Key_AsciiTilde, "AsciiTilde", "The AsciiTilde key");
		KEY(Key_nobreakspace, "nobreakspace", "The nobreakspace key");
		KEY(Key_exclamdown, "exclamdown", "The exclamdown key");
		KEY(Key_cent, "cent", "The cent key");
		KEY(Key_sterling, "sterling", "The sterling key");
		KEY(Key_currency, "currency", "The currency key");
		KEY(Key_yen, "yen", "The yen key");
		KEY(Key_brokenbar, "brokenbar", "The brokenbar key");
		KEY(Key_section, "section", "The section key");
		KEY(Key_diaeresis, "diaeresis", "The diaeresis key");
		KEY(Key_copyright, "copyright", "The copyright key");
		KEY(Key_ordfeminine, "ordfeminine", "The ordfeminine key");
		KEY(Key_guillemotleft, "guillemotleft", "The guillemotleft key");
		KEY(Key_notsign, "notsign", "The notsign key");
		KEY(Key_hyphen, "hyphen", "The hyphen key");
		KEY(Key_registered, "registered", "The registered key");
		KEY(Key_macron, "macron", "The macron key");
		KEY(Key_degree, "degree", "The degree key");
		KEY(Key_plusminus, "plusminus", "The plusminus key");
		KEY(Key_twosuperior, "twosuperior", "The twosuperior key");
		KEY(Key_threesuperior, "threesuperior", "The threesuperior key");
		KEY(Key_acute, "acute", "The acute key");
		KEY(Key_mu, "mu", "The mu key");
		KEY(Key_paragraph, "paragraph", "The paragraph key");
		KEY(Key_periodcentered, "periodcentered", "The periodcentered key");
		KEY(Key_cedilla, "cedilla", "The cedilla key");
		KEY(Key_onesuperior, "onesuperior", "The onesuperior key");
		KEY(Key_masculine, "masculine", "The masculine key");
		KEY(Key_guillemotright, "guillemotright", "The guillemotright key");
		KEY(Key_onequarter, "onequarter", "The onequarter key");
		KEY(Key_onehalf, "onehalf", "The onehalf key");
		KEY(Key_threequarters, "threequarters", "The threequarters key");
		KEY(Key_questiondown, "questiondown", "The questiondown key");
		KEY(Key_Agrave, "Agrave", "The Agrave key");
		KEY(Key_Aacute, "Aacute", "The Aacute key");
		KEY(Key_Acircumflex, "Acircumflex", "The Acircumflex key");
		KEY(Key_Atilde, "Atilde", "The Atilde key");
		KEY(Key_Adiaeresis, "Adiaeresis", "The Adiaeresis key");
		KEY(Key_Aring, "Aring", "The Aring key");
		KEY(Key_AE, "AE", "The AE key");
		KEY(Key_Ccedilla, "Ccedilla", "The Ccedilla key");
		KEY(Key_Egrave, "Egrave", "The Egrave key");
		KEY(Key_Eacute, "Eacute", "The Eacute key");
		KEY(Key_Ecircumflex, "Ecircumflex", "The Ecircumflex key");
		KEY(Key_Ediaeresis, "Ediaeresis", "The Ediaeresis key");
		KEY(Key_Igrave, "Igrave", "The Igrave key");
		KEY(Key_Iacute, "Iacute", "The Iacute key");
		KEY(Key_Icircumflex, "Icircumflex", "The Icircumflex key");
		KEY(Key_Idiaeresis, "Idiaeresis", "The Idiaeresis key");
		KEY(Key_ETH, "ETH", "The ETH key");
		KEY(Key_Ntilde, "Ntilde", "The Ntilde key");
		KEY(Key_Ograve, "Ograve", "The Ograve key");
		KEY(Key_Oacute, "Oacute", "The Oacute key");
		KEY(Key_Ocircumflex, "Ocircumflex", "The Ocircumflex key");
		KEY(Key_Otilde, "Otilde", "The Otilde key");
		KEY(Key_Odiaeresis, "Odiaeresis", "The Odiaeresis key");
		KEY(Key_multiply, "multiply", "The multiply key");
		KEY(Key_Ooblique, "Ooblique", "The Ooblique key");
		KEY(Key_Ugrave, "Ugrave", "The Ugrave key");
		KEY(Key_Uacute, "Uacute", "The Uacute key");
		KEY(Key_Ucircumflex, "Ucircumflex", "The Ucircumflex key");
		KEY(Key_Udiaeresis, "Udiaeresis", "The Udiaeresis key");
		KEY(Key_Yacute, "Yacute", "The Yacute key");
		KEY(Key_THORN, "THORN", "The THORN key");
		KEY(Key_ssharp, "ssharp", "The ssharp key");
		KEY(Key_division, "division", "The division key");
		KEY(Key_ydiaeresis, "ydiaeresis", "The ydiaeresis key");
		KEY(Key_Multi_key, "Multi_key", "The Multi_key key");
		KEY(Key_Codeinput, "Codeinput", "The Codeinput key");
		KEY(Key_SingleCandidate, "SingleCandidate", "The SingleCandidate key");
		KEY(Key_MultipleCandidate, "MultipleCandidate", "The MultipleCandidate key");
		KEY(Key_PreviousCandidate, "PreviousCandidate", "The PreviousCandidate key");
		KEY(Key_Mode_switch, "Mode_switch", "The Mode_switch key");
		KEY(Key_Kanji, "Kanji", "The Kanji key");
		KEY(Key_Muhenkan, "Muhenkan", "The Muhenkan key");
		KEY(Key_Henkan, "Henkan", "The Henkan key");
		KEY(Key_Romaji, "Romaji", "The Romaji key");
		KEY(Key_Hiragana, "Hiragana", "The Hiragana key");
		KEY(Key_Katakana, "Katakana", "The Katakana key");
		KEY(Key_Hiragana_Katakana, "Hiragana_Katakana", "The Hiragana_Katakana key");
		KEY(Key_Zenkaku, "Zenkaku", "The Zenkaku key");
		KEY(Key_Hankaku, "Hankaku", "The Hankaku key");
		KEY(Key_Zenkaku_Hankaku, "Zenkaku_Hankaku", "The Zenkaku_Hankaku key");
		KEY(Key_Touroku, "Touroku", "The Touroku key");
		KEY(Key_Massyo, "Massyo", "The Massyo key");
		KEY(Key_Kana_Lock, "Kana_Lock", "The Kana_Lock key");
		KEY(Key_Kana_Shift, "Kana_Shift", "The Kana_Shift key");
		KEY(Key_Eisu_Shift, "Eisu_Shift", "The Eisu_Shift key");
		KEY(Key_Eisu_toggle, "Eisu_toggle", "The Eisu_toggle key");
		KEY(Key_Hangul, "Hangul", "The Hangul key");
		KEY(Key_Hangul_Start, "Hangul_Start", "The Hangul_Start key");
		KEY(Key_Hangul_End, "Hangul_End", "The Hangul_End key");
		KEY(Key_Hangul_Hanja, "Hangul_Hanja", "The Hangul_Hanja key");
		KEY(Key_Hangul_Jamo, "Hangul_Jamo", "The Hangul_Jamo key");
		KEY(Key_Hangul_Romaja, "Hangul_Romaja", "The Hangul_Romaja key");
		KEY(Key_Hangul_Jeonja, "Hangul_Jeonja", "The Hangul_Jeonja key");
		KEY(Key_Hangul_Banja, "Hangul_Banja", "The Hangul_Banja key");
		KEY(Key_Hangul_PreHanja, "Hangul_PreHanja", "The Hangul_PreHanja key");
		KEY(Key_Hangul_PostHanja, "Hangul_PostHanja", "The Hangul_PostHanja key");
		KEY(Key_Hangul_Special, "Hangul_Special", "The Hangul_Special key");
		KEY(Key_Dead_Grave, "Dead_Grave", "The Dead_Grave key");
		KEY(Key_Dead_Acute, "Dead_Acute", "The Dead_Acute key");
		KEY(Key_Dead_Circumflex, "Dead_Circumflex", "The Dead_Circumflex key");
		KEY(Key_Dead_Tilde, "Dead_Tilde", "The Dead_Tilde key");
		KEY(Key_Dead_Macron, "Dead_Macron", "The Dead_Macron key");
		KEY(Key_Dead_Breve, "Dead_Breve", "The Dead_Breve key");
		KEY(Key_Dead_Abovedot, "Dead_Abovedot", "The Dead_Abovedot key");
		KEY(Key_Dead_Diaeresis, "Dead_Diaeresis", "The Dead_Diaeresis key");
		KEY(Key_Dead_Abovering, "Dead_Abovering", "The Dead_Abovering key");
		KEY(Key_Dead_Doubleacute, "Dead_Doubleacute", "The Dead_Doubleacute key");
		KEY(Key_Dead_Caron, "Dead_Caron", "The Dead_Caron key");
		KEY(Key_Dead_Cedilla, "Dead_Cedilla", "The Dead_Cedilla key");
		KEY(Key_Dead_Ogonek, "Dead_Ogonek", "The Dead_Ogonek key");
		KEY(Key_Dead_Iota, "Dead_Iota", "The Dead_Iota key");
		KEY(Key_Dead_Voiced_Sound, "Dead_Voiced_Sound", "The Dead_Voiced_Sound key");
		KEY(Key_Dead_Semivoiced_Sound, "Dead_Semivoiced_Sound", "The Dead_Semivoiced_Sound key");
		KEY(Key_Dead_Belowdot, "Dead_Belowdot", "The Dead_Belowdot key");
		KEY(Key_Dead_Hook, "Dead_Hook", "The Dead_Hook key");
		KEY(Key_Dead_Horn, "Dead_Horn", "The Dead_Horn key");
		KEY(Key_Back, "Back", "The Back key");
		KEY(Key_Forward, "Forward", "The Forward key");
		KEY(Key_Stop, "Stop", "The Stop key");
		KEY(Key_Refresh, "Refresh", "The Refresh key");
		KEY(Key_VolumeDown, "VolumeDown", "The VolumeDown key");
		KEY(Key_VolumeMute, "VolumeMute", "The VolumeMute key");
		KEY(Key_VolumeUp, "VolumeUp", "The VolumeUp key");
		KEY(Key_BassBoost, "BassBoost", "The BassBoost key");
		KEY(Key_BassUp, "BassUp", "The BassUp key");
		KEY(Key_BassDown, "BassDown", "The BassDown key");
		KEY(Key_TrebleUp, "TrebleUp", "The TrebleUp key");
		KEY(Key_TrebleDown, "TrebleDown", "The TrebleDown key");
		KEY(Key_MediaPlay, "MediaPlay", "A key setting the state of the media player to play");
		KEY(Key_MediaStop, "MediaStop", "A key setting the state of the media player to stop");
		KEY(Key_MediaPrevious, "MediaPrevious", "The MediaPrevious key");
		KEY(Key_MediaNext, "MediaNext", "The MediaNext key");
		KEY(Key_MediaRecord, "MediaRecord", "The MediaRecord key");
		KEY(Key_MediaPause, "MediaPause", "A key setting the state of the media player to pause (Note: not the pause/break key)");
		KEY(Key_MediaTogglePlayPause, "MediaTogglePlayPause", "A key to toggle the play/pause state in the media player (rather than setting an absolute state)");
		KEY(Key_HomePage, "HomePage", "The HomePage key");
		KEY(Key_Favorites, "Favorites", "The Favorites key");
		KEY(Key_Search, "Search", "The Search key");
		KEY(Key_Standby, "Standby", "The Standby key");
		KEY(Key_OpenUrl, "OpenUrl", "The OpenUrl key");
		KEY(Key_LaunchMail, "LaunchMail", "The LaunchMail key");
		KEY(Key_LaunchMedia, "LaunchMedia", "The LaunchMedia key");
		KEY(Key_Launch0, "Launch0", "On X11 this key is mapped to \"My Computer\" (XF86XK_MyComputer) key for legacy reasons.");
		KEY(Key_Launch1, "Launch1", "On X11 this key is mapped to \"Calculator\" (XF86XK_Calculator) key for legacy reasons.");
		KEY(Key_Launch2, "Launch2", "On X11 this key is mapped to XF86XK_Launch0 key for legacy reasons.");
		KEY(Key_Launch3, "Launch3", "On X11 this key is mapped to XF86XK_Launch1 key for legacy reasons.");
		KEY(Key_Launch4, "Launch4", "On X11 this key is mapped to XF86XK_Launch2 key for legacy reasons.");
		KEY(Key_Launch5, "Launch5", "On X11 this key is mapped to XF86XK_Launch3 key for legacy reasons.");
		KEY(Key_Launch6, "Launch6", "On X11 this key is mapped to XF86XK_Launch4 key for legacy reasons.");
		KEY(Key_Launch7, "Launch7", "On X11 this key is mapped to XF86XK_Launch5 key for legacy reasons.");
		KEY(Key_Launch8, "Launch8", "On X11 this key is mapped to XF86XK_Launch6 key for legacy reasons.");
		KEY(Key_Launch9, "Launch9", "On X11 this key is mapped to XF86XK_Launch7 key for legacy reasons.");
		KEY(Key_LaunchA, "LaunchA", "On X11 this key is mapped to XF86XK_Launch8 key for legacy reasons.");
		KEY(Key_LaunchB, "LaunchB", "On X11 this key is mapped to XF86XK_Launch9 key for legacy reasons.");
		KEY(Key_LaunchC, "LaunchC", "On X11 this key is mapped to XF86XK_LaunchA key for legacy reasons.");
		KEY(Key_LaunchD, "LaunchD", "On X11 this key is mapped to XF86XK_LaunchB key for legacy reasons.");
		KEY(Key_LaunchE, "LaunchE", "On X11 this key is mapped to XF86XK_LaunchC key for legacy reasons.");
		KEY(Key_LaunchF, "LaunchF", "On X11 this key is mapped to XF86XK_LaunchD key for legacy reasons.");
		KEY(Key_LaunchG, "LaunchG", "On X11 this key is mapped to XF86XK_LaunchE key for legacy reasons.");
		KEY(Key_LaunchH, "LaunchH", "On X11 this key is mapped to XF86XK_LaunchF key for legacy reasons.");
		KEY(Key_MonBrightnessUp, "MonBrightnessUp", "The MonBrightnessUp key");
		KEY(Key_MonBrightnessDown, "MonBrightnessDown", "The MonBrightnessDown key");
		KEY(Key_KeyboardLightOnOff, "KeyboardLightOnOff", "The KeyboardLightOnOff key");
		KEY(Key_KeyboardBrightnessUp, "KeyboardBrightnessUp", "The KeyboardBrightnessUp key");
		KEY(Key_KeyboardBrightnessDown, "KeyboardBrightnessDown", "The KeyboardBrightnessDown key");
		KEY(Key_PowerOff, "PowerOff", "The PowerOff key");
		KEY(Key_WakeUp, "WakeUp", "The WakeUp key");
		KEY(Key_Eject, "Eject", "The Eject key");
		KEY(Key_ScreenSaver, "ScreenSaver", "The ScreenSaver key");
		KEY(Key_WWW, "WWW", "The WWW key");
		KEY(Key_Memo, "Memo", "The Memo key");
		KEY(Key_LightBulb, "LightBulb", "The LightBulb key");
		KEY(Key_Shop, "Shop", "The Shop key");
		KEY(Key_History, "History", "The History key");
		KEY(Key_AddFavorite, "AddFavorite", "The AddFavorite key");
		KEY(Key_HotLinks, "HotLinks", "The HotLinks key");
		KEY(Key_BrightnessAdjust, "BrightnessAdjust", "The BrightnessAdjust key");
		KEY(Key_Finance, "Finance", "The Finance key");
		KEY(Key_Community, "Community", "The Community key");
		KEY(Key_AudioRewind, "AudioRewind", "The AudioRewind key");
		KEY(Key_BackForward, "BackForward", "The BackForward key");
		KEY(Key_ApplicationLeft, "ApplicationLeft", "The ApplicationLeft key");
		KEY(Key_ApplicationRight, "ApplicationRight", "The ApplicationRight key");
		KEY(Key_Book, "Book", "The Book key");
		KEY(Key_CD, "CD", "The CD key");
		KEY(Key_Calculator, "Calculator", "On X11 this key is not mapped for legacy reasons. Use Qt::Key_Launch1 instead.");
		KEY(Key_ToDoList, "ToDoList", "The ToDoList key");
		KEY(Key_ClearGrab, "ClearGrab", "The ClearGrab key");
		KEY(Key_Close, "Close", "The Close key");
		KEY(Key_Copy, "Copy", "The Copy key");
		KEY(Key_Cut, "Cut", "The Cut key");
		KEY(Key_Display, "Display", "The Display key");
		KEY(Key_DOS, "DOS", "The DOS key");
		KEY(Key_Documents, "Documents", "The Documents key");
		KEY(Key_Excel, "Excel", "The Excel key");
		KEY(Key_Explorer, "Explorer", "The Explorer key");
		KEY(Key_Game, "Game", "The Game key");
		KEY(Key_Go, "Go", "The Go key");
		KEY(Key_iTouch, "iTouch", "The iTouch key");
		KEY(Key_LogOff, "LogOff", "The LogOff key");
		KEY(Key_Market, "Market", "The Market key");
		KEY(Key_Meeting, "Meeting", "The Meeting key");
		KEY(Key_MenuKB, "MenuKB", "The MenuKB key");
		KEY(Key_MenuPB, "MenuPB", "The MenuPB key");
		KEY(Key_MySites, "MySites", "The MySites key");
		KEY(Key_News, "News", "The News key");
		KEY(Key_OfficeHome, "OfficeHome", "The OfficeHome key");
		KEY(Key_Option, "Option", "The Option key");
		KEY(Key_Paste, "Paste", "The Paste key");
		KEY(Key_Phone, "Phone", "The Phone key");
		KEY(Key_Calendar, "Calendar", "The Calendar key");
		KEY(Key_Reply, "Reply", "The Reply key");
		KEY(Key_Reload, "Reload", "The Reload key");
		KEY(Key_RotateWindows, "RotateWindows", "The RotateWindows key");
		KEY(Key_RotationPB, "RotationPB", "The RotationPB key");
		KEY(Key_RotationKB, "RotationKB", "The RotationKB key");
		KEY(Key_Save, "Save", "The Save key");
		KEY(Key_Send, "Send", "The Send key");
		KEY(Key_Spell, "Spell", "The Spell key");
		KEY(Key_SplitScreen, "SplitScreen", "The SplitScreen key");
		KEY(Key_Support, "Support", "The Support key");
		KEY(Key_TaskPane, "TaskPane", "The TaskPane key");
		KEY(Key_Terminal, "Terminal", "The Terminal key");
		KEY(Key_Tools, "Tools", "The Tools key");
		KEY(Key_Travel, "Travel", "The Travel key");
		KEY(Key_Video, "Video", "The Video key");
		KEY(Key_Word, "Word", "The Word key");
		KEY(Key_Xfer, "Xfer", "The Xfer key");
		KEY(Key_ZoomIn, "ZoomIn", "The ZoomIn key");
		KEY(Key_ZoomOut, "ZoomOut", "The ZoomOut key");
		KEY(Key_Away, "Away", "The Away key");
		KEY(Key_Messenger, "Messenger", "The Messenger key");
		KEY(Key_WebCam, "WebCam", "The WebCam key");
		KEY(Key_MailForward, "MailForward", "The MailForward key");
		KEY(Key_Pictures, "Pictures", "The Pictures key");
		KEY(Key_Music, "Music", "The Music key");
		KEY(Key_Battery, "Battery", "The Battery key");
		KEY(Key_Bluetooth, "Bluetooth", "The Bluetooth key");
		KEY(Key_WLAN, "WLAN", "The WLAN key");
		KEY(Key_UWB, "UWB", "The UWB key");
		KEY(Key_AudioForward, "AudioForward", "The AudioForward key");
		KEY(Key_AudioRepeat, "AudioRepeat", "The AudioRepeat key");
		KEY(Key_AudioRandomPlay, "AudioRandomPlay", "The AudioRandomPlay key");
		KEY(Key_Subtitle, "Subtitle", "The Subtitle key");
		KEY(Key_AudioCycleTrack, "AudioCycleTrack", "The AudioCycleTrack key");
		KEY(Key_Time, "Time", "The Time key");
		KEY(Key_Hibernate, "Hibernate", "The Hibernate key");
		KEY(Key_View, "View", "The View key");
		KEY(Key_TopMenu, "TopMenu", "The TopMenu key");
		KEY(Key_PowerDown, "PowerDown", "The PowerDown key");
		KEY(Key_Suspend, "Suspend", "The Suspend key");
		KEY(Key_ContrastAdjust, "ContrastAdjust", "The ContrastAdjust key");
		KEY(Key_TouchpadToggle, "TouchpadToggle", "The TouchpadToggle key");
		KEY(Key_TouchpadOn, "TouchpadOn", "The TouchpadOn key");
		KEY(Key_TouchpadOff, "TouchpadOff", "The TouchpadOff key");
		KEY(Key_MicMute, "MicMute", "The MicMute key");
		KEY(Key_MediaLast, "MediaLast", "The MediaLast key");
		KEY(Key_unknown, "unknown", "The unknown key");
		KEY(Key_Call, "Call", "A key to answer or initiate a call (see Qt::Key_ToggleCallHangup for a key to toggle current call state)");
		KEY(Key_Camera, "Camera", "A key to activate the camera shutter");
		KEY(Key_CameraFocus, "CameraFocus", "A key to focus the camera");
		KEY(Key_Context1, "Context1", "The Context1 key");
		KEY(Key_Context2, "Context2", "The Context2 key");
		KEY(Key_Context3, "Context3", "The Context3 key");
		KEY(Key_Context4, "Context4", "The Context4 key");
		KEY(Key_Flip, "Flip", "The Flip key");
		KEY(Key_Hangup, "Hangup", "A key to end an ongoing call (see Qt::Key_ToggleCallHangup for a key to toggle current call state)");
		KEY(Key_No, "No", "The No key");
		KEY(Key_Select, "Select", "The Select key");
		KEY(Key_Yes, "Yes", "The Yes key");
		KEY(Key_ToggleCallHangup, "ToggleCallHangup", "A key to toggle the current call state (ie. either answer, or hangup) depending on current call state");
		KEY(Key_VoiceDial, "VoiceDial", "The VoiceDial key");
		KEY(Key_LastNumberRedial, "LastNumberRedial", "The LastNumberRedial key");
		KEY(Key_Execute, "Execute", "The Execute key");
		KEY(Key_Printer, "Printer", "The Printer key");
		KEY(Key_Play, "Play", "The Play key");
		KEY(Key_Sleep, "Sleep", "The Sleep key");
		KEY(Key_Zoom, "Zoom", "The Zoom key");
		KEY(Key_Cancel, "Cancel", "The Cancel key");
#undef KEY

		foreach(ButtonInputSignal* button, _keyboardButtons.values())
		{
			_keyboardDevice->addButtonSignal(button);
		} // end foreach

		emit deviceAttached(_keyboardDevice);
	} // end if

	if(!_mouseDevice)
	{
		_mouseDevice = new GenericDevice(this, InputDevice::MOUSE, "Dumb Mouse");

		_mouseAxes["X"] = new AxisInputSignal(_mouseDevice, "X", "horizontal movement");
		_mouseAxes["Y"] = new AxisInputSignal(_mouseDevice, "Y", "vertical movement");

		_mouseDevice->addAxisSignal(_mouseAxes["X"]);
		_mouseDevice->addAxisSignal(_mouseAxes["Y"]);

#define BTN(BUTTON, NAME, DESC)	_mouseButtons[Qt::BUTTON] = new ButtonInputSignal(_mouseDevice, NAME, DESC)
		BTN(LeftButton, "Left", "The left button. (The left button may be the right button on left-handed mice.)");
		BTN(RightButton, "Right", "The right button.");
		BTN(MidButton, "Middle", "The middle button.");
		BTN(BackButton, "Back", "The 'Back' button. (Typically present on the 'thumb' side of a mouse with extra buttons. This is NOT the tilt wheel.)");
		BTN(ForwardButton, "Forward", "The 'Forward' button. (Typically present beside the 'Back' button, and also pressed by the thumb.)");
		BTN(TaskButton, "Task", "The 'Task' button.");
		BTN(ExtraButton4, "Extra 4", "The 7th non-wheel mouse button.");
		BTN(ExtraButton5, "Extra 5", "The 8th non-wheel mouse button.");
		BTN(ExtraButton6, "Extra 6", "The 9th non-wheel mouse button.");
		BTN(ExtraButton7, "Extra 7", "The 10th non-wheel mouse button.");
		BTN(ExtraButton8, "Extra 8", "The 11th non-wheel mouse button.");
		BTN(ExtraButton9, "Extra 9", "The 12th non-wheel mouse button.");
		BTN(ExtraButton10, "Extra 10", "The 13th non-wheel mouse button.");
		BTN(ExtraButton11, "Extra 11", "The 14th non-wheel mouse button.");
		BTN(ExtraButton12, "Extra 12", "The 15th non-wheel mouse button.");
		BTN(ExtraButton13, "Extra 13", "The 16th non-wheel mouse button.");
		BTN(ExtraButton14, "Extra 14", "The 17th non-wheel mouse button.");
		BTN(ExtraButton15, "Extra 15", "The 18th non-wheel mouse button.");
		BTN(ExtraButton16, "Extra 16", "The 19th non-wheel mouse button.");
		BTN(ExtraButton17, "Extra 17", "The 20th non-wheel mouse button.");
		BTN(ExtraButton18, "Extra 18", "The 21st non-wheel mouse button.");
		BTN(ExtraButton19, "Extra 19", "The 22nd non-wheel mouse button.");
		BTN(ExtraButton20, "Extra 20", "The 23rd non-wheel mouse button.");
		BTN(ExtraButton21, "Extra 21", "The 24th non-wheel mouse button.");
		BTN(ExtraButton22, "Extra 22", "The 25th non-wheel mouse button.");
		BTN(ExtraButton23, "Extra 23", "The 26th non-wheel mouse button.");
		BTN(ExtraButton24, "Extra 24", "The 27th non-wheel mouse button.");
#undef BTN

		foreach(ButtonInputSignal* button, _mouseButtons.values())
		{
			_mouseDevice->addButtonSignal(button);
		} // end foreach

		emit deviceAttached(_mouseDevice);
	} // end if

	_window = window;

	_screenSize = _window->screen()->size();
	connect(_window, SIGNAL(screenChanged(QScreen*)), this, SLOT(onScreenChanged(QScreen*)));

	connect(_window, SIGNAL(mouseMoved(QMouseEvent*, QPoint)), this, SLOT(onMouseMoved(QMouseEvent*, QPoint)));
	connect(_window, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(onMousePressed(QMouseEvent*)));
	connect(_window, SIGNAL(mouseReleased(QMouseEvent*)), this, SLOT(onMouseReleased(QMouseEvent*)));

	connect(_window, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(onKeyPressed(QKeyEvent*)));
	connect(_window, SIGNAL(keyReleased(QKeyEvent*)), this, SLOT(onKeyReleased(QKeyEvent*)));
} // end setWindow

void QtDriver::onScreenChanged(QScreen* screen)
{
	_screenSize = screen->size();
} // end onScreenChanged

void QtDriver::onKeyPressed(QKeyEvent* event)
{
	if(_keyboardButtons.contains(event->key()))
	{
		_keyboardButtons[event->key()]->emitUpdated(true, event->isAutoRepeat());
	}
	else
	{
		_logger.error(QString("WTF!?!? %1").arg(event->key()));
	} // end if
} // end onKeyPressed

void QtDriver::onKeyReleased(QKeyEvent* event)
{
	if(_keyboardButtons.contains(event->key()))
	{
		_keyboardButtons[event->key()]->emitUpdated(false, event->isAutoRepeat());
	}
	else
	{
		_logger.error(QString("WTF!?!? %1").arg(event->key()));
	} // end if
} // end onKeyReleased

void QtDriver::onMouseMoved(QMouseEvent* event, QPoint screenDelta)
{
	float divisor = fmax(_screenSize.width(), _screenSize.height());

	_mouseAxes["X"]->emitUpdated(screenDelta.x() / divisor);
	_mouseAxes["Y"]->emitUpdated(screenDelta.y() / divisor);
} // end onMouseMoved

void QtDriver::onMousePressed(QMouseEvent* event)
{
	_mouseButtons[event->button()]->emitUpdated(true);
} // end onMousePressed

void QtDriver::onMouseReleased(QMouseEvent* event)
{
	_mouseButtons[event->button()]->emitUpdated(false);
} // end onMouseReleased
