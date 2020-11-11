#include <async++.h>

#include <wx/wx.h>
#include <wx/animate.h>
#include <wx/filepicker.h>
#include <wx/xrc/xmlres.h>

#include "DecryptPanel.h"
#include "../WhatsApp/Crypt/Crypt12.h"

namespace UI {

wxDEFINE_EVENT(DECRYPT_PANEL_DECRYPTED, wxCommandEvent);
wxDEFINE_EVENT(DECRYPT_PANEL_ERROR, wxCommandEvent);

DecryptPanel::DecryptPanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("DecryptPanel"));
  Bind(wxEVT_BUTTON, &DecryptPanel::onDecrypt, this, getDecryptButton().GetId());
  Bind(DECRYPT_PANEL_DECRYPTED, &DecryptPanel::onDone, this);
  Bind(DECRYPT_PANEL_ERROR, &DecryptPanel::onError, this);
}

void DecryptPanel::onDecrypt(wxCommandEvent &event) {
  std::string encryptedDatabase = XRCCTRL(*this, "encryptedDatabase", wxFilePickerCtrl)->GetPath().ToStdString();
  std::string key = XRCCTRL(*this, "key", wxFilePickerCtrl)->GetPath().ToStdString();

  getDecryptButton().setLoading(true);
  async::spawn([encryptedDatabase, key] { WhatsApp::Crypt::decrypt12(encryptedDatabase, key, "output.db"); })
    .then([this](async::task<void> task) {
      try {
        task.get();
        wxPostEvent(this, wxCommandEvent(DECRYPT_PANEL_DECRYPTED));
      } catch (const std::exception& e) {
        wxPostEvent(this, wxCommandEvent(DECRYPT_PANEL_ERROR));
      }
    });
}

void DecryptPanel::onDone(wxCommandEvent &event) {
  wxMessageBox(_("Database decrypted successfully"), _("Decryption"), wxICON_INFORMATION);
  getDecryptButton().setLoading(false);
}

void DecryptPanel::onError(wxCommandEvent &event) {
  wxMessageBox(_("Something went wrong"), _("An error occured"), wxICON_ERROR);
  getDecryptButton().setLoading(false);
}

ButtonWithSpinner::ButtonWithSpinner &DecryptPanel::getDecryptButton() {
  return *static_cast<ButtonWithSpinner::ButtonWithSpinner *>(this->FindWindow("decrypt"));
}

} // namespace UI
