#pragma once

#include <wx/wx.h>

#include <memory>
#include <optional>
#include <vector>

class wxListEvent;

namespace WhatsApp {
class Chat;
class Database;
} // namespace WhatsApp

namespace UI {

namespace LoadingPanel {
class LoadingPanel;
}

class DatabasePanel : public wxPanel {
public:
  DatabasePanel(wxWindow *parent, std::unique_ptr<WhatsApp::Database> database);
  ~DatabasePanel();
  void loadChats();

private:
  std::unique_ptr<WhatsApp::Database> database;
  std::vector<std::unique_ptr<WhatsApp::Chat>> chats;
  std::optional<WhatsApp::Chat *> selectedChat;

  void onChatsLoaded(wxCommandEvent &event);
  void onMessagesLoaded(wxCommandEvent &event);

  void onChatSelected(wxListEvent &event);
  void displayChat(WhatsApp::Chat &chat);

  LoadingPanel::LoadingPanel &getDatabaseLoadingPanel();
  LoadingPanel::LoadingPanel &getMessagesLoadingPanel();
};

} // namespace UI
