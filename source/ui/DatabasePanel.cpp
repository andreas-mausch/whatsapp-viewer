#include <asynqro/future.h>
#include <asynqro/tasks.h>

#include <wx/listctrl.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "DatabasePanel.h"
#include "MessagePanel.h"
#include "PanelList.cpp"

#include "../whatsapp/Chat.h"
#include "../whatsapp/Database.h"

namespace UI {

template class PanelList<WhatsApp::Message *, MessagePanel>;

DatabasePanel::DatabasePanel(wxWindow *parent, std::unique_ptr<WhatsApp::Database> database)
    : database(std::move(database)), selectedChat(std::nullopt) {
  Bind(wxEVT_LIST_ITEM_SELECTED, &DatabasePanel::OnDisplayChat, this, XRCID("chats"));
  wxXmlResource::Get()->LoadPanel(this, parent, _("DatabasePanel"));
  wxXmlResource::Get()->AttachUnknownControl("messages", new PanelList<WhatsApp::Message *, MessagePanel>(this));

  asynqro::tasks::run([this]() { return this->database->loadChats(); })
    .onSuccess([](std::vector<std::unique_ptr<WhatsApp::Chat>> chats) {
      // this->chats = std::move(chats);
      // this->updateChats();
    });
}

void DatabasePanel::OnDisplayChat(wxListEvent &event) {
  WhatsApp::Chat &chat = *reinterpret_cast<WhatsApp::Chat *>(event.GetData());
  openChat(chat);
}

void DatabasePanel::updateChats() {
  std::cout << "updateChats 1" << std::endl;
  wxListCtrl *chatControl = XRCCTRL(*this, "chats", wxListCtrl);
  std::cout << "updateChats 2" << std::endl;
  chatControl->DeleteAllItems();
  std::cout << "updateChats 3" << std::endl;

  for (auto &chat : chats) {
    std::cout << "updateChats 3a: " << chat->getId() << std::endl;
    wxListItem item;
    item.SetId(chatControl->GetItemCount());
    item.SetData(chat.get());
    item.SetText(chat->getId());
    chatControl->InsertItem(item);
  }
  std::cout << "updateChats 4" << std::endl;

  chatControl->SetColumnWidth(0, wxLIST_AUTOSIZE);
}

void DatabasePanel::openChat(WhatsApp::Chat &chat) {
/*  asynqro::tasks::run([this, &chat]() { return database->loadMessages(chat); })
    .onSuccess([this, &chat](auto messages) {
      chat.setMessages(std::move(messages));
      selectedChat = std::make_optional(&chat);

      auto *messagesPanel = static_cast<PanelList<WhatsApp::Message *, MessagePanel> *>(this->FindWindowByName("messages"));
      messagesPanel->clear();

      std::vector<WhatsApp::Message *> pointerMessages;
      std::transform(chat.getMessages().begin(), chat.getMessages().end(), std::back_inserter(pointerMessages),
          [](const auto& message){return message.get();});
      messagesPanel->setList(pointerMessages);
    });*/
}

} // namespace UI
