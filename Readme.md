# Precursors QML Client

This is our "experimental" attempt to build the precursors client using Qt/QML and Horde3D. Why the quotes? Well,
because things have been going so well that recently development has shifted almost entirely towards fleshing this out,
and trying to reach feature parity with the current client. If we're able to do that, and what we're left with is something
that works better, is easier to maintain, and all the developers like better, then we'll consider the experiment a
success, and make this the official client.

## Status

Currently, we have working network communication, complete QML and Horde3D integration, and the Ares model ported over.
We actually perform a log in, get our list of characters, and display them. After that, we don't do anything more. Visually,
we see the ship, and can rotate it or the camera with the keyboard or mouse. That's is.

Under the hood, we have the framework for a logging system, we have working settings, and we're wrapping Horde3D in a
nice QObject wrapper.

### ToDo:

* Send the selected character to the server.
* Handle entity events from the server, creating/loading entities, and updating them. (aka: port EntityManager)
* Handle level loading events from the server, loading the right scenes
	* Finish converting the asteroid models
	* Build the skybox
* Design and build a system for handling input/controlling the player's avatar.
	* We can already handle input, but it's in QML. At the very least, this needs to be in Javascript.
* Split the Horde3D code out into it's own library.

At this point, we'd have parity with the current client. Next up:

* Finish new GUI theme
* Implement 'Warp To' w/ gui.
* Switch to new Sol system scene.
* Implement Chat GUI.
* ???
* profit

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
