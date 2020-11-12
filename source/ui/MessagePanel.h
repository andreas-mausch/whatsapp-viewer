#pragma once

#include <wx/wx.h>

namespace WhatsApp {
class Message;
} // namespace WhatsApp

namespace UI {

class MessagePanel : public wxPanel {
public:
  MessagePanel(wxWindow *parent, const WhatsApp::Message *message);
};

} // namespace UI
