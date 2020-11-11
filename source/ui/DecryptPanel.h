#pragma once

#include "ButtonWithSpinner/ButtonWithSpinner.h"

namespace UI {

class DecryptPanel : public wxPanel {
public:
  DecryptPanel(wxWindow *parent);
private:
  void onDecrypt(wxCommandEvent &event);
  ButtonWithSpinner::ButtonWithSpinner &getDecryptButton();
};

} // namespace UI
