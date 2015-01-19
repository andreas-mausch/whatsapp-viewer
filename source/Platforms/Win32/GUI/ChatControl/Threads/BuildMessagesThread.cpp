#include <windows.h>

#include "BuildMessagesThread.h"
#include "../ChatControl.h"
#include "../ChatControlElement.h"
#include "../Elements/ChatControlMessageFrame.h"
#include "../Elements/DayBreak.h"
#include "../Elements/Messages/ChatControlMessage.h"
#include "../Elements/Messages/ChatControlMessageAudio.h"
#include "../Elements/Messages/ChatControlMessageImage.h"
#include "../Elements/Messages/ChatControlMessageLocation.h"
#include "../Elements/Messages/ChatControlMessageText.h"
#include "../Elements/Messages/ChatControlMessageVideo.h"
#include "../../../Timestamp.h"
#include "../../../../../VectorUtils.h"
#include "../../../../../Synchronization/Lock.h"
#include "../../../../../WhatsApp/Chat.h"
#include "../../../../../WhatsApp/Message.h"

BuildMessagesThread::BuildMessagesThread(HWND window, Lock &lock, WhatsappChat *chat, std::vector<ChatControlElement *> &elements, Smileys &smileys, Font &dateFont, ImageDecoder &imageDecoder)
	: window(window), lock(lock), chat(chat), elements(elements), smileys(smileys), dateFont(dateFont), imageDecoder(imageDecoder)
{
}

BuildMessagesThread::~BuildMessagesThread()
{
}

void BuildMessagesThread::run()
{
	if (!lock.tryLockWhile(running))
	{
		return;
	}

	clearVector(elements);

	if (chat != NULL)
	{
		long long lastTimestamp = -1;

		std::vector<WhatsappMessage *> &messages = chat->getMessages(running);
		for (std::vector<WhatsappMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
		{
			if (!running)
			{
				break;
			}

			WhatsappMessage &message = **it;
			ChatControlMessageFrame *messageFrame = NULL;

			if (!isSameDay(lastTimestamp, message.getTimestamp()))
			{
				this->elements.push_back(new DayBreak(formatDate(message.getTimestamp()), 0, dateFont));
				lastTimestamp = message.getTimestamp();
			}

			int color;

			if (message.isFromMe())
			{
				color = RGB(186, 230, 145);
			}
			else
			{
				color = RGB(204, 217, 255);
			}

			switch (message.getMediaWhatsappType())
			{
				case MEDIA_WHATSAPP_TEXT:
				{
					messageFrame = new ChatControlMessageFrame(new ChatControlMessageText(message, 0, smileys), 0, color, dateFont);
				} break;
				case MEDIA_WHATSAPP_IMAGE:
				{
					if (message.getRawDataSize() > 0 && message.getRawData() != NULL)
					{
						messageFrame = new ChatControlMessageFrame(new ChatControlMessageImage(message, 0, imageDecoder), 0, color, dateFont);
					}
				} break;
				case MEDIA_WHATSAPP_AUDIO:
				{
					messageFrame = new ChatControlMessageFrame(new ChatControlMessageAudio(message, 0, smileys), 0, color, dateFont);
				} break;
				case MEDIA_WHATSAPP_VIDEO:
				{
					messageFrame = new ChatControlMessageFrame(new ChatControlMessageVideo(message, 0, imageDecoder), 0, color, dateFont);
				} break;
				case MEDIA_WHATSAPP_LOCATION:
				{
					messageFrame = new ChatControlMessageFrame(new ChatControlMessageLocation(message, 0, imageDecoder), 0, color, dateFont);
				} break;
			}

			if (messageFrame != NULL)
			{
				this->elements.push_back(messageFrame);
			}
		}
	}

	if (running)
	{
		PostMessage(window, WM_CHATCONTROL, CHAT_CONTROL_BUILDING_MESSAGES_FINISHED, 0);
	}

	lock.unlock();
}
