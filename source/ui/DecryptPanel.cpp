#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "DecryptPanel.h"

namespace UI {

DecryptPanel::DecryptPanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("DecryptPanel"));
}

} // namespace UI
