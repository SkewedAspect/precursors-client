# RFI: Precursors Official Client

Requiem for Innocence: Precursors ('RFI: Precursors', or just 'Precursors' for short) is an open source MMO-styled game, set in a science fiction epic. The universe is vast and multi-layered, with several factions all in conflict with each other... everything you would expect from a classic science fiction epic. The game itself is designed to be fun and enjoyable, and suitable for casual players and small groups.

Our primary design imperative is: "Is it fun?"

## Project Overview

This is our "experimental" attempt to build the precursors client using Qt/QML and Horde3D. Why the quotes? Well,
because things have been going so well that we've decided to make this the official client. Unfortunately, we're not _quite_
to the point of feature parity with the [old client](https://bitbucket.org/skewedaspect/precursors-client). Until we reach
that, this client will still be "experimental".

## Status

Currently, we have working network communication, complete QML and Horde3D integration, and most of the models ported over.
We can perform a log in, get our list of character, display them, and send our choice back to the server. The client can then 
load scenes from the server, and alsp load entities when told to. Visually, we see the ship, and can rotate it or the camera 
with the keyboard or mouse. If the old client connects, we can watch it fly around, but it never sees our ship move. (We're
not sending input information to the server.)

Under the hood, we have some incredibly powerful and streamlines frameworks for logging, networking, Horde3D integration,
and are currently working on one for handling input.

### ToDo:

* Design and build a system for handling input/controlling the player's avatar. [IN PROGRESS]
* Converting the asteroid models to Horde3D.	[PARTIALLY COMPLETE]
* Split the Horde3D code out into it's own library. [PARTIALLY COMPLETE]

Once we get here, we'll have parity with the "old" client. I recommend following the 
[issue list](https://bitbucket.org/skewedaspect/precursors-client-qml/issues?status=new&status=open) on BitBucket to 
see what we're planning on tackling next.

## Building

In short, install Qt 5.1, Horde3D (from Subversion), and then just run `./build.sh`. Unfortunately, different platforms
require more than just those simple instructions, so we've written a whole section of the wiki for you:

[Building the Code](https://bitbucket.org/skewedaspect/precursors-client-qml/wiki/Building%20the%20Code)

Enjoy!

## Contributing

Feel free to fork and send us a pull request! Or, contact us at [developers@skewedaspect.com](mailto:developes@skewedaspect.com).
We welcome any contributions!
