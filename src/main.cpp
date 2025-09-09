#include <iostream>
#include <stdexcept>

#include <app/MainWindow.hpp>

int main(int argc, char *argv[]) {
    try {
        auto app = Gtk::Application::create(argc, argv, "com.noinsts.contest-maker");
        MainWindow window;
        return app -> run(window);
    } 
    catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << std::endl;
        return 1;
    }
}
