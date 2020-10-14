#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "MessagePanel.h"

wxBEGIN_EVENT_TABLE(MessagePanel, wxPanel)
wxEND_EVENT_TABLE()

MessagePanel::MessagePanel(wxWindow* parent, const std::string &message)
{
    wxXmlResource::Get()->LoadPanel(this, parent, _("MessagePanel"));
    XRCCTRL(*this, "message", wxStaticText)->SetLabelText(message);
}
