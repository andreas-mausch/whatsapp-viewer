#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/xrc/xmlres.h>

#include "DecryptPanel.h"
#include "../WhatsApp/Crypt/Crypt12.h"

namespace UI {

DecryptPanel::DecryptPanel(wxWindow *parent) {
  wxXmlResource::Get()->LoadPanel(this, parent, _("DecryptPanel"));
  Bind(wxEVT_BUTTON, &DecryptPanel::onDecrypt, this, XRCID("decrypt"));
}

void DecryptPanel::onDecrypt(wxCommandEvent &event) {
  std::string encryptedDatabase = XRCCTRL(*this, "encryptedDatabase", wxFilePickerCtrl)->GetPath().ToStdString();
  std::string key = XRCCTRL(*this, "key", wxFilePickerCtrl)->GetPath().ToStdString();

  try {
    WhatsApp::Crypt::decrypt12(encryptedDatabase, key, "output.db");
  } catch (const std::exception &exception) {
    wxMessageBox(exception.what(), _("An error occured"), wxICON_ERROR);
  }
}

} // namespace UI
