#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "HelloWorldApp.h"
#include "HelloWorldFrame.h"

extern void InitXmlResource();

bool HelloWorldApp::OnInit()
{
    wxXmlResource::Get()->InitAllHandlers();
    InitXmlResource();

    HelloWorldFrame *frame = new HelloWorldFrame(NULL);
    frame->Show(true);

    return true;
}
