# RFI: Precursors Official Client

Requiem for Innocence: Precursors ('RFI: Precursors', or just 'Precursors' for short) is an open source MMO-styled game, set in a science fiction epic. The universe is vast and multi-layered, with several factions all in conflict with each other... everything you would expect from a classic science fiction epic. The game itself is designed to be fun and enjoyable; suitable for casual players and small groups. We are focusing very strongly on making our gameplay as enjoyable for it's own sake as possible.

Our primary design imperative is: "Is it fun?"

## Project Overview

We've gone through many iterations to get to this point. We have a rock solid [server](https://bitbucket.org/skewedaspect/precursors-server), written in Erlang. We now have a rock solid client, written in Qt/QML. And the only gameplay we have is chasing each other around huge asteroids. (We can't even collide with each other yet.)

But, the groundwork is finally done. Now we're going to add new features, like (re)adding player chat. We're going to add space stations, and the ability to dock with them. We're going to refine our [basic gameplay](https://bitbucket.org/skewedaspect/precursors-client-qml/wiki/Design/Basic%20Gameplay), and get to the point where we can dogfight each other, and outfit our ships. After that, we're going to add more star systems, and the ability to travel between them. Pretty quickly, we're going to have an actual MMO. Then we can focus on adding the RPG elements. It's exciting!

## Status

Currently, we have working network communication, complete QML and Horde3D integration, and most of the models ported over.
We can perform a log in, get our list of character, display them, and send our choice back to the server. The client can then 
load scenes from the server, and also load entities when told to. We can fly around the map, and watch anyone else who logs in
fly around too. Under the hood, we have some incredibly powerful and streamlined frameworks for logging, networking, Horde3D integration,
and for handling input.

We're moving at a pretty quick pace; I recommend following the 
[issue list](https://bitbucket.org/skewedaspect/precursors-client-qml/issues?status=new&status=open) on BitBucket to 
see what we're planning on tackling next.

## Building

In short, install Qt 5.1, Horde3D (from Subversion), and then just run `./build.sh`. Unfortunately, different platforms
require more than just those simple instructions, so we've written a whole section of the wiki for you:

[Building the Code](https://bitbucket.org/skewedaspect/precursors-client-qml/wiki/Building%20the%20Code)

(It's not entirely complete; it get updated when we get time, and frankly, we've been focused on getting other things out the door.)

## Contributing

Feel free to fork and send us a pull request! Or, contact us at [developers@skewedaspect.com](mailto:developers@skewedaspect.com).
We welcome any contributions!

Not a programmer? No problem! We deperately need asset artists. Concept art, model design, textures... anything art related
would be appreciated. Just drop us a line at [developers@skewedaspect.com](mailto:developers@skewedaspect.com?Subject=Interested%20Artist).
