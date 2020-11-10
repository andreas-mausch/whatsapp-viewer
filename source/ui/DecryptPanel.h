#pragma once

namespace UI {

class DecryptPanel : public wxPanel {
public:
  DecryptPanel(wxWindow *parent);
private:
  void onDecrypt(wxCommandEvent &event);
};

} // namespace UI
