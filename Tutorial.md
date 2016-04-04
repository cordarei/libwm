# Introduction #

Libwm is a cross-platform software library for window and context management in OpenGL applications.

# Creating an application with libwm #

## Application entry point ##

Libwm provides a portable application entry point. The client application entry point function is called `wm_main` and the actual entry point (`main` or `WinMain`) is provided by the `wm-entrypoint` library. For Win32 console applications you should use `wm-entrypoint-console` and configure your compiler settings to build console applications.

```
#include <wm/wm.hpp>

int wm_main(int argc, char *argv[]) // int wm_main(void) or void wm_main() do not work.
{
    return 0; // must return a value, no implicit return 0
}
```

The `wm-entrypoint` library does not perform any initialization, it is only a thin wrapper layer for application entry point and command line parameters.

### Optional: use platform specific entry point ###

If you don't want to use the Libwm entry point, you can to handle platform specific entry points yourself.

```
#ifdef WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
#else
int main(int argc, char *argv[])
#endif
```

## Initialize Display ##

A `Display` object is created to initialize Libwm. The Display constructor takes two parameters, a string `name` and an integer `screen`. They can be used to initialize Libwm to use a specific display and screen on the X Windowing System. Other platforms ignore these parameters. The default parameters are `NULL` for `name` and -1 to use the default `screen`. In the example below, no parameters are given and the defaults are used.

The `Display` object should not be destroyed before all `Window`, `Configuration` and other objects using the Display are destroyed. The destructor of Display will take care of de-initializing Libwm.

```
    wm::Display display;
```

## Query configuration ##

Before a window can be created, the framebuffer configuration must be queried. Creating a new `Configuration` object will query the available pixel formats from the windowing system. The Configuration object should not be destroyed before all Windows, Contexts, Surfaces, etc have been destroyed.

The `Configuration` constructor queries all suitable pixel formats. Only RGBA pixel formats that support double buffered OpenGL rendering to a window are accepted. The list of compatible the pixel formats of a configuration can be accessed using `Configuration::getFormat` and `::numFormats`. The easiest way to pick a pixel format is using the `wm::choose` function to select a pixel format that matches a given criteria. The match may not be exact.

```
    wm::Configuration config(display);
    const wm::PixelFormat& pixelformat = choose(config, 8, 8, 8, 8, 16, 0);
```

## Create a window ##

The `Window` constructor can be used to create new windows. The pixel format of a window cannot be changed after a window is created.

```
    const unsigned int width = 400, height = 300;
    wm::Window window(display, width, height, pixelformat);
```

### Optional: make the window unresizable ###

Many games prefer to use fixed size windows because of gameplay or aesthetic reasons or technical limitations. To make a Window unresizable, use `Window::setMinMaxSize` with equal and non-zero minimum and maximum size limits. The actual window size is not changed by setMinMaxSize. The window size may be changed by the user or the windowing system (even if the window is hidden) at any point, so to make sure that the window conforms to the size limits, `Window::resize` must be called after `setMinMaxSize`.

```
    window.setMinMaxSize(width, height, width, height);
    window.resize(width, height); // must be called after setMinMaxSize to enforce limits
```

### Optional: make the window full screen ###

`Window::fullscreen` can be used to request a full screen window from the windowing system. The windowing system will resize the window to fill the entire screen. If the full screen size is outside of the window size limits, the window will not be resized and fullscreened.

```
    window.fullscreen(true);
```

### Optional: set the Window title ###

To change the title string of a window, use `Window::setTitle`.

```
    window.setTitle("You can use UTF-8: \xc3\xa4");
```

### Optional: hide mouse cursor or move mouse ###

You can hide the mouse cursor with `Window::showCursor`. Hiding or showing the mouse cursor will only be in effect when the mouse cursor is over the specified window. You can move the mouse cursor using `Window::warpMouse`. The coordinates for `warpMouse` are given in window coordinates, relative to the upper left corner of the window client area.

```
    window.showCursor(false);
    window.warpMouse(width / 2, height / 2);
```

## Create rendering surface ##

To be able to draw into a window, a `Surface` and a `Context` are required. A rendering surface is associated with a window and there may be only one surface per window.

```
    wm::Surface surface(window);
```

## Create rendering context ##

The rendering context is required to store global OpenGL state and resources, such as textures and vertex buffers. The desired OpenGL version is specified when creating the context. However, the actual version of the context may differ from the requested version. Use `glGetIntegerv(GL_MAJOR_VERSION)`, `glGetError() == GL_INVALID_ENUM` and `glGetString(GL_VERSION)` to detect the actual OpenGL version. Note that `glGetString(GL_VERSION)` is deprecated in OpenGL 3.x. The rendering context is not associated with a window but must be created with a pixel format matching to the window it will be used with.

