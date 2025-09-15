#include "app/DebugWindow.hpp"

DebugWindow::DebugWindow() {
    setupUI();
}

void DebugWindow::setupUI() {
    set_title("Debug відладка");
    set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT);
    set_position(Gtk::WIN_POS_CENTER);
    
    textView.set_editable(false);
    textView.set_monospace(true);

    scrolled.add(textView);
    scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    add(scrolled);
    show_all();
}
