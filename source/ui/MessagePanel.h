#pragma once

namespace UI {

class MessagePanel : public wxPanel {
public:
  MessagePanel(wxWindow *parent, const std::string &message);
};

} // namespace UI
