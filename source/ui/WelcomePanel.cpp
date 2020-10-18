#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "WelcomePanel.h"

namespace UI {

WelcomePanel::WelcomePanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("WelcomePanel"));
}

} // namespace UI
