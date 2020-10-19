#include <wx/listctrl.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "DatabasePanel.h"
#include "MessagePanel.h"
#include "PanelList.cpp"

#include "../whatsapp/Database.h"

namespace UI {

template class PanelList<WhatsApp::Message *, MessagePanel>;

DatabasePanel::DatabasePanel(wxWindow *parent, std::unique_ptr<WhatsApp::Database> database)
    : database(std::move(database)), selectedChat(std::nullopt) {
  Bind(wxEVT_LIST_ITEM_SELECTED, &DatabasePanel::OnDisplayChat, this, XRCID("chats"));
  wxXmlResource::Get()->LoadPanel(this, parent, _("DatabasePanel"));
  wxXmlResource::Get()->AttachUnknownControl("messages", new PanelList<WhatsApp::Message *, MessagePanel>(this));

  chats = this->database->loadChats();
  updateChats();
}

void DatabasePanel::OnDisplayChat(wxListEvent &event) {
  WhatsApp::Chat &chat = *reinterpret_cast<WhatsApp::Chat *>(event.GetData());
  openChat(chat);
}

void DatabasePanel::updateChats() {
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

void DatabasePanel::openChat(WhatsApp::Chat &chat) {
  chat.setMessages(database->loadMessages(chat));
  selectedChat = std::make_optional(&chat);

  auto *messagesPanel = static_cast<PanelList<WhatsApp::Message *, MessagePanel> *>(this->FindWindowByName("messages"));
  messagesPanel->clear();

  std::vector<WhatsApp::Message *> messages;
  std::transform(chat.getMessages().begin(), chat.getMessages().end(), std::back_inserter(messages),
      [](const auto& message){return message.get();});
  messagesPanel->setList(messages);
}

} // namespace UI
