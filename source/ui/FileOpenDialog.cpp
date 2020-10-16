#include <wx/wx.h>

#include "FileOpenDialog.h"

namespace UI {

std::optional<std::string> fileOpenDialog(wxWindow *parent) {
  wxFileDialog dialog(parent, _("Open Database file"), "", "",
                      "Database files (*.db)|*.db",
                      wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (dialog.ShowModal() == wxID_CANCEL) {
    return std::nullopt;
  }

  return std::make_optional(dialog.GetPath().ToStdString());
}

} // namespace UI
