#include "app/DebugWindow.hpp"
#include <core/StreamRedirector.hpp>

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

    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &DebugWindow::updateTextView),
        500 
    );
}

bool DebugWindow::updateTextView() {
    static size_t lastSize = 0;
    std::string text = StreamRedirector::getCapturedText();

    if (text.size() > lastSize) {
        addText(text.substr(lastSize));
        lastSize = text.size();
    }

    return true;
}

void DebugWindow::addText(const std::string& text) {
    auto buffer = textView.get_buffer();
    auto endIter = buffer->end();

    buffer->insert(endIter, text + "\n");
    textView.scroll_to(buffer->get_insert());
}
