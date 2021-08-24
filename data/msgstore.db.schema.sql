CREATE TABLE props (_id INTEGER PRIMARY KEY AUTOINCREMENT, key TEXT UNIQUE, value TEXT);
CREATE TABLE messages (_id INTEGER PRIMARY KEY AUTOINCREMENT, key_remote_jid TEXT NOT NULL, key_from_me INTEGER, key_id TEXT NOT NULL, status INTEGER, needs_push INTEGER, data TEXT, timestamp INTEGER, media_url TEXT, media_mime_type TEXT, media_wa_type TEXT, media_size INTEGER, media_name TEXT, media_caption TEXT, media_hash TEXT, media_duration INTEGER, origin INTEGER, latitude REAL, longitude REAL, thumb_image TEXT, remote_resource TEXT, received_timestamp INTEGER, send_timestamp INTEGER, receipt_server_timestamp INTEGER, receipt_device_timestamp INTEGER, read_device_timestamp INTEGER, played_device_timestamp INTEGER, raw_data BLOB, recipient_count INTEGER, participant_hash TEXT, starred INTEGER, quoted_row_id INTEGER, mentioned_jids TEXT, multicast_id TEXT, edit_version INTEGER, media_enc_hash TEXT, payment_transaction_id TEXT, forwarded INTEGER, preview_type INTEGER, send_count INTEGER, lookup_tables INTEGER, future_message_type INTEGER);
CREATE UNIQUE INDEX messages_key_index on messages (key_remote_jid, key_from_me, key_id);
CREATE INDEX media_hash_index on messages (media_hash);
CREATE INDEX media_type_index on messages (media_wa_type);
CREATE INDEX media_type_jid_index on messages (key_remote_jid, media_wa_type);
CREATE INDEX messages_jid_id_index on messages (key_remote_jid, _id);
CREATE INDEX starred_index on messages (starred);
CREATE TABLE message (_id INTEGER PRIMARY KEY AUTOINCREMENT, chat_row_id INTEGER NOT NULL, from_me INTEGER NOT NULL, key_id TEXT NOT NULL, sender_jid_row_id INTEGER, status INTEGER, broadcast INTEGER, recipient_count INTEGER, participant_hash TEXT, origination_flags INTEGER, origin INTEGER, timestamp INTEGER, received_timestamp INTEGER, receipt_server_timestamp INTEGER, message_type INTEGER, text_data TEXT, starred INTEGER, lookup_tables INTEGER, sort_id INTEGER NOT NULL DEFAULT 0 );
CREATE INDEX message_sort_id_index ON message (sort_id);
CREATE UNIQUE INDEX message_key_index on message (chat_row_id, from_me, key_id, sender_jid_row_id);
CREATE INDEX message_starred_index on message (starred);
CREATE INDEX message_type_chat_index on message (chat_row_id, message_type);
CREATE INDEX message_chat_id_index ON message (chat_row_id,_id);
CREATE INDEX message_type_index ON message(message_type);
CREATE VIRTUAL TABLE messages_fts USING FTS3()
/* messages_fts(content) */;
CREATE TABLE IF NOT EXISTS 'messages_fts_content'(docid INTEGER PRIMARY KEY, 'c0content');
CREATE TABLE IF NOT EXISTS 'messages_fts_segments'(blockid INTEGER PRIMARY KEY, block BLOB);
CREATE TABLE IF NOT EXISTS 'messages_fts_segdir'(level INTEGER,idx INTEGER,start_block INTEGER,leaves_end_block INTEGER,end_block INTEGER,root BLOB,PRIMARY KEY(level, idx));
CREATE VIRTUAL TABLE message_ftsv2 USING FTS4(content, fts_jid, fts_namespace)
/* message_ftsv2(content,fts_jid,fts_namespace) */;
CREATE TABLE IF NOT EXISTS 'message_ftsv2_content'(docid INTEGER PRIMARY KEY, 'c0content', 'c1fts_jid', 'c2fts_namespace');
CREATE TABLE IF NOT EXISTS 'message_ftsv2_segments'(blockid INTEGER PRIMARY KEY, block BLOB);
CREATE TABLE IF NOT EXISTS 'message_ftsv2_segdir'(level INTEGER,idx INTEGER,start_block INTEGER,leaves_end_block INTEGER,end_block INTEGER,root BLOB,PRIMARY KEY(level, idx));
CREATE TABLE IF NOT EXISTS 'message_ftsv2_docsize'(docid INTEGER PRIMARY KEY, size BLOB);
CREATE TABLE IF NOT EXISTS 'message_ftsv2_stat'(id INTEGER PRIMARY KEY, value BLOB);
CREATE TABLE messages_quotes (_id INTEGER PRIMARY KEY AUTOINCREMENT, key_remote_jid TEXT NOT NULL, key_from_me INTEGER, key_id TEXT NOT NULL, status INTEGER, needs_push INTEGER, data TEXT, timestamp INTEGER, media_url TEXT, media_mime_type TEXT, media_wa_type TEXT, media_size INTEGER, media_name TEXT, media_caption TEXT, media_hash TEXT, media_duration INTEGER, origin INTEGER, latitude REAL, longitude REAL, thumb_image TEXT, remote_resource TEXT, received_timestamp INTEGER, send_timestamp INTEGER, receipt_server_timestamp INTEGER, receipt_device_timestamp INTEGER, read_device_timestamp INTEGER, played_device_timestamp INTEGER, raw_data BLOB, recipient_count INTEGER, participant_hash TEXT, starred INTEGER, quoted_row_id INTEGER, mentioned_jids TEXT, multicast_id TEXT, edit_version INTEGER, media_enc_hash TEXT, payment_transaction_id TEXT, forwarded INTEGER, preview_type INTEGER, send_count INTEGER, lookup_tables INTEGER, future_message_type INTEGER);
CREATE TABLE message_quoted (message_row_id INTEGER PRIMARY KEY AUTOINCREMENT, chat_row_id INTEGER NOT NULL, parent_message_chat_row_id INTEGER NOT NULL, from_me INTEGER NOT NULL, sender_jid_row_id INTEGER, key_id TEXT NOT NULL, timestamp INTEGER, message_type INTEGER, origin INTEGER, text_data TEXT, payment_transaction_id TEXT, lookup_tables INTEGER);
CREATE TABLE messages_hydrated_four_row_template (message_row_id INTEGER PRIMARY KEY, message_template_id TEXT);
CREATE TABLE messages_vcards (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, sender_jid TEXT, chat_jid TEXT, vcard TEXT);
CREATE TABLE messages_vcards_jids (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, vcard_jid TEXT, vcard_row_id INTEGER);
CREATE TABLE message_orphaned_edit (_id INTEGER PRIMARY KEY, key_id TEXT    NOT NULL, from_me INTEGER NOT NULL, chat_row_id INTEGER NOT NULL, sender_jid_row_id INTEGER NOT NULL DEFAULT (0), timestamp INTEGER, message_type INTEGER NOT NULL, revoked_key_id TEXT, retry_count INTEGER);
CREATE UNIQUE INDEX message_orphaned_edit_key_index ON message_orphaned_edit (    key_id,    from_me,    chat_row_id,    sender_jid_row_id);
CREATE TABLE message_quoted_mentions (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, jid_row_id INTEGER);
CREATE UNIQUE INDEX quoted_mentions_index ON message_quoted_mentions (message_row_id, jid_row_id);
CREATE TABLE message_quoted_vcard (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, vcard TEXT);
CREATE UNIQUE INDEX message_quoted_vcard_index ON message_quoted_vcard (message_row_id, vcard);
CREATE TABLE message_product (message_row_id INTEGER PRIMARY KEY, business_owner_jid INTEGER, product_id TEXT, title TEXT, description TEXT, currency_code TEXT, amount_1000 INTEGER, retailer_id TEXT, url TEXT, product_image_count INTEGER, sale_amount_1000 INTEGER);
CREATE TABLE quoted_message_product (message_row_id INTEGER PRIMARY KEY, business_owner_jid INTEGER, product_id TEXT, title TEXT, description TEXT, currency_code TEXT, amount_1000 INTEGER, retailer_id TEXT, url TEXT, product_image_count INTEGER, sale_amount_1000 INTEGER);
CREATE TABLE message_quoted_product (message_row_id INTEGER PRIMARY KEY, business_owner_jid INTEGER, product_id TEXT, title TEXT, description TEXT, currency_code TEXT, amount_1000 INTEGER, retailer_id TEXT, url TEXT, product_image_count INTEGER, sale_amount_1000 INTEGER);
CREATE TABLE message_order (message_row_id INTEGER PRIMARY KEY, order_id TEXT,thumbnail BLOB,order_title TEXT,item_count INTEGER,status INTEGER,surface INTEGER,message TEXT,seller_jid INTEGER,token TEXT,currency_code TEXT,total_amount_1000 INTEGER);
CREATE TABLE quoted_message_order (message_row_id INTEGER PRIMARY KEY, order_id TEXT,thumbnail BLOB,order_title TEXT,item_count INTEGER,status INTEGER,surface INTEGER,message TEXT,seller_jid INTEGER,token TEXT,currency_code TEXT,total_amount_1000 INTEGER);
CREATE TABLE message_quoted_order (message_row_id INTEGER PRIMARY KEY, order_id TEXT,thumbnail BLOB,order_title TEXT,item_count INTEGER,status INTEGER,surface INTEGER,message TEXT,seller_jid INTEGER,token TEXT,currency_code TEXT,total_amount_1000 INTEGER);
CREATE TABLE message_group_invite (message_row_id INTEGER PRIMARY KEY, group_jid_row_id INTEGER NOT NULL, admin_jid_row_id INTEGER NOT NULL, group_name TEXT, invite_code TEXT, expiration INTEGER, invite_time INTEGER, expired INTEGER);
CREATE TABLE message_quoted_group_invite_legacy (message_row_id INTEGER PRIMARY KEY, group_jid_row_id INTEGER NOT NULL, admin_jid_row_id INTEGER NOT NULL, group_name TEXT, invite_code TEXT, expiration INTEGER, invite_time INTEGER, expired INTEGER);
CREATE TABLE message_quoted_group_invite (message_row_id INTEGER PRIMARY KEY, group_jid_row_id INTEGER NOT NULL, admin_jid_row_id INTEGER NOT NULL, group_name TEXT, invite_code TEXT, expiration INTEGER, invite_time INTEGER, expired INTEGER);
CREATE TABLE message_template (message_row_id            INTEGER PRIMARY KEY, content_text_data         TEXT NOT NULL, footer_text_data          TEXT, template_id               TEXT);
CREATE INDEX message_template_index ON message_template (message_row_id);
CREATE TABLE message_template_button (_id INTEGER PRIMARY KEY AUTOINCREMENT,message_row_id INTEGER, text_data TEXT NOT NULL, extra_data TEXT, button_type INTEGER,used INTEGER,selected_index INTEGER);
CREATE INDEX message_template_button_index ON message_template_button (message_row_id);
CREATE TABLE message_template_quoted (message_row_id INTEGER PRIMARY KEY, content_text_data TEXT NOT NULL,footer_text_data TEXT);
CREATE TABLE message_location (message_row_id INTEGER PRIMARY KEY, chat_row_id INTEGER, latitude REAL, longitude REAL, place_name TEXT, place_address TEXT, url TEXT, live_location_share_duration INTEGER, live_location_sequence_number INTEGER, live_location_final_latitude REAL, live_location_final_longitude REAL, live_location_final_timestamp INTEGER, map_download_status INTEGER);
CREATE TABLE message_quoted_location (message_row_id INTEGER PRIMARY KEY, latitude REAL, longitude REAL, place_name TEXT, place_address TEXT, url TEXT, thumbnail BLOB);
CREATE TABLE message_media (  message_row_id INTEGER PRIMARY KEY, chat_row_id INTEGER, autotransfer_retry_enabled INTEGER, multicast_id TEXT, media_job_uuid TEXT, transferred INTEGER, transcoded INTEGER, file_path TEXT, file_size INTEGER, suspicious_content INTEGER, trim_from INTEGER, trim_to INTEGER, face_x INTEGER, face_y INTEGER, media_key BLOB, media_key_timestamp INTEGER, width INTEGER, height INTEGER, has_streaming_sidecar INTEGER, gif_attribution INTEGER, thumbnail_height_width_ratio REAL, direct_path TEXT, first_scan_sidecar BLOB, first_scan_length INTEGER, message_url TEXT, mime_type TEXT, file_length INTEGER, media_name TEXT, file_hash TEXT, media_duration INTEGER, page_count INTEGER, enc_file_hash TEXT, partial_media_hash TEXT, partial_media_enc_hash TEXT, is_animated_sticker INTEGER, original_file_hash TEXT, mute_video INTEGER DEFAULT 0);
CREATE INDEX message_media_chat_index ON message_media(chat_row_id);
CREATE INDEX message_media_hash_index ON message_media(file_hash);
CREATE INDEX message_media_original_file_hash_index ON message_media(original_file_hash);
CREATE TABLE message_media_interactive_annotation (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, location_latitude REAL, location_longitude REAL, location_name TEXT, sort_order INTEGER);
CREATE UNIQUE INDEX message_media_interactive_annotation_index on message_media_interactive_annotation (message_row_id, sort_order);
CREATE TABLE message_media_interactive_annotation_vertex ( _id INTEGER PRIMARY KEY AUTOINCREMENT,  message_media_interactive_annotation_row_id INTEGER,  x REAL,  y REAL,  sort_order INTEGER);
CREATE UNIQUE INDEX message_media_interactive_annotation_vertex_index on message_media_interactive_annotation_vertex (message_media_interactive_annotation_row_id, sort_order);
CREATE TABLE message_quoted_media (message_row_id INTEGER PRIMARY KEY, media_job_uuid TEXT, transferred INTEGER, file_path TEXT, file_size INTEGER, media_key BLOB, media_key_timestamp INTEGER, width INTEGER, height INTEGER, direct_path TEXT, message_url TEXT, mime_type TEXT, file_length INTEGER, media_name TEXT, file_hash TEXT, media_duration INTEGER, page_count INTEGER, enc_file_hash TEXT, thumbnail BLOB);
CREATE TABLE frequents (_id INTEGER PRIMARY KEY AUTOINCREMENT, jid TEXT NOT NULL, type INTEGER NOT NULL, message_count INTEGER NOT NULL);
CREATE TABLE frequent (_id INTEGER PRIMARY KEY AUTOINCREMENT, jid_row_id INTEGER NOT NULL, type INTEGER NOT NULL, message_count INTEGER NOT NULL);
CREATE UNIQUE INDEX frequent_index ON frequent (jid_row_id, type);
CREATE TABLE receipt_user(_id INTEGER PRIMARY KEY AUTOINCREMENT,message_row_id INTEGER NOT NULL,receipt_user_jid_row_id INTEGER NOT NULL,receipt_timestamp INTEGER,read_timestamp INTEGER,played_timestamp INTEGER);
CREATE UNIQUE INDEX receipt_user_index on receipt_user(message_row_id,receipt_user_jid_row_id);
CREATE TABLE receipt_device (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER NOT NULL, receipt_device_jid_row_id INTEGER NOT NULL, receipt_device_timestamp INTEGER);
CREATE UNIQUE INDEX receipt_device_index ON receipt_device (message_row_id, receipt_device_jid_row_id);
CREATE INDEX receipt_device_table_device_index on receipt_device (receipt_device_jid_row_id);
CREATE TABLE receipt_orphaned (_id INTEGER PRIMARY KEY AUTOINCREMENT, chat_row_id INTEGER NOT NULL, from_me INTEGER NOT NULL, key_id TEXT NOT NULL, receipt_device_jid_row_id INTEGER NOT NULL, receipt_recipient_jid_row_id INTEGER, status INTEGER, timestamp INTEGER);
CREATE UNIQUE INDEX receipt_orphaned_index ON receipt_orphaned (chat_row_id, from_me, key_id, receipt_device_jid_row_id, receipt_recipient_jid_row_id, status);
CREATE TABLE receipts (_id INTEGER PRIMARY KEY AUTOINCREMENT, key_remote_jid TEXT NOT NULL, key_id TEXT NOT NULL, remote_resource TEXT, receipt_device_timestamp INTEGER, read_device_timestamp INTEGER, played_device_timestamp INTEGER);
CREATE INDEX receipts_key_index on receipts (key_remote_jid, key_id);
CREATE TABLE message_mentions (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, jid_row_id INTEGER);
CREATE UNIQUE INDEX mentions_index ON message_mentions (message_row_id, jid_row_id);
CREATE TABLE message_vcard (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, vcard TEXT);
CREATE UNIQUE INDEX message_vcard_index ON message_vcard (message_row_id, vcard);
CREATE TABLE message_media_vcard_count(_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, count INTEGER);
CREATE UNIQUE INDEX message_media_vcard_count_index on message_media_vcard_count(message_row_id);
CREATE TABLE message_vcard_jid(_id INTEGER PRIMARY KEY AUTOINCREMENT, vcard_jid_row_id INTEGER, vcard_row_id INTEGER, message_row_id INTEGER);
CREATE UNIQUE INDEX message_vcard_jid_index on message_vcard_jid(vcard_jid_row_id, vcard_row_id, message_row_id);
CREATE TABLE user_device (_id INTEGER PRIMARY KEY AUTOINCREMENT, user_jid_row_id INTEGER, device_jid_row_id INTEGER, key_index INTEGER NOT NULL DEFAULT 0);
CREATE UNIQUE INDEX user_device_index on user_device (user_jid_row_id,device_jid_row_id);
CREATE TABLE group_participant_user (_id INTEGER PRIMARY KEY AUTOINCREMENT, group_jid_row_id INTEGER NOT NULL, user_jid_row_id INTEGER NOT NULL, rank INTEGER, pending INTEGER);
CREATE UNIQUE INDEX group_participant_user_index ON group_participant_user (group_jid_row_id, user_jid_row_id);
CREATE TABLE group_participant_device (_id INTEGER PRIMARY KEY AUTOINCREMENT, group_participant_row_id INTEGER NOT NULL, device_jid_row_id INTEGER NOT NULL, sent_sender_key INTEGER);
CREATE UNIQUE INDEX group_participant_device_index ON group_participant_device (group_participant_row_id, device_jid_row_id);
CREATE TABLE message_external_ad_content (message_row_id INTEGER PRIMARY KEY, title TEXT, body TEXT, media_type INTEGER, thumbnail_url TEXT, full_thumbnail BLOB, micro_thumbnail BLOB, media_url TEXT, source_type TEXT, source_id TEXT, source_url TEXT);
CREATE TABLE group_participants (_id INTEGER PRIMARY KEY AUTOINCREMENT, gjid TEXT NOT NULL, jid TEXT NOT NULL, admin INTEGER, pending INTEGER, sent_sender_key INTEGER);
CREATE UNIQUE INDEX group_participants_index ON group_participants (gjid, jid);
CREATE TABLE group_participants_history (_id INTEGER PRIMARY KEY AUTOINCREMENT, timestamp DATETIME NOT NULL, gjid TEXT NOT NULL, jid TEXT NOT NULL, action INTEGER NOT NULL, old_phash TEXT NOT NULL, new_phash TEXT NOT NULL);
CREATE INDEX group_participants_history_index ON group_participants_history (gjid);
CREATE TABLE group_notification_version (group_jid_row_id INTEGER PRIMARY KEY, subject_timestamp INTEGER NOT NULL, announcement_version INTEGER NOT NULL, participant_version INTEGER NOT NULL);
CREATE UNIQUE INDEX group_notification_version_index ON group_notification_version (group_jid_row_id);
CREATE TABLE media_refs (_id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT UNIQUE, ref_count INTEGER);
CREATE TABLE message_thumbnails (thumbnail BLOB, timestamp DATETIME, key_remote_jid TEXT NOT NULL, key_from_me INTEGER, key_id TEXT NOT NULL);
CREATE UNIQUE INDEX messages_thumbnail_key_index ON message_thumbnails (key_remote_jid, key_from_me, key_id);
CREATE TABLE message_streaming_sidecar (message_row_id INTEGER PRIMARY KEY, sidecar BLOB, chunk_lengths BLOB, timestamp INTEGER);
CREATE TABLE mms_thumbnail_metadata (message_row_id INTEGER PRIMARY KEY, direct_path TEXT, media_key BLOB, media_key_timestamp INTEGER, enc_thumb_hash TEXT, thumb_hash TEXT, thumb_width INTEGER, thumb_height INTEGER, transferred INTEGER, micro_thumbnail BLOB, insert_timestamp INTEGER);
CREATE INDEX mms_thumbnail_metadata_transferred_index ON mms_thumbnail_metadata(transferred);
CREATE TABLE audio_data (message_row_id INTEGER PRIMARY KEY, waveform BLOB);
CREATE TABLE status_list (_id INTEGER PRIMARY KEY AUTOINCREMENT, key_remote_jid TEXT UNIQUE, message_table_id INTEGER, last_read_message_table_id INTEGER, last_read_receipt_sent_message_table_id INTEGER, first_unread_message_table_id INTEGER, autodownload_limit_message_table_id INTEGER, timestamp INTEGER, unseen_count INTEGER, total_count INTEGER);
CREATE TABLE status (_id INTEGER PRIMARY KEY AUTOINCREMENT, jid_row_id INTEGER UNIQUE, message_table_id INTEGER, last_read_message_table_id INTEGER, last_read_receipt_sent_message_table_id INTEGER, first_unread_message_table_id INTEGER, autodownload_limit_message_table_id INTEGER, timestamp INTEGER, unseen_count INTEGER, total_count INTEGER);
CREATE TABLE deleted_chat_job
(        _id INTEGER PRIMARY KEY AUTOINCREMENT,
        chat_row_id INTEGER NOT NULL,
        block_size INTEGER,
        deleted_message_row_id INTEGER,
        deleted_starred_message_row_id INTEGER,
        deleted_messages_remove_files BOOLEAN,
        deleted_categories_message_row_id INTEGER,
        deleted_categories_starred_message_row_id INTEGER,
        deleted_categories_remove_files BOOLEAN,
        deleted_message_categories TEXT);
