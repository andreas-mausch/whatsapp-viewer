#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/xrc/xmlres.h>

#include "MainFrame.h"
#include "FileOpenDialog.h"

#include "../whatsapp/Database.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(XRCID("ID_OpenDatabase"), MainFrame::OnOpenDatabase)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxWindow* parent)
{
    wxXmlResource::Get()->LoadFrame(this, parent, _("MainFrame"));
    SetIcon(wxXmlResource::Get()->LoadIcon(_("icon")));
    SetStatusText(_("Welcome to wxWidgets!"));
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(_("WhatsApp Viewer by Andreas Mausch"),
                 _("About WhatsApp Viewer"),
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
    chats = database->loadChats();
    updateChats();
}

void MainFrame::updateChats()
{
    wxListCtrl* chatControl = XRCCTRL(*this, "chats", wxListCtrl);
    chatControl->DeleteAllItems();

    for(auto& chat: chats)
    {
        chatControl->InsertItem(chatControl->GetItemCount(), chat.getId());
    }
}
