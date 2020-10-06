#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "App.h"
#include "MainFrame.h"

extern void InitXmlResource();

bool App::OnInit()
{
    wxXmlResource::Get()->InitAllHandlers();
    InitXmlResource();
    wxInitAllImageHandlers();

    MainFrame *frame = new MainFrame(nullptr);
    frame->Show(true);

    return true;
}