CREATE INDEX deleted_chat_job_index ON deleted_chat_job (chat_row_id, _id);
CREATE TABLE pay_transactions (key_remote_jid TEXT, key_from_me INTEGER, key_id TEXT, id TEXT, timestamp INTEGER, status INTEGER, error_code TEXT, sender TEXT, receiver TEXT, type INTEGER, currency TEXT, amount_1000, credential_id TEXT, methods TEXT, bank_transaction_id TEXT, metadata TEXT, init_timestamp INTEGER, request_key_id TEXT, country TEXT, version INTEGER, future_data BLOB,service_id INTEGER, background_id TEXT);
CREATE UNIQUE INDEX message_payment_transactions_index ON pay_transactions (key_id);
CREATE UNIQUE INDEX message_payment_transactions_id_index ON pay_transactions (id);
CREATE TABLE pay_transaction (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, remote_jid_row_id INTEGER, key_id TEXT, interop_id TEXT, id TEXT, timestamp INTEGER, status INTEGER, error_code TEXT, sender_jid_row_id INTEGER, receiver_jid_row_id INTEGER, type INTEGER, currency_code TEXT, amount_1000, credential_id TEXT, methods TEXT, bank_transaction_id TEXT, metadata TEXT, init_timestamp INTEGER, request_key_id TEXT, country TEXT, version INTEGER, future_data BLOB, service_id INTEGER, background_id TEXT);
CREATE UNIQUE INDEX message_payment_transaction_id_index ON pay_transaction (id);
CREATE TABLE payment_background (background_id TEXT PRIMARY KEY, file_size INTEGER, width INTEGER, height INTEGER, mime_type TEXT, placeholder_color INTEGER, text_color INTEGER, subtext_color INTEGER, fullsize_url TEXT, description TEXT, lg TEXT);
CREATE TABLE payment_background_order (background_id TEXT PRIMARY KEY, background_order INTEGER);
CREATE TABLE call_log (_id INTEGER PRIMARY KEY AUTOINCREMENT, jid_row_id INTEGER, from_me INTEGER, call_id TEXT, transaction_id INTEGER, timestamp INTEGER, video_call INTEGER, duration INTEGER, call_result INTEGER, bytes_transferred INTEGER, group_jid_row_id INTEGER NOT NULL DEFAULT 0, is_joinable_group_call INTEGER, call_creator_device_jid_row_id INTEGER NOT NULL DEFAULT 0, call_random_id TEXT);
CREATE UNIQUE INDEX call_log_key_index ON call_log (jid_row_id, from_me, call_id, transaction_id);
CREATE TABLE call_log_participant_v2 (    _id             INTEGER PRIMARY KEY AUTOINCREMENT,    call_log_row_id INTEGER,    jid_row_id      INTEGER,    call_result     INTEGER);
CREATE UNIQUE INDEX call_log_participant_key_index on call_log_participant_v2 (call_log_row_id, jid_row_id);
CREATE TABLE joinable_call_log (call_log_row_id INTEGER PRIMARY KEY,call_id TEXT NOT NULL,joinable_video_call INTEGER NOT NULL DEFAULT 0,group_jid_row_id INTEGER NOT NULL DEFAULT 0);
CREATE UNIQUE INDEX joinable_call_log_call_id_index on joinable_call_log (call_id);
CREATE INDEX joinable_call_log_group_jid_row_id_index on joinable_call_log (group_jid_row_id);
CREATE TABLE missed_call_logs (_id INTEGER PRIMARY KEY AUTOINCREMENT, message_row_id INTEGER, timestamp INTEGER, video_call INTEGER, group_jid_row_id INTEGER NOT NULL DEFAULT 0, is_joinable_group_call INTEGER);
CREATE UNIQUE INDEX missed_call_logs_key_index ON missed_call_logs (message_row_id);
CREATE TABLE missed_call_log_participant (_id INTEGER PRIMARY KEY AUTOINCREMENT, call_logs_row_id INTEGER, jid TEXT, call_result INTEGER);
CREATE UNIQUE INDEX missed_call_log_participants_key_index on missed_call_log_participant (call_logs_row_id, jid);
CREATE TABLE jid (_id INTEGER PRIMARY KEY AUTOINCREMENT, user TEXT NOT NULL, server TEXT NOT NULL, agent INTEGER, device INTEGER, type INTEGER, raw_string TEXT);
CREATE UNIQUE INDEX jid_key_new_index ON jid (    user,    server,    agent,    device,    type);
CREATE UNIQUE INDEX jid_raw_string_index ON jid (    raw_string);
CREATE TABLE chat (_id INTEGER PRIMARY KEY AUTOINCREMENT,jid_row_id INTEGER UNIQUE,hidden INTEGER,subject TEXT,created_timestamp INTEGER,display_message_row_id INTEGER,last_message_row_id INTEGER,last_read_message_row_id INTEGER,last_read_receipt_sent_message_row_id INTEGER,last_important_message_row_id INTEGER,archived INTEGER,sort_timestamp INTEGER,mod_tag INTEGER,gen REAL,spam_detection INTEGER,unseen_earliest_message_received_time INTEGER,unseen_message_count INTEGER,unseen_missed_calls_count INTEGER,unseen_row_count INTEGER,plaintext_disabled INTEGER,vcard_ui_dismissed INTEGER,change_number_notified_message_row_id INTEGER,show_group_description INTEGER,ephemeral_expiration INTEGER,last_read_ephemeral_message_row_id INTEGER,ephemeral_setting_timestamp INTEGER,ephemeral_disappearing_messages_initiator INTEGER,unseen_important_message_count INTEGER NOT NULL DEFAULT 0 );
CREATE TABLE message_link (_id INTEGER PRIMARY KEY AUTOINCREMENT, chat_row_id INTEGER, message_row_id INTEGER, link_index INTEGER);
CREATE UNIQUE INDEX message_link_index ON message_link (message_row_id, link_index);
CREATE TABLE message_thumbnail (message_row_id INTEGER PRIMARY KEY, thumbnail BLOB);
CREATE TABLE media_hash_thumbnail (media_hash TEXT PRIMARY KEY, thumbnail BLOB);
CREATE TABLE message_forwarded(message_row_id INTEGER PRIMARY KEY, forward_score INTEGER);
CREATE TABLE message_text (message_row_id INTEGER PRIMARY KEY, description TEXT, page_title TEXT, url TEXT, font_style INTEGER, text_color INTEGER, background_color INTEGER, preview_type INTEGER);
CREATE TABLE message_quoted_text (message_row_id INTEGER PRIMARY KEY, thumbnail BLOB);
CREATE TABLE message_future(message_row_id INTEGER PRIMARY KEY, version INTEGER, data BLOB, future_message_type INTEGER);
CREATE TABLE message_revoked (message_row_id INTEGER PRIMARY KEY, revoked_key_id TEXT NOT NULL);
CREATE TABLE message_rating (message_row_id INTEGER PRIMARY KEY, rating INTEGER NOT NULL);
CREATE TABLE message_payment (    message_row_id                         INTEGER PRIMARY KEY,    sender_jid_row_id                      INTEGER,    receiver_jid_row_id                    INTEGER,    amount_with_symbol                     TEXT,    remote_resource                        TEXT,    remote_message_sender_jid_row_id       INTEGER,    remote_message_from_me                 INTEGER,    remote_message_key                     TEXT);
CREATE TABLE message_payment_transaction_reminder (    message_row_id            INTEGER PRIMARY KEY,    web_stub                  TEXT,    amount                    TEXT,    transfer_date             TEXT,    payment_sender_name       TEXT,    expiration                INTEGER,    remote_message_key        TEXT);
CREATE TABLE message_payment_status_update (    message_row_id         INTEGER PRIMARY KEY,    transaction_info       TEXT,    transaction_data       TEXT,    init_timestamp         TEXT,    update_timestamp       TEXT,    amount_data            TEXT);
CREATE TABLE message_send_count (message_row_id INTEGER PRIMARY KEY, send_count INTEGER);
CREATE TABLE message_system (message_row_id INTEGER PRIMARY KEY, action_type INTEGER NOT NULL);
CREATE TABLE message_system_group (message_row_id INTEGER PRIMARY KEY, is_me_joined INTEGER);
CREATE TABLE message_system_value_change (message_row_id INTEGER PRIMARY KEY, old_data TEXT);
CREATE TABLE message_system_number_change (message_row_id INTEGER PRIMARY KEY, old_jid_row_id INTEGER, new_jid_row_id INTEGER);
CREATE TABLE message_system_device_change (message_row_id INTEGER PRIMARY KEY, device_added_count INTEGER, device_removed_count INTEGER);
CREATE TABLE message_system_initial_privacy_provider(message_row_id INTEGER PRIMARY KEY, privacy_provider INTEGER NOT NULL DEFAULT 0, verified_biz_name TEXT, biz_state_id INTEGER);
CREATE TABLE message_system_photo_change (message_row_id INTEGER PRIMARY KEY, new_photo_id TEXT, old_photo BLOB, new_photo BLOB);
CREATE TABLE message_system_chat_participant (message_row_id INTEGER, user_jid_row_id INTEGER);
CREATE INDEX message_system_chat_participant_index ON message_system_chat_participant (message_row_id);
CREATE TABLE message_system_business_state (message_row_id INTEGER PRIMARY KEY, privacy_message_type INTEGER NOT NULL, business_name TEXT);
CREATE TABLE message_system_block_contact (message_row_id INTEGER PRIMARY KEY, is_blocked INTEGER);
CREATE TABLE message_system_linked_group_call(message_row_id INTEGER PRIMARY KEY, call_id TEXT, is_video_call INTEGER);
CREATE TABLE message_ui_elements(_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, message_row_id INTEGER NOT NULL, element_type INTEGER, element_content TEXT);
CREATE TABLE message_quoted_ui_elements(message_row_id INTEGER NOT NULL PRIMARY KEY, element_type INTEGER, element_content TEXT);
CREATE TABLE message_ui_elements_reply (message_row_id INTEGER PRIMARY KEY, element_type INTEGER,reply_values TEXT,reply_description TEXT );
CREATE TABLE message_quoted_ui_elements_reply (message_row_id INTEGER PRIMARY KEY, element_type INTEGER,reply_values TEXT,reply_description TEXT );
CREATE TABLE message_quoted_ui_elements_reply_legacy (message_row_id INTEGER PRIMARY KEY, element_type INTEGER,reply_values TEXT,reply_description TEXT );
CREATE TABLE messages_links (_id INTEGER PRIMARY KEY AUTOINCREMENT, key_remote_jid TEXT, message_row_id INTEGER, link_index INTEGER);
CREATE TABLE user_device_info (user_jid_row_id INTEGER PRIMARY KEY, raw_id INTEGER NOT NULL, timestamp INTEGER NOT NULL);
CREATE UNIQUE INDEX user_device_info_index ON user_device_info (user_jid_row_id);
CREATE TABLE message_privacy_state (message_row_id INTEGER NOT NULL PRIMARY KEY, host_storage INTEGER, actual_actors INTEGER, privacy_mode_ts INTEGER NOT NULL, business_name TEXT);
CREATE TABLE played_self_receipt(message_row_id INTEGER PRIMARY KEY,to_jid_row_id INTEGER NOT NULL,participant_jid_row_id INTEGER,message_id TEXT NOT NULL);
CREATE TABLE message_payment_invite (message_row_id INTEGER PRIMARY KEY, service INTEGER, expiration_timestamp INTEGER);
CREATE TABLE message_quoted_payment_invite (message_row_id INTEGER PRIMARY KEY, service INTEGER, expiration_timestamp INTEGER);
CREATE TABLE messages_quotes_payment_invite_legacy (message_row_id INTEGER PRIMARY KEY, service INTEGER, expiration_timestamp INTEGER);
CREATE TABLE message_system_payment_invite_setup(message_row_id INTEGER PRIMARY KEY, service INTEGER, invite_used INTEGER);
CREATE TABLE conversion_tuples (jid_row_id INTEGER PRIMARY KEY, data TEXT, source TEXT, biz_count INTEGER, has_user_sent_last_message BOOLEAN, last_interaction INTEGER);
CREATE TABLE labels (_id INTEGER PRIMARY KEY AUTOINCREMENT, label_name TEXT, predefined_id INTEGER, color_id INTEGER);
CREATE UNIQUE INDEX labels_index ON labels (label_name);
CREATE TABLE labeled_jid (_id INTEGER PRIMARY KEY AUTOINCREMENT, label_id INTEGER NOT NULL, jid_row_id INTEGER NOT NULL);
CREATE UNIQUE INDEX labeled_jid_index ON labeled_jid (label_id, jid_row_id);
CREATE TABLE labeled_jids (_id INTEGER PRIMARY KEY AUTOINCREMENT, label_id INTEGER NOT NULL, jid TEXT);
CREATE UNIQUE INDEX labeled_jids_index ON labeled_jids (label_id, jid);
CREATE VIRTUAL TABLE labeled_messages_fts USING FTS3()
/* labeled_messages_fts(content) */;
CREATE TABLE IF NOT EXISTS 'labeled_messages_fts_content'(docid INTEGER PRIMARY KEY, 'c0content');
CREATE TABLE IF NOT EXISTS 'labeled_messages_fts_segments'(blockid INTEGER PRIMARY KEY, block BLOB);
CREATE TABLE IF NOT EXISTS 'labeled_messages_fts_segdir'(level INTEGER,idx INTEGER,start_block INTEGER,leaves_end_block INTEGER,end_block INTEGER,root BLOB,PRIMARY KEY(level, idx));
CREATE TABLE labeled_messages(_id INTEGER PRIMARY KEY AUTOINCREMENT,label_id INTEGER NOT NULL,message_row_id INTEGER NOT NULL);
CREATE UNIQUE INDEX labeled_messages_index ON labeled_messages (label_id, message_row_id);
CREATE TABLE away_messages (_id INTEGER PRIMARY KEY AUTOINCREMENT, jid TEXT UNIQUE NOT NULL);
CREATE TABLE quick_replies (_id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT UNIQUE NOT NULL, content TEXT NOT NULL);
CREATE TABLE quick_reply_usage (_id INTEGER PRIMARY KEY AUTOINCREMENT, quick_reply_id TEXT NOT NULL, usage_date DATE, usage_count INTEGER);
CREATE TABLE quick_reply_keywords (_id INTEGER PRIMARY KEY AUTOINCREMENT, quick_reply_id TEXT NOT NULL, keyword_id TEXT NOT NULL);
CREATE TABLE keywords (_id INTEGER PRIMARY KEY AUTOINCREMENT, keyword TEXT UNIQUE NOT NULL);
CREATE TABLE quick_reply_attachments(_id INTEGER PRIMARY KEY AUTOINCREMENT, quick_reply_id TEXT NOT NULL, uri TEXT NOT NULL, caption TEXT, media_type INTEGER);
CREATE INDEX attachments_quick_reply_id_index on quick_reply_attachments (quick_reply_id);
CREATE TABLE message_invoice (message_row_id INTEGER PRIMARY KEY,wa_invoice_id TEXT NOT NULL,amount TEXT NOT NULL,note TEXT NOT NULL,token TEXT,sender_jid_row_id INTEGER,receiver_jid_row_id INTEGER,status INTEGER,status_ts INTEGER,creation_ts INTEGER,attachment_type INTEGER,attachment_mimetype TEXT,attachment_media_key BLOB,attachment_media_key_ts INTEGER,attachment_file_sha256 BLOB,attachment_file_enc_sha256 BLOB,attachment_direct_path TEXT,attachment_jpeg_thumbnail BLOB,metadata TEXT);
CREATE TABLE message_quote_invoice (message_row_id INTEGER PRIMARY KEY,amount TEXT NOT NULL,note TEXT NOT NULL,status INTEGER,attachment_jpeg_thumbnail BLOB);
CREATE TABLE invoice_transactions (message_row_id INTEGER PRIMARY KEY,pay_transaction_id INTEGER);
CREATE TABLE message_view_once_media (message_row_id INTEGER PRIMARY KEY, state INTEGER NOT NULL );
CREATE INDEX message_view_once_media_state_index ON message_view_once_media (state);
CREATE TABLE message_ephemeral(message_row_id INTEGER PRIMARY KEY, duration INTEGER NOT NULL, expire_timestamp INTEGER NOT NULL);
CREATE INDEX message_ephemeral_expire_timestamp_index on message_ephemeral(expire_timestamp);
CREATE TABLE message_ephemeral_setting (message_row_id INTEGER PRIMARY KEY, setting_duration INTEGER, setting_reason INTEGER, user_jid_row_id_csv TEXT);
CREATE TABLE message_system_ephemeral_setting_not_applied(message_row_id INTEGER PRIMARY KEY, setting_duration INTEGER);
CREATE TABLE message_broadcast_ephemeral (message_row_id INTEGER PRIMARY KEY, shared_secret BLOB NOT NULL );
CREATE TABLE message_status_psa_campaign (message_row_id INTEGER PRIMARY KEY, campaign_id TEXT , duration INTEGER, first_seen_timestamp INTEGER);
CREATE VIEW legacy_available_messages_view AS
 SELECT messages.*, chat._id AS chat_row_id,expire_timestamp
 FROM messages AS messages
    JOIN
        jid AS jid
    ON
        jid.raw_string = messages.key_remote_jid
    JOIN
        chat AS chat
    ON
        chat.jid_row_id = jid._id
    LEFT JOIN
        deleted_chat_job as job
    ON 
        job.chat_row_id = chat._id    LEFT JOIN
        message_ephemeral AS message_ephemeral
    ON messages._id = message_ephemeral.message_row_id
 WHERE IFNULL(NOT((IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_starred_message_row_id, 1)) OR ( (job.deleted_message_categories IS NOT NULL) AND   (job.deleted_message_categories LIKE '%"' || messages.media_wa_type || '"%') AND   ( (IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_categories_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_categories_starred_message_row_id, 1)) ))), 0)
