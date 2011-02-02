#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>

#include <wm/wm.hpp>
#include <wm/util/utf8.hpp>
#include <wm/util/ostream_output.hpp>

#include "draw.hpp"

int wm_main(int, char *[])
{
    wm::Display display;

    wm::Configuration config(display);
    wm::SimpleQueue queue;
    wm::Window window(display, 400, 300, choose(config, 8, 8, 8, 8, 16, 0), queue);
    wm::Surface surface(window);
    wm::Context context(window.pixelformat());

    window.show();
    wm::CurrentContext current(context, surface, surface);

    int min, maj;
    test::checkGLversion(maj, min);
    std::cout << "GL version " << maj << '.' << min << std::endl;

    surface.swap();

    bool quit_flag = false;
    while(!quit_flag)
    {
        display.wait();

        while(!queue.empty())
        {
            wm::Event event = queue.pop();
            std::cout << event << std::endl;

            switch(event.type)
            {
                case wm::EventType::BUTTON_DOWN:
                    quit_flag = true;
                    break;
                case wm::EventType::EXPOSE:
                    unsigned int width, height;
                    event.expose.window->surface().getSize(width, height);
                    test::draw(width, height, false);
                    event.expose.window->surface().swap();
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
}
