#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "MessagePanel.h"

#include "../whatsapp/Message.h"

namespace UI {

MessagePanel::MessagePanel(wxWindow *parent, const WhatsApp::Message &message) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("MessagePanel"));
  XRCCTRL(*this, "message", wxStaticText)->SetLabelText(message.getId());
}

} // namespace UI