/* legacy_available_messages_view(_id,key_remote_jid,key_from_me,key_id,status,needs_push,data,timestamp,media_url,media_mime_type,media_wa_type,media_size,media_name,media_caption,media_hash,media_duration,origin,latitude,longitude,thumb_image,remote_resource,received_timestamp,send_timestamp,receipt_server_timestamp,receipt_device_timestamp,read_device_timestamp,played_device_timestamp,raw_data,recipient_count,participant_hash,starred,quoted_row_id,mentioned_jids,multicast_id,edit_version,media_enc_hash,payment_transaction_id,forwarded,preview_type,send_count,lookup_tables,future_message_type,chat_row_id,expire_timestamp) */;
CREATE VIEW message_view AS SELECT messages._id AS _id, messages._id AS sort_id, chat._id AS chat_row_id, key_from_me AS from_me, key_id, -1 AS sender_jid_row_id, remote_resource AS sender_jid_raw_string, status, needs_push AS broadcast, recipient_count, participant_hash, forwarded AS origination_flags, origin, timestamp, received_timestamp, receipt_server_timestamp, CAST (CASE WHEN (messages.media_wa_type = 0 AND messages.status=6) THEN 7 ELSE messages.media_wa_type END AS INTEGER) AS message_type, '' as text_data, starred, lookup_tables, data, media_url, media_mime_type, media_size, media_name, media_caption, media_hash, media_duration, latitude, longitude, thumb_image, raw_data, quoted_row_id, mentioned_jids, multicast_id, edit_version, media_enc_hash, payment_transaction_id, preview_type, receipt_device_timestamp, read_device_timestamp, played_device_timestamp, future_message_type, 1 AS table_version FROM messages JOIN jid AS chat_jid ON messages.key_remote_jid= chat_jid.raw_string JOIN chat AS chat ON chat.jid_row_id = chat_jid._id
/* message_view(_id,sort_id,chat_row_id,from_me,key_id,sender_jid_row_id,sender_jid_raw_string,status,broadcast,recipient_count,participant_hash,origination_flags,origin,timestamp,received_timestamp,receipt_server_timestamp,message_type,text_data,starred,lookup_tables,data,media_url,media_mime_type,media_size,media_name,media_caption,media_hash,media_duration,latitude,longitude,thumb_image,raw_data,quoted_row_id,mentioned_jids,multicast_id,edit_version,media_enc_hash,payment_transaction_id,preview_type,receipt_device_timestamp,read_device_timestamp,played_device_timestamp,future_message_type,table_version) */;
CREATE VIEW available_message_view AS SELECT messages._id AS _id, messages._id AS sort_id, chat._id AS chat_row_id, key_from_me AS from_me, key_id, -1 AS sender_jid_row_id, remote_resource AS sender_jid_raw_string, status, needs_push AS broadcast, recipient_count, participant_hash, forwarded AS origination_flags, origin, timestamp, received_timestamp, receipt_server_timestamp, CAST (CASE WHEN (messages.media_wa_type = 0 AND messages.status=6) THEN 7 ELSE messages.media_wa_type END AS INTEGER) AS message_type, '' as text_data, starred, lookup_tables, data, media_url, media_mime_type, media_size, media_name, media_caption, media_hash, media_duration, latitude, longitude, thumb_image, raw_data, quoted_row_id, mentioned_jids, multicast_id, edit_version, media_enc_hash, payment_transaction_id, preview_type, receipt_device_timestamp, read_device_timestamp, played_device_timestamp, future_message_type, 1 AS table_version, expire_timestamp FROM messages JOIN jid AS chat_jid ON messages.key_remote_jid= chat_jid.raw_string JOIN chat AS chat ON chat.jid_row_id = chat_jid._id LEFT JOIN message_ephemeral AS message_ephemeral ON messages._id = message_ephemeral.message_row_id LEFT JOIN deleted_chat_job AS job ON job.chat_row_id = chat._id WHERE IFNULL(NOT((IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_starred_message_row_id, 1)) OR ( (job.deleted_message_categories IS NOT NULL) AND   (job.deleted_message_categories LIKE '%"' || messages.media_wa_type || '"%') AND   ( (IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_categories_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_categories_starred_message_row_id, 1)) ))), 0)
/* available_message_view(_id,sort_id,chat_row_id,from_me,key_id,sender_jid_row_id,sender_jid_raw_string,status,broadcast,recipient_count,participant_hash,origination_flags,origin,timestamp,received_timestamp,receipt_server_timestamp,message_type,text_data,starred,lookup_tables,data,media_url,media_mime_type,media_size,media_name,media_caption,media_hash,media_duration,latitude,longitude,thumb_image,raw_data,quoted_row_id,mentioned_jids,multicast_id,edit_version,media_enc_hash,payment_transaction_id,preview_type,receipt_device_timestamp,read_device_timestamp,played_device_timestamp,future_message_type,table_version,expire_timestamp) */;
CREATE VIEW deleted_messages_view AS 
 SELECT messages._id AS _id, messages._id AS sort_id, chat._id AS chat_row_id, key_from_me AS from_me, key_id, -1 AS sender_jid_row_id, remote_resource AS sender_jid_raw_string, status, needs_push AS broadcast, recipient_count, participant_hash, forwarded AS origination_flags, origin, timestamp, received_timestamp, receipt_server_timestamp, CAST (CASE WHEN (messages.media_wa_type = 0 AND messages.status=6) THEN 7 ELSE messages.media_wa_type END AS INTEGER) AS message_type, '' as text_data, starred, lookup_tables, data, media_url, media_mime_type, media_size, media_name, media_caption, media_hash, media_duration, latitude, longitude, thumb_image, raw_data, quoted_row_id, mentioned_jids, multicast_id, edit_version, media_enc_hash, payment_transaction_id, preview_type, receipt_device_timestamp, read_device_timestamp, played_device_timestamp, future_message_type, 1 AS table_version, 
 (( ((job.deleted_messages_remove_files == 1) AND ((IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_starred_message_row_id, 1)))) OR   ((job.deleted_categories_remove_files == 1) AND ( (job.deleted_message_categories IS NOT NULL) AND   (job.deleted_message_categories LIKE '%"' || messages.media_wa_type || '"%') AND   ( (IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_categories_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_categories_starred_message_row_id, 1)) ))))) as remove_files 
 FROM deleted_chat_job AS job
 JOIN chat AS chat ON job.chat_row_id = chat._id JOIN jid AS chat_jid ON chat.jid_row_id = chat_jid._id
 LEFT JOIN messages AS messages ON messages.key_remote_jid = chat_jid.raw_string
 WHERE IFNULL((IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_starred_message_row_id, 1)) OR ( (job.deleted_message_categories IS NOT NULL) AND   (job.deleted_message_categories LIKE '%"' || messages.media_wa_type || '"%') AND   ( (IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_categories_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_categories_starred_message_row_id, 1)) )), 0) 
 ORDER BY messages._id
