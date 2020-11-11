#include <wx/wx.h>
#include <wx/animate.h>
#include <wx/filepicker.h>
#include <wx/xrc/xmlres.h>

#include "DecryptPanel.h"
#include "../WhatsApp/Crypt/Crypt12.h"

namespace UI {

DecryptPanel::DecryptPanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("DecryptPanel"));
  Bind(wxEVT_BUTTON, &DecryptPanel::onDecrypt, this, getDecryptButton().GetId());
}

void DecryptPanel::onDecrypt(wxCommandEvent &event) {
  std::string encryptedDatabase = XRCCTRL(*this, "encryptedDatabase", wxFilePickerCtrl)->GetPath().ToStdString();
  std::string key = XRCCTRL(*this, "key", wxFilePickerCtrl)->GetPath().ToStdString();

  try {
    getDecryptButton().setLoading(true);
    WhatsApp::Crypt::decrypt12(encryptedDatabase, key, "output.db");
  } catch (const std::exception &exception) {
    wxMessageBox(exception.what(), _("An error occured"), wxICON_ERROR);
  }
  getDecryptButton().setLoading(false);
}

ButtonWithSpinner::ButtonWithSpinner &DecryptPanel::getDecryptButton() {
  return *static_cast<ButtonWithSpinner::ButtonWithSpinner *>(this->FindWindow("decrypt"));
}

} // namespace UI
