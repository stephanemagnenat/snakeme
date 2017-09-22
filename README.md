SnakeMe
=======

Copyright 2000 Cécile Grivaz, Stéphane Magnenat, Lynda Metref, Gabriel Walt.
All rights reserved.

Preamble
--------

This is the source code of my vintage snake game, back from year 2000!
I have corrected it so that it compiles without warnings with modern compilers and works on 64 bit platforms.
Yet, the C++ dialect is antique and the whole thing very dirty.
It is on github for the preservation of our common digital legacy.

[Stéphane Magnenat](http://stephane.magnenat)

Notes
-----

All source code and original graphics are included.
More graphics and levels can be downloaded at
http://stephane.magnenat.net/archives/snakeme/download.html

Full game extension documentation is available at:
http://stephane.magnenat.net/archives/snakeme/developers.html

Source tree explanation:

```
makeprev/	Preview maker tool for generating miniatures for the web site
SGU/		SnakeMe Graphics Utilities, support library under LGPL v.2
src/		Main SnakeMe source
snakeme/	SnakeMe ready-to-use working distribution, 
		just need a working executable to have the full game
tools/		Sprite editor binary for Windows
BUG.txt		List of known bugs
CHANGELOG.txt	The name is clear
INSTALL.txt	Detailed install instructions
README.txt	This file
CMakeLists.txt	CMake build file
SoundDOC.txt	Documentatino of sound effects with filename mapping
LICENSE.txt	The GNU General Public License v.2 which applies
		to the SnakeMe's source code, graphics and sounds.
```

If you want to compile SnakeMe, go on reading INSTALL.txt
