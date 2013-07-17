# Precursors QML Client

This is our "experimental" attempt to build the precursors client using Qt/QML and Horde3D. Why the quotes? Well,
because things have been going so well that recently development has shifted almost entirely towards fleshing this out,
and trying to reach feature parity with the current client. If we're able to do that, and what we're left with is something
that works better, is easier to maintain, and all the developers like better, then we'll consider the experiment a
success, and make this the official client.

## Status

Currently, we have working network communication, complete QML and Horde3D integration, and some of the models ported over.
We actually perform a log in, get our list of character, display them, and log in. The client then can load scenes from the server, 
and also load entities when told to. Visually, we see the ship, and can rotate it or the camera with the keyboard or mouse. That's is.

Under the hood, we have the framework for a logging system, we have working settings, and we're wrapping Horde3D in a
nice QObject wrapper.

### ToDo:

* Finish converting the asteroid models
* Design and build a system for handling input/controlling the player's avatar.
	* We can already handle input, but it's in QML. At the very least, this needs to be in Javascript.
* Split the Horde3D code out into it's own library. [PARTIALLY COMPLETE]

Once we get here, we'll have parity with the "old" client. I recommend following the 
[issue list](https://bitbucket.org/skewedaspect/precursors-client-qml/issues?status=new&status=open) on BitBucket to 
see what we're planning on tackling next.

### Still to be Tackled:

As wonderful as this new project is, there's still some issues that need to be tackled:

* Building on Windows (just need to try it)
* Unit Tests
* Packaging for distribution (lots of groundwork's been done in the cmake files...)

## Building

In short, install Qt 5.1, Horde3D (from Subversion), and then just run `./build.sh`. Unfortunately, different platforms
require more than just those simple instructions, so we've written a whole section of the wiki for you:

[Building the Code](https://bitbucket.org/skewedaspect/precursors-client-qml/wiki/Building%20the%20Code)

Enjoy!

## Contributing

Feel free to fork and send us a pull request! Or, contact us at [developers@skewedaspect.com](mailto:developes@skewedaspect.com).
We welcome any contributions!
