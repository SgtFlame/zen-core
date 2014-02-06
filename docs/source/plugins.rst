Plugin System
==============

The bulk of the implementation within Zen Core exists in support of the Plugin System and the related registries.

Plugins are components inside of shared libraries (DLL or SO, depending on your platform).  These components are dynamically loaded at runtime on demand.

The Plugin System registry stores information about the different components that can be loaded.  Applications can interrogate the plugin registry, find a component that it needs to load, then load it dynamically at runtime.

Zen Core achieves this through concepts called Extensions and Extension Points.

Extension Points
-----------------

An Extension Point is a place within the framework which can be extended.  

It consists primarily of a definition inside of an XML file, plus an abstract interface that defines what the extension point should do.

Extensions
-----------

An Extension is an implementation of an Extension Point that does the extending.  In other words, it is the concrete implementation of the abstract interface deefined in the Extension Point.

How it works
-------------

An Extension Point is defined through the use of four pieces of code.  An interface, an abstract factory, a manager and an XML schema.

First is the Interface, which is defined through an abstract C++ class, generally called a Service.  This interface defines what the Extension Point can "do".  For example, "I_RenderingService" defines a Rendering Extension Point, and the abstract methods (virtual xxx() = 0) define what a Rendering Extension Point can do.

The second piece of an Extension Point is an abstract Factory.  The abstract Factory defines how an Extension Point Service is created.  Since it's abstract, the factory doesn't really create anything because the only implementation is the constructor and the destructor, which are generally empty.

The third piece of an Extension Point is a manager.  The manager of an extension point generally is a singleton helper class that implements the interaction between the extension point and the Zen Core Plugin System.

The last piece of of an Extension Point is actually coded in XML.  A portion of the XML defines the namespace, the service name and the location of a schema.  The namespace and service name defined in the XML generally corresponds with the C++ namespace and interface name of the extension point Service, minus the "I_" portion.  Using our previous example of the Rendering extension point, I_RenderingService is in the "Zen::Engine::Rendering" namespace, so inside the extension point XML, the namespace is declared as "Zen::Engine::Rendering" and the service name is "RenderingService".

The schema to which the XML points is a schema used as the meta data that defines what type of information is required when defining an Extension that implements the Extension Point.

In Zen Core, the bulk of the abstract classes are related to creating plugins and most of the interactions among the interfaces are rather vague, but their's a good reason for this.

Zen Core is a framework framework.  The sole purpose of the framework is to make it easier to create other frameworks.

