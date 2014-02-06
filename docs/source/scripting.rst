Scripting Framework
=====================

Also included in Zen Core is a scripting framework.  This framework makes it easier for you to integrate scripting languages into your application.

Using the framework, you define scriptable classes, object and methods in a generic way.  Scripting plugins utilize this information and implement the "glue" code that does the actual C/C++ binding to the script library.

Currently Lua and Python scripting libraries have been implemented, making it so that with very little code you can easily create applications that bind to either (or both) of those languages.  Adding new languages is simple, and as new scripting plugins are created, your applications using the scripting framework will already be capable of using the additional languages.


