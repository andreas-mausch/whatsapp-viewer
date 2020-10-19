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

  frame = new MainFrame(nullptr);
  frame->Show(true);

  return true;
}

int App::OnRun() {
  try {
    char **argv = this->argv;

    options.add_options()
          ("h,help", "Print usage")
          ("database", "Load database file", cxxopts::value<std::string>());
    options.parse_positional({"database"});
    auto parsed = options.parse(argc, argv);

    if (parsed.count("help")) {
      std::cout << options.help() << std::endl;
      return 0;
    }

    if (parsed.count("database")) {
      auto filename = parsed["database"].as<std::string>();
      frame->openDatabase(filename);
    }
  } catch (cxxopts::OptionException &exception) {
    std::cout << "Invalid options: " << exception.what() << std::endl;
    std::cout << options.help() << std::endl;
    return -1;
  }

  return wxApp::OnRun();
}

} // namespace UI
