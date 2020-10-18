#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "WelcomePanel.h"

namespace UI {

wxBEGIN_EVENT_TABLE(WelcomePanel, wxPanel)
wxEND_EVENT_TABLE()

WelcomePanel::WelcomePanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("WelcomePanel"));
}

} // namespace UI
