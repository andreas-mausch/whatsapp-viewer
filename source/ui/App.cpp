#include <wx/xrc/xmlres.h>

#include "App.h"
#include "MainFrame.h"

extern void InitXmlResource();

namespace UI {

bool App::OnInit() {
  wxXmlResource::Get()->InitAllHandlers();
  InitXmlResource();
  wxInitAllImageHandlers();

  MainFrame *frame = new MainFrame(nullptr);
  frame->Show(true);

  return true;
}

} // namespace UI
