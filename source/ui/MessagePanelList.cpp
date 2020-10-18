#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "MessagePanel.h"
#include "MessagePanelList.h"

#include "../whatsapp/Message.h"

namespace UI {

wxBEGIN_EVENT_TABLE(MessagePanelList, wxScrolledWindow)
wxEND_EVENT_TABLE()

MessagePanelList::MessagePanelList(wxWindow *parent) {
  wxXmlResource::Get()->LoadObject(this, parent, _("PanelList"), "wxScrolledWindow");
}

MessagePanelList::~MessagePanelList() {
  clear();
}

void MessagePanelList::clear() {
  GetSizer()->Clear(true);
}

void MessagePanelList::setList(const std::vector<WhatsApp::Message> &list) {
  for (auto &message : list) {
    GetSizer()->Add(new MessagePanel(this, message.getId()));
  }

  GetSizer()->Layout();
  SetVirtualSize(GetBestVirtualSize());
  FitInside();
}

} // namespace UI
