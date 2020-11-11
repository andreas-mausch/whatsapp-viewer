#include "WelcomePanel.h"

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

namespace UI {

WelcomePanel::WelcomePanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("WelcomePanel"));
}

} // namespace UI
