#include "MainFrame.h"

#include <wx/xrc/xmlres.h>

#include <memory>

#include "../WhatsApp/Database.h"
#include "DatabasePanel.h"
#include "DecryptPanel.h"
#include "FileOpenDialog.h"
#include "WelcomePanel.h"

namespace UI {

MainFrame::MainFrame(wxWindow *parent) : panel(std::nullopt) {
  wxXmlResource::Get()->LoadFrame(this, parent, _("MainFrame"));
  DragAcceptFiles(true);

  Bind(wxEVT_MENU, &MainFrame::onOpenDatabase, this, XRCID("ID_OpenDatabase"));
  Bind(wxEVT_MENU, &MainFrame::onDecryptDatabase, this,
       XRCID("ID_DecryptDatabase"));
  Bind(wxEVT_MENU, &MainFrame::onExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &MainFrame::onAbout, this, wxID_ABOUT);
  Bind(wxEVT_DROP_FILES, wxDropFilesEventHandler(MainFrame::onDropFiles), this,
       wxID_ANY);

  SetIcon(wxXmlResource::Get()->LoadIcon(_("icon")));
  setPanel(new WelcomePanel(this));
}

void MainFrame::onExit(wxCommandEvent &event) { Close(true); }

void MainFrame::onAbout(wxCommandEvent &event) {
  wxMessageBox(_("WhatsApp Viewer by Andreas Mausch"),
               _("About WhatsApp Viewer"), wxOK | wxICON_INFORMATION);
}

void MainFrame::onOpenDatabase(wxCommandEvent &event) {
  auto filename = fileOpenDialog(this);

  if (!filename) {
    return;
  }

  openDatabase(*filename);
}

void MainFrame::onDecryptDatabase(wxCommandEvent &event) {
  setPanel(new DecryptPanel(this));
}

void MainFrame::onDropFiles(wxDropFilesEvent &event) {
  if (event.GetNumberOfFiles() == 1) {
    openDatabase(event.GetFiles()[0].ToStdString());
  }
}

void MainFrame::openDatabase(const std::string &filename) {
  try {
    auto database = std::make_unique<WhatsApp::Database>(filename);
    setPanel(new DatabasePanel(this, std::move(database)));
  } catch (std::exception &exception) {
    wxMessageBox(exception.what(), _("Something went wrong"), wxICON_ERROR);
  }
}

void MainFrame::setPanel(wxPanel *panel) {
  if (this->panel) {
    (*this->panel)->Destroy();
  }

  this->panel = std::make_optional<wxPanel *>(panel);
  wxXmlResource::Get()->AttachUnknownControl("panel", *this->panel);
}

} // namespace UI
