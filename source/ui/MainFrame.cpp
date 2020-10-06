#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/xrc/xmlres.h>

#include "MainFrame.h"
#include "FileOpenDialog.h"

#include "../whatsapp/Database.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(XRCID("ID_OpenDatabase"),   MainFrame::OnOpenDatabase)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxWindow* parent)
{
    wxXmlResource::Get()->LoadFrame(this, parent, _("MainFrame"));
    SetStatusText("Welcome to wxWidgets!");
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(_("WhatsApp Viewer by Andreas Mausch"),
                 "About WhatsApp Viewer",
                 wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpenDatabase(wxCommandEvent& event)
{
    auto filename = fileOpenDialog(this);

    if (!filename)
    {
        return;
    }

    database = std::make_unique<WhatsApp::Database>(*filename);

    wxListCtrl* chats = XRCCTRL(*this, "chats", wxListCtrl);
    chats->DeleteAllItems();

    for(auto& chat: database->getChats())
    {
        chats->InsertItem(chats->GetItemCount(), chat.getId());
    }
}
