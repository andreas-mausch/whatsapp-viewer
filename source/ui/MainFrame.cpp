#include <memory>

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "DatabasePanel.h"
#include "FileOpenDialog.h"
#include "MainFrame.h"
#include "WelcomePanel.h"

#include "../whatsapp/Database.h"

namespace UI {

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(XRCID("ID_OpenDatabase"), MainFrame::OnOpenDatabase)
  EVT_MENU(wxID_EXIT, MainFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxWindow *parent)
    : mainPanel(std::nullopt) {
  wxXmlResource::Get()->LoadFrame(this, parent, _("MainFrame"));
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

  if (mainPanel) {
    (*mainPanel)->Destroy();
    mainPanel = std::nullopt;
  }

  auto database = std::make_unique<WhatsApp::Database>(*filename);
  setMainPanel(new DatabasePanel(this, std::move(database)));
}

void MainFrame::setMainPanel(wxPanel *panel) {
  if (mainPanel) {
    (*mainPanel)->Destroy();
  }

  mainPanel = std::make_optional<wxPanel *>(panel);
  wxXmlResource::Get()->AttachUnknownControl("mainPanel", *mainPanel);
}

} // namespace UI
