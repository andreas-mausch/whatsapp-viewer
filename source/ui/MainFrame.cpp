#include <memory>

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "DatabasePanel.h"
#include "FileOpenDialog.h"
#include "LoadingPanel.h"
#include "MainFrame.h"
#include "WelcomePanel.h"

#include "../whatsapp/Database.h"

namespace UI {

MainFrame::MainFrame(wxWindow *parent) :
  mainPanel(std::nullopt) {
  wxXmlResource::Get()->LoadFrame(this, parent, _("MainFrame"));
  DragAcceptFiles(true);

  Bind(wxEVT_MENU, &MainFrame::OnOpenDatabase, this, XRCID("ID_OpenDatabase"));
  Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_DROP_FILES, wxDropFilesEventHandler(MainFrame::OnDropFiles), this,
       wxID_ANY);

  SetIcon(wxXmlResource::Get()->LoadIcon(_("icon")));
  setMainPanel(new WelcomePanel(this));
}

void MainFrame::OnExit(wxCommandEvent &event) { Close(true); }

void MainFrame::OnAbout(wxCommandEvent &event) {
  wxMessageBox(_("WhatsApp Viewer by Andreas Mausch"),
               _("About WhatsApp Viewer"), wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpenDatabase(wxCommandEvent &event) {
  auto filename = fileOpenDialog(this);

  if (!filename) {
    return;
  }

  openDatabase(*filename);
}

void MainFrame::OnDropFiles(wxDropFilesEvent &event) {
  if (event.GetNumberOfFiles() == 1) {
    openDatabase(event.GetFiles()[0].ToStdString());
  }
}

void MainFrame::openDatabase(const std::string &filename) {
  try {
    auto database = std::make_unique<WhatsApp::Database>(filename);
    setMainPanel(new DatabasePanel(this, std::move(database)));
  } catch (std::exception &exception) {
    wxMessageBox(exception.what(), _("An error occured"), wxICON_ERROR);
  }
}

void MainFrame::setMainPanel(wxPanel *panel) {
  if (mainPanel) {
    (*mainPanel)->Destroy();
  }

  mainPanel = std::make_optional<wxPanel *>(panel);
  wxXmlResource::Get()->AttachUnknownControl("mainPanel", *mainPanel);
}

} // namespace UI