```
    int openGLversionMaj = 3, // Create OpenGL 3.2 context
        openGLversionMin = 2; 
    bool compatible = false,  // disable backward compatibility with OpenGL 3.1 and earlier
        debug = true,         // create a debug context
        direct = true;        // attempt to use direct rendering
    wm::Context *shared = 0;  // a pointer to a shared context
    wm::Context context(window.pixelformat(),
        openGLversionMaj, openGLversionMin,
        compatible, debug, direct,
        shared);
```

## Show window and make context current ##

`Window::show` and `::hide` are used to show and hide a window. To set the OpenGL context and rendering surface for the current thread, use `makeCurrent`. You need to show the window before using associated the rendering surface.

```
    window.show();                           // You must show the window
    makeCurrent(context, surface, surface);  // before using the rendering surface
```

## Draw and swap buffers ##

After calling `makeCurrent` to set the per-thread rendering context and surface, OpenGL can be used to draw to the window. After the drawing, `Surface::swap` is used to swap the double buffer.

```

#include <wm/opengl/opengl.hpp>  // handles platform specific #includes

    glClearColor(0.2, 0.4, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    /// Draw using OpenGL
    
    surface.swap(); // Swap buffers
```

## Event handling ##

Every `Window` has got it's own event queue. `Display::wait` or `::poll` is used to listen for events from the windowing system, process them and add new Libwm `Events` to the event queues of the windows corresponding to the source of the events. `Window::wait` and `::dispatch` are used to retrieve the events from the event queue and invoke user callbacks. User callbacks are done by overriding virtual functions in a `EventHandler` subclass.

```
    struct Handler : public wm::EventHandler
    {
        Handler() : quit(false), redraw(true) { }

        // The window was closed
        virtual void handle(const wm::CloseEvent&) { quit = true; }

        // A key was pressed or released
        virtual void handle(const wm::KeyEvent& event)
        {
            // Check if Esc was pressed
            if(event.symbol() == wm::keyboard::ESCAPE && event.state())
                quit = true;
        }

        // The window should be redrawn
        virtual void handle(const wm::ExposeEvent&) { redraw = true; }

        bool quit;
        bool redraw;
    };
```

Before an event handler can recieve events from a window, a `Connection` object must be created to connect the window and an event handler. A window can have many event handlers connected and event handler can be connected to many windows. When an event arrives, all event handlers that are connected to the event's source window are invoked in an unspecified order.

```
    Handler handler;           // Create an event handler
    wm::Connection connection(window, handler, false); // true to connect on constructor
    connection.connect();      // or false and connect later
    connection.disconnect();   // you can disconnect
    connection.connect();      // and then reconnect

    // the Connection destructor will disconnect, but in some cases you must explicitly disconnect
```

## Main loop ##

When a window is created, event handlers are connected and the rendering context and surface have been set up, it's time to run. Here is an example of a single window blocking main loop that waits for user input. It uses `Window::wait` that wait and process windowing system events using `Display::wait` until an event arrives to the window's event queue and then dispatches the events to the window's event handler.

```
    while(!handler.quit)
    {
        if(handler.redraw)
        {
            redraw();
            surface.swap();
            handler.redraw = false;
        }

        // Wait for events and call event handlers
        window.wait(); // wait for events (using Display::wait)
    }
```

Here is an example of a game or animation application main loop. It redraws the window at limited maximum frame rate and updates the game or animation state at a fixed interval. At the end of the main loop, windowing system events are processed using non-blocking `Display::poll` and `Window::dispatch`.

```
    while(!handler.quit)
    {
        timer = getCurrentTime();

        // Redraw with limited frame rate
        if(timer >= redrawTimestamp + redrawInterval)
        {
            redraw();
            surface.swap();
            redrawTimestamp = getCurrentTime();
        }

        // Update game or animation state at a fixed time step
        if(timer >= updateTimestamp + updateInterval)
        {
            for(t = updateTimestamp; t <= timer; t += updateInterval)
            {
                gametime += updateInterval;
                gamestate.update(gameinput, gametime, updateInterval);
            }

            updateTimestamp = t;
        }

        // Listen and process windowing system events
        display.poll();

        // Then call the event handlers
        if(!window.dispatch())
        {
            // No events to handle, sleep until there's work to do
            sleep_until(
                min(redrawTimestamp + redrawInterval,
                    updateTimestamp + updateInterval));
        }
    }
```