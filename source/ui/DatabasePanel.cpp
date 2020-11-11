#include "DatabasePanel.h"

#include <wx/listctrl.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "../WhatsApp/Database.h"
#include "LoadingPanel/LoadingPanel.h"
#include "MessagePanel.h"
#include "PanelList.cpp"

namespace UI {

template class PanelList<WhatsApp::Message *, MessagePanel>;

wxDEFINE_EVENT(DATABASE_PANEL_CHATS_LOADED, wxCommandEvent);
wxDEFINE_EVENT(DATABASE_PANEL_MESSAGES_LOADED, wxCommandEvent);

DatabasePanel::DatabasePanel(wxWindow *parent,
                             std::unique_ptr<WhatsApp::Database> database)
    : database(std::move(database)), selectedChat(std::nullopt) {
  Bind(wxEVT_LIST_ITEM_SELECTED, &DatabasePanel::onChatSelected, this,
       XRCID("chats"));
  Bind(DATABASE_PANEL_CHATS_LOADED, &DatabasePanel::onChatsLoaded, this);
  Bind(DATABASE_PANEL_MESSAGES_LOADED, &DatabasePanel::onMessagesLoaded, this);

  wxXmlResource::Get()->LoadPanel(this, parent, _("DatabasePanel"));
  wxXmlResource::Get()->AttachUnknownControl(
      "messages", new PanelList<WhatsApp::Message *, MessagePanel>(this));

  loadChats();
}

DatabasePanel::~DatabasePanel() {
  database->interrupt();
  getMessagesLoadingPanel().cancel();
  getDatabaseLoadingPanel().cancel();
}

LoadingPanel::LoadingPanel &DatabasePanel::getDatabaseLoadingPanel() {
  return *static_cast<LoadingPanel::LoadingPanel *>(
      this->FindWindow("databaseLoadingPanel"));
}

LoadingPanel::LoadingPanel &DatabasePanel::getMessagesLoadingPanel() {
  return *static_cast<LoadingPanel::LoadingPanel *>(
      this->FindWindow("messagesLoadingPanel"));
}

void DatabasePanel::loadChats() {
  auto cancellationToken = std::make_unique<async::cancellation_token>();
  auto task =
      async::spawn([this] {
        return this->database->loadChats();
      }).then([this](std::vector<std::unique_ptr<WhatsApp::Chat>> &&chats) {
        this->selectedChat = std::nullopt;
        this->chats = std::move(chats);
        wxPostEvent(this, wxCommandEvent(DATABASE_PANEL_CHATS_LOADED));
      });

  getDatabaseLoadingPanel().setTask(std::move(task),
                                    std::move(cancellationToken));
}

void DatabasePanel::onChatSelected(wxListEvent &event) {
  WhatsApp::Chat &chat = *reinterpret_cast<WhatsApp::Chat *>(event.GetData());
  displayChat(chat);
}

void DatabasePanel::onChatsLoaded(wxCommandEvent &event) {
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

void DatabasePanel::onMessagesLoaded(wxCommandEvent &event) {
  auto *messagesPanel =
      static_cast<PanelList<WhatsApp::Message *, MessagePanel> *>(
          FindWindow("messages"));
  messagesPanel->clear();

  if (selectedChat) {
    std::vector<WhatsApp::Message *> messagePointers;
    std::transform((*selectedChat)->getMessages().begin(),
                   (*selectedChat)->getMessages().end(),
                   std::back_inserter(messagePointers),
                   [](const auto &message) { return message.get(); });
    messagesPanel->setList(messagePointers);
  }
}

void DatabasePanel::displayChat(WhatsApp::Chat &chat) {
  database->interrupt();
  getMessagesLoadingPanel().cancel();

  auto cancellationToken = std::make_unique<async::cancellation_token>();
  auto task =
      async::spawn([this, &chat] { return this->database->loadMessages(chat); })
          .then([this, &chat](
                    std::vector<std::unique_ptr<WhatsApp::Message>> messages) {
            chat.setMessages(std::move(messages));
            selectedChat = std::make_optional(&chat);
            wxPostEvent(this, wxCommandEvent(DATABASE_PANEL_MESSAGES_LOADED));
          });

  getMessagesLoadingPanel().setTask(std::move(task),
                                    std::move(cancellationToken));
}

} // namespace UI
