#include <iostream>
#include <wx/xrc/xmlres.h>

#include "App.h"
#include "MainFrame.h"

extern void InitXmlResource();

namespace UI {

App::App() : options("WhatsAppViewer", "Small tool to display chats from the Android msgstore.db database (crypt12)") {}

bool App::OnInit() {
  wxXmlResource::Get()->InitAllHandlers();
  InitXmlResource();
  wxInitAllImageHandlers();

  MainFrame *frame = new MainFrame(nullptr);
  frame->Show(true);

  return true;
}

int App::OnRun() {
  try {
    char **argv = this->argv;

    options.add_options()
          ("h,help", "Print usage");
    auto parsed = options.parse(argc, argv);

    if (parsed.count("help")) {
      std::cout << options.help() << std::endl;
      return 0;
    }
  } catch (cxxopts::OptionException &exception) {
    std::cout << "Invalid options" << std::endl;
    std::cout << options.help() << std::endl;
    return -1;
  }

  return wxApp::OnRun();
}

} // namespace UI
