#include <iostream>
#include <stdexcept>

#include <app/MainWindow.hpp>
#include <core/StreamRedirector.hpp>

int main(int argc, char *argv[]) {
    try {
        StreamRedirector coutRedirector(std::cout);
        StreamRedirector cerrRedirector(std::cerr);

        auto app = Gtk::Application::create(argc, argv, "com.noinsts.contest-maker");
        MainWindow window;
        return app -> run(window);
    } 
    catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << std::endl;
        return 1;
    }
}
