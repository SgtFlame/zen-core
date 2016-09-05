zen-core
========

Zen Core C++ Library

The Zen Core library adds several enhanced capabilities to C++

 * Events
 * 2d and 3d Math
 * Advanced smart pointers (deprecated, now using STL smart pointers)
 * Plugin system modelled after OSGi / Eclipse (moved to [indie-zen/zen-plugin](https://github.com/indie-zen/zen-plugin))
 * Scripting integration (moved to [indie-zen/zen-scripting](https://github.com/indie-zen/zen-scripting))
 * Multi-threading (deprecated, now using STL threading)

This library is cross platform, supporting gcc, XCode, and MSVC on Linux, Mac OS X and 
Windows operating systems.

It's currently being extracted from the Google Code repository which contained all of the
IndieZen source code.  This git repository contains all of the source code but does
not yet contain any of the CMake make files.  If you need help building this, please
reference the CMake files on Google Code: https://code.google.com/p/indiezen/

For additional documentation see http://sgtflame.github.io/zen-core/