/* deleted_messages_view(_id,sort_id,chat_row_id,from_me,key_id,sender_jid_row_id,sender_jid_raw_string,status,broadcast,recipient_count,participant_hash,origination_flags,origin,timestamp,received_timestamp,receipt_server_timestamp,message_type,text_data,starred,lookup_tables,data,media_url,media_mime_type,media_size,media_name,media_caption,media_hash,media_duration,latitude,longitude,thumb_image,raw_data,quoted_row_id,mentioned_jids,multicast_id,edit_version,media_enc_hash,payment_transaction_id,preview_type,receipt_device_timestamp,read_device_timestamp,played_device_timestamp,future_message_type,table_version,remove_files) */;
CREATE VIEW deleted_messages_ids_view AS 
 SELECT messages._id AS _id, CAST (CASE WHEN (messages.media_wa_type = 0 AND messages.status=6) THEN 7 ELSE messages.media_wa_type END AS INTEGER) AS message_type, job.chat_row_id AS chat_row_id FROM deleted_chat_job AS job
 JOIN chat AS chat ON job.chat_row_id = chat._id JOIN jid AS chat_jid ON chat.jid_row_id = chat_jid._id
 LEFT JOIN messages AS messages ON messages.key_remote_jid = chat_jid.raw_string
 WHERE IFNULL((IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_starred_message_row_id, 1)) OR ( (job.deleted_message_categories IS NOT NULL) AND   (job.deleted_message_categories LIKE '%"' || messages.media_wa_type || '"%') AND   ( (IFNULL(messages.starred, 0) = 0 AND messages._id <= IFNULL(job.deleted_categories_message_row_id, 1)) OR (IFNULL(messages.starred, 0) = 1 AND messages._id <= IFNULL(job.deleted_categories_starred_message_row_id, 1)) )), 0)
