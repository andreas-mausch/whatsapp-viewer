#pragma once

#include <vector>

namespace WhatsApp {
class Message;
} // namespace WhatsApp

namespace UI {

class MessagePanelList : public wxScrolledWindow {
public:
  MessagePanelList(wxWindow *parent);
  virtual ~MessagePanelList();

  void clear();
  void setList(const std::vector<WhatsApp::Message> &list);
};

} // namespace UI
