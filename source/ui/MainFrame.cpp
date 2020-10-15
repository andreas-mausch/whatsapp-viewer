#include <wx/listctrl.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "FileOpenDialog.h"
#include "MainFrame.h"
#include "MessagePanel.h"

#include "../whatsapp/Database.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(XRCID("ID_OpenDatabase"), MainFrame::OnOpenDatabase)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_LIST_ITEM_SELECTED(XRCID("chats"), MainFrame::OnDisplayChat)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxWindow *parent) {
  wxXmlResource::Get()->LoadFrame(this, parent, _("MainFrame"));
  SetIcon(wxXmlResource::Get()->LoadIcon(_("icon")));
  SetStatusText(_("Welcome to wxWidgets!"));
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

  database = std::make_unique<WhatsApp::Database>(*filename);
  selectedChat = std::nullopt;
  chats = database->loadChats();
  updateChats();
}

void MainFrame::OnDisplayChat(wxListEvent &event) {
  WhatsApp::Chat &chat = *reinterpret_cast<WhatsApp::Chat *>(event.GetData());
  openChat(chat);
}

void MainFrame::updateChats() {
  wxListCtrl *chatControl = XRCCTRL(*this, "chats", wxListCtrl);
  chatControl->DeleteAllItems();

  for (auto &chat : chats) {
    wxListItem item;
    item.SetId(chatControl->GetItemCount());
    item.SetData(&chat);
    item.SetText(chat.getId());
    chatControl->InsertItem(item);
  }

  chatControl->SetColumnWidth(0, wxLIST_AUTOSIZE);
}

void MainFrame::openChat(WhatsApp::Chat &chat) {
  chat.setMessages(database->loadMessages(chat));
  selectedChat = std::make_optional(&chat);

  wxScrolledWindow *messages = XRCCTRL(*this, "messages", wxScrolledWindow);
  wxSizer *sizer = messages->GetSizer();
  sizer->Clear(true);

  for (auto &message : chat.getMessages()) {
    sizer->Add(new MessagePanel(messages, message.getId()));
  }

  sizer->Layout();
  messages->SetVirtualSize(messages->GetBestVirtualSize());
  messages->FitInside();
}