/* deleted_messages_ids_view(_id,message_type,chat_row_id) */;
CREATE VIEW chat_view AS SELECT c._id AS _id, j.raw_string AS raw_string_jid, hidden, subject, created_timestamp, display_message_row_id, last_message_row_id, last_read_message_row_id, last_read_receipt_sent_message_row_id, last_important_message_row_id, archived, sort_timestamp, mod_tag, gen, spam_detection, unseen_earliest_message_received_time, unseen_message_count, unseen_missed_calls_count, unseen_row_count, plaintext_disabled, vcard_ui_dismissed, change_number_notified_message_row_id, show_group_description, ephemeral_expiration, last_read_ephemeral_message_row_id, ephemeral_setting_timestamp, ephemeral_disappearing_messages_initiator, unseen_important_message_count FROM chat c LEFT JOIN jid j ON c.jid_row_id=j._id
/* chat_view(_id,raw_string_jid,hidden,subject,created_timestamp,display_message_row_id,last_message_row_id,last_read_message_row_id,last_read_receipt_sent_message_row_id,last_important_message_row_id,archived,sort_timestamp,mod_tag,gen,spam_detection,unseen_earliest_message_received_time,unseen_message_count,unseen_missed_calls_count,unseen_row_count,plaintext_disabled,vcard_ui_dismissed,change_number_notified_message_row_id,show_group_description,ephemeral_expiration,last_read_ephemeral_message_row_id,ephemeral_setting_timestamp,ephemeral_disappearing_messages_initiator,unseen_important_message_count) */;
CREATE TRIGGER call_log_bd_for_call_log_participant_v2_trigger BEFORE DELETE ON call_log BEGIN DELETE FROM call_log_participant_v2 WHERE call_log_row_id=old._id; END;
CREATE TRIGGER call_log_bd_for_joinable_call_log_trigger BEFORE DELETE ON call_log BEGIN DELETE FROM joinable_call_log WHERE call_log_row_id=old._id; END;
CREATE TRIGGER chat_bd_for_message_link_trigger BEFORE DELETE ON chat BEGIN DELETE FROM message_link WHERE chat_row_id=old._id; END;
CREATE TRIGGER group_participant_user_bd_for_group_participant_device_trigger BEFORE DELETE ON group_participant_user BEGIN DELETE FROM group_participant_device WHERE group_participant_row_id=old._id; END;
CREATE TRIGGER labels_bd_for_labeled_jid_trigger BEFORE DELETE ON labels BEGIN DELETE FROM labeled_jid WHERE label_id=old._id; END;
CREATE TRIGGER labels_bd_for_labeled_jids_trigger BEFORE DELETE ON labels BEGIN DELETE FROM labeled_jids WHERE label_id=old._id; END;
CREATE TRIGGER labels_bd_for_labeled_messages_trigger BEFORE DELETE ON labels BEGIN DELETE FROM labeled_messages WHERE label_id=old._id; END;
CREATE TRIGGER message_media_bd_for_message_media_interactive_annotation_trigger BEFORE DELETE ON message_media BEGIN DELETE FROM message_media_interactive_annotation WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_media_bd_for_message_media_vcard_count_trigger BEFORE DELETE ON message_media BEGIN DELETE FROM message_media_vcard_count WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_media_interactive_annotation_bd_for_message_media_interactive_annotation_vertex_trigger BEFORE DELETE ON message_media_interactive_annotation BEGIN DELETE FROM message_media_interactive_annotation_vertex WHERE message_media_interactive_annotation_row_id=old._id; END;
CREATE TRIGGER message_quoted_bd_for_message_quoted_group_invite_trigger BEFORE DELETE ON message_quoted BEGIN DELETE FROM message_quoted_group_invite WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_quoted_bd_for_message_quoted_location_trigger BEFORE DELETE ON message_quoted BEGIN DELETE FROM message_quoted_location WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_quoted_bd_for_message_quoted_media_trigger BEFORE DELETE ON message_quoted BEGIN DELETE FROM message_quoted_media WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_quoted_bd_for_message_quoted_mentions_trigger BEFORE DELETE ON message_quoted BEGIN DELETE FROM message_quoted_mentions WHERE message_row_id = old.message_row_id; END;
CREATE TRIGGER message_quoted_bd_for_message_quoted_payment_invite_trigger BEFORE DELETE ON message_quoted BEGIN DELETE FROM message_quoted_payment_invite WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_quoted_bd_for_message_quoted_text_trigger BEFORE DELETE ON message_quoted BEGIN DELETE FROM message_quoted_text WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_quoted_bd_for_message_quoted_vcard_trigger BEFORE DELETE ON message_quoted BEGIN DELETE FROM message_quoted_vcard WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_payment_status_update_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_payment_status_update WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_payment_transaction_reminder_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_payment_transaction_reminder WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_payment_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_payment WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_block_contact_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_block_contact WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_business_state_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_business_state WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_chat_participant_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_chat_participant WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_device_change_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_device_change WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_ephemeral_setting_not_applied_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_ephemeral_setting_not_applied WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_group_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_group WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_initial_privacy_provider_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_initial_privacy_provider WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_linked_group_call_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_linked_group_call WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_number_change_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_number_change WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_payment_invite_setup_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_payment_invite_setup WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_photo_change_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_photo_change WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_system_bd_for_message_system_value_change_trigger BEFORE DELETE ON message_system BEGIN DELETE FROM message_system_value_change WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_template_bd_for_message_template_button_trigger BEFORE DELETE ON message_template BEGIN DELETE FROM message_template_button WHERE message_row_id=old.message_row_id; END;
CREATE TRIGGER message_vcard_bd_for_message_vcard_jid_trigger BEFORE DELETE ON message_vcard BEGIN DELETE FROM message_vcard_jid WHERE vcard_row_id=old._id; END;
CREATE TRIGGER message_vcard_bd_for_messages_vcards_jids_trigger BEFORE DELETE ON message_vcard BEGIN DELETE FROM messages_vcards_jids WHERE message_row_id = old.message_row_id; END;
CREATE TRIGGER messages_bd_for_audio_data_trigger BEFORE DELETE ON messages BEGIN DELETE FROM audio_data WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_labeled_messages_fts_trigger BEFORE DELETE ON messages BEGIN DELETE FROM labeled_messages_fts WHERE docid=old._id; END;
CREATE TRIGGER messages_bd_for_labeled_messages_trigger BEFORE DELETE ON messages BEGIN DELETE FROM labeled_messages WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_broadcast_ephemeral_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_broadcast_ephemeral WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_ephemeral_setting_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_ephemeral_setting WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_ephemeral_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_ephemeral WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_external_ad_content_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_external_ad_content WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_forwarded_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_forwarded WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_ftsv2_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_ftsv2 WHERE docid=old._id; END;
CREATE TRIGGER messages_bd_for_message_future_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_future WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_group_invite_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_group_invite WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_link_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_link WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_location_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_location WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_media_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_media WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_mentions_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_mentions WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_order_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_order WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_payment_invite_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_payment_invite WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_privacy_state_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_privacy_state WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_product_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_product WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_quoted_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_quoted WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_rating_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_rating WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_revoked_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_revoked WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_send_count_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_send_count WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_status_psa_campaign_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_status_psa_campaign WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_streaming_sidecar_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_streaming_sidecar WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_system_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_system WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_template_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_template WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_text_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_text WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_thumbnail_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_thumbnail WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message WHERE _id=old._id; END;
CREATE TRIGGER messages_bd_for_message_ui_elements_reply_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_ui_elements_reply WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_ui_elements_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_ui_elements WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_vcard_jid_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_vcard_jid WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_vcard_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_vcard WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_message_view_once_media_trigger BEFORE DELETE ON messages BEGIN DELETE FROM message_view_once_media WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_messages_fts_trigger BEFORE DELETE ON messages BEGIN DELETE FROM messages_fts WHERE docid=old._id; END;
CREATE TRIGGER messages_bd_for_messages_hydrated_four_row_template_trigger BEFORE DELETE ON messages BEGIN DELETE FROM messages_hydrated_four_row_template WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_messages_links_trigger BEFORE DELETE ON messages BEGIN DELETE FROM messages_links WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_messages_quotes_trigger BEFORE DELETE ON messages BEGIN DELETE FROM messages_quotes WHERE _id=old.quoted_row_id; END;
CREATE TRIGGER messages_bd_for_messages_vcards_trigger BEFORE DELETE ON messages BEGIN DELETE FROM messages_vcards WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_missed_call_logs_trigger BEFORE DELETE ON messages BEGIN DELETE FROM missed_call_logs WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_mms_thumbnail_metadata_trigger BEFORE DELETE ON messages BEGIN DELETE FROM mms_thumbnail_metadata WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_played_self_receipt_trigger BEFORE DELETE ON messages BEGIN DELETE FROM played_self_receipt WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_receipt_device_trigger BEFORE DELETE ON messages BEGIN DELETE FROM receipt_device WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_receipt_user_trigger BEFORE DELETE ON messages BEGIN DELETE FROM receipt_user WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_bd_for_receipts_trigger BEFORE DELETE ON messages BEGIN DELETE FROM receipts WHERE key_remote_jid=old.key_remote_jid AND key_id=old.key_id; END;
CREATE TRIGGER messages_quotes_bd_for_message_quoted_group_invite_legacy_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM message_quoted_group_invite_legacy WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_quotes_bd_for_message_quoted_order_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM message_quoted_order WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_quotes_bd_for_message_quoted_product_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM message_quoted_product WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_quotes_bd_for_message_quoted_ui_elements_reply_legacy_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM message_quoted_ui_elements_reply_legacy WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_quotes_bd_for_message_quoted_ui_elements_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM message_quoted_ui_elements WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_quotes_bd_for_message_template_quoted_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM message_template_quoted WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_quotes_bd_for_messages_quotes_payment_invite_legacy_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM messages_quotes_payment_invite_legacy WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_quotes_bd_for_quoted_message_order_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM quoted_message_order WHERE message_row_id=old._id; END;
CREATE TRIGGER messages_quotes_bd_for_quoted_message_product_trigger BEFORE DELETE ON messages_quotes BEGIN DELETE FROM quoted_message_product WHERE message_row_id=old._id; END;
CREATE TRIGGER missed_call_logs_bd_for_missed_call_log_participant_trigger BEFORE DELETE ON missed_call_logs BEGIN DELETE FROM missed_call_log_participant WHERE call_logs_row_id=old._id; END;
CREATE TRIGGER payment_background_bd_for_payment_background_order_trigger BEFORE DELETE ON payment_background BEGIN DELETE FROM payment_background_order WHERE background_id=old.background_id; END;
CREATE TRIGGER quick_replies_bd_for_quick_reply_attachments_trigger BEFORE DELETE ON quick_replies BEGIN DELETE FROM quick_reply_attachments WHERE quick_reply_id=old._id; END;
CREATE TRIGGER quick_replies_bd_for_quick_reply_keywords_trigger BEFORE DELETE ON quick_replies BEGIN DELETE FROM quick_reply_keywords WHERE quick_reply_id=old._id; END;
CREATE TRIGGER quick_replies_bd_for_quick_reply_usage_trigger BEFORE DELETE ON quick_replies BEGIN DELETE FROM quick_reply_usage WHERE quick_reply_id=old._id; END;
