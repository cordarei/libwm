libwm is a cross-platform C++ library to help with windowing, event handling and context creation in [OpenGL](http://www.opengl.org/) applications.

### Libwm 0.2.1 released ###

**Jan 30th, 2011** Libwm 0.2.1 released

Changes:
  * New API for changing current OpenGL context
There will be no more libwm release tarballs, source code is available in the repository.

**Aug 28th, 2009** Libwm 0.2.0 released.

Changes:
  * `Surface::getSize`
  * OpenGL 3.x support using ARB\_create\_context extensions
  * Explicit control over direct/indirect rendering on Xlib
  * Separate read and draw surfaces on Win32 using WGL\_ARB\_make\_current\_read
  * Multisampling support using GLX 1.3, GLX\_ARB\_multisample for GLX 1.2 and WGL\_ARB\_multisample
  * Detect shared-exponent (sRGB) capable pixel formats
  * Support for floating point and packed unsigned float pixel formats
  * Detect slow, non-hardware accelerated pixel formats
  * `Configuration::getProcAddress` wrapper function for loading OpenGL extension functions

**Aug 13th, 2009** Libwm 0.1.0 released.


## Documentation ##

  * [Building instructions](http://code.google.com/p/libwm/wiki/Building)
  * [Tutorial](http://code.google.com/p/libwm/wiki/Tutorial)

## Features ##

  * Windowing, input event handling and OpenGL context creation with Win32 or X11 on Linux, Mac OS X and other Unixes
  * Not limited to single window, single OpenGL context or single thread
  * Internationalized text input using Windows Unicode or X Input Methods
  * Simple C++ programming interface
  * [RAII](http://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) for resource management, exceptions for error handling
  * Uses the [Pimpl idiom](http://en.wikipedia.org/wiki/Opaque_pointer) for strong encapsulation, no namespace clutter