#include <wx/listctrl.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "DatabasePanel.h"
#include "LoadingPanel.h"
#include "MessagePanel.h"
#include "PanelList.cpp"

#include "../whatsapp/Database.h"

namespace UI {

template class PanelList<WhatsApp::Message *, MessagePanel>;

wxDEFINE_EVENT(DATABASE_PANEL_CHATS_LOADED, wxCommandEvent);
wxDEFINE_EVENT(DATABASE_PANEL_MESSAGES_LOADED, wxCommandEvent);

DatabasePanel::DatabasePanel(wxWindow *parent, std::unique_ptr<WhatsApp::Database> database)
    : database(std::move(database)), selectedChat(std::nullopt) {
  Bind(wxEVT_LIST_ITEM_SELECTED, &DatabasePanel::OnDisplayChat, this, XRCID("chats"));
  Bind(DATABASE_PANEL_CHATS_LOADED, &DatabasePanel::updateChats, this);
  Bind(DATABASE_PANEL_MESSAGES_LOADED, &DatabasePanel::updateMessages, this);
  wxXmlResource::Get()->LoadPanel(this, parent, _("DatabasePanel"));
  wxXmlResource::Get()->AttachUnknownControl("messages", new PanelList<WhatsApp::Message *, MessagePanel>(this));

  loadChats();
}

DatabasePanel::~DatabasePanel() {
  database->interrupt();
  getLoadingPanel().cancel();
}

LoadingPanel &DatabasePanel::getLoadingPanel() {
  return *static_cast<LoadingPanel *>(this->FindWindow("loadingPanel"));
}

void DatabasePanel::loadChats() {
  auto cancellationToken = std::make_unique<async::cancellation_token>();
  auto task = async::spawn([this] { return this->database->loadChats(); })
    .then([this](std::vector<std::unique_ptr<WhatsApp::Chat>> &&chats) {
      this->chats = std::move(chats);
      wxPostEvent(this, wxCommandEvent(DATABASE_PANEL_CHATS_LOADED));
    });

  getLoadingPanel().setTask(std::move(task), std::move(cancellationToken));
}

void DatabasePanel::OnDisplayChat(wxListEvent &event) {
  WhatsApp::Chat &chat = *reinterpret_cast<WhatsApp::Chat *>(event.GetData());
  openChat(chat);
}

void DatabasePanel::updateChats(wxCommandEvent &event) {
  wxListCtrl *chatControl = XRCCTRL(*this, "chats", wxListCtrl);
  chatControl->DeleteAllItems();

  for (auto &chat : chats) {
    wxListItem item;
    item.SetId(chatControl->GetItemCount());
    item.SetData(chat.get());
    item.SetText(chat->getId());
    chatControl->InsertItem(item);
  }

  chatControl->SetColumnWidth(0, wxLIST_AUTOSIZE);
}

void DatabasePanel::updateMessages(wxCommandEvent &event) {
  auto *messagesPanel = static_cast<PanelList<WhatsApp::Message *, MessagePanel> *>(this->FindWindow("messages"));
  messagesPanel->clear();

  if (selectedChat) {
    std::vector<WhatsApp::Message *> messagePointers;
    std::transform((*selectedChat)->getMessages().begin(), (*selectedChat)->getMessages().end(), std::back_inserter(messagePointers),
        [](const auto& message){return message.get();});
    messagesPanel->setList(messagePointers);
  }
}

void DatabasePanel::openChat(WhatsApp::Chat &chat) {
  async::spawn([this, &chat] { return this->database->loadMessages(chat); })
    .then([this, &chat](std::vector<std::unique_ptr<WhatsApp::Message>> messages) {
      chat.setMessages(std::move(messages));
      selectedChat = std::make_optional(&chat);
      wxPostEvent(this, wxCommandEvent(DATABASE_PANEL_MESSAGES_LOADED));
    });
}

} // namespace UI
