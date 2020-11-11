#pragma once

#include "ButtonWithSpinner/ButtonWithSpinner.h"

namespace UI {

class DecryptPanel : public wxPanel {
public:
  DecryptPanel(wxWindow *parent);
private:
  void onDecrypt(wxCommandEvent &event);
  void onDone(wxCommandEvent &event);
  void onError(wxCommandEvent &event);
  ButtonWithSpinner::ButtonWithSpinner &getDecryptButton();
};

} // namespace UI
