(import "././login/env.inc")
(import "gui/lisp.inc")
(import "lib/text/files.inc")

(enums +event 0
	(enum close)
	(enum file_button tree_button)
	(enum exts_action ok_action))

(ui-window *window* :nil
	(ui-flow _ (:flow_flags +flow_down_fill)
		(ui-title-bar *window_title* "" (0xea19) +event_close)
		(ui-flow _ (:flow_flags +flow_right_fill)
			(ui-buttons (0xe93a) +event_ok_action (:font *env_toolbar_font*))
			(ui-label _ (:text "Filename:"))
			(. (ui-textfield filename (:clear_text "" :hint_text "new filename" :color +argb_white))
				:connect +event_ok_action))
		(ui-flow _ (:flow_flags +flow_right_fill)
			(ui-label _ (:text "Filter:"))
			(. (ui-textfield ext_filter (:clear_text "" :hint_text "suffix" :color +argb_white))
				:connect +event_exts_action))
		(ui-flow _ (:flow_flags +flow_right_fill :font *env_terminal_font* :color +argb_white :border 1)
			(ui-flow _ (:flow_flags +flow_down_fill)
				(ui-label _ (:text "Folders" :font *env_window_font*))
				(ui-scroll tree_scroll +scroll_flag_vertical :nil
					(ui-flow tree_flow (:flow_flags +flow_down_fill :color +argb_white
						:min_width 256))))
			(ui-flow _ (:flow_flags +flow_down_fill)
				(ui-label _ (:text "Files" :font *env_window_font*))
				(ui-scroll files_scroll +scroll_flag_vertical :nil
					(ui-flow files_flow (:flow_flags +flow_down_fill :color +argb_white
						:min_width 256)))))))

(defun populate-files (files dir exts)
	;filter files and dirs to only those that match and are unique
	(if (= (length (setq exts (split exts ", "))) 0) (setq exts '("")))
	(defq dirs_with_exts (list) files_within_dir (list))
	(each (lambda (_)
			(defq i (inc (find-rev "/" _)) d (slice 0 i _) f (slice i -1 _))
			(if (notany (lambda (_) (eql d _)) dirs_with_exts)
				(push dirs_with_exts d))
			(if (eql dir d)
				(push files_within_dir f)))
		(filter (lambda (f) (some (lambda (ext) (ends-with ext f)) exts)) files))
	(setq dirs_with_exts (sort cmp dirs_with_exts))
	;populate tree and files flows
	(each (# (. %0 :sub)) tree_buttons)
	(each (# (. %0 :sub)) file_buttons)
	(clear tree_buttons file_buttons)
	(each (lambda (_)
		(def (defq b (Button)) :text _)
		(if (eql _ dir) (def b :color +argb_grey14))
		(. tree_flow :add_child (. b :connect +event_tree_button))
		(push tree_buttons b)) dirs_with_exts)
	(each (lambda (_)
		(def (defq b (Button)) :text _)
		(. files_flow :add_child (. b :connect +event_file_button))
		(push file_buttons b)) files_within_dir)
	;layout and size window
	(bind '(_ ch) (. tree_scroll :get_size))
	(bind '(w h) (. tree_flow :pref_size))
	(. tree_flow :set_size w h)
	(. tree_flow :layout)
	(def tree_scroll :min_width w :min_height (max ch 512))
	(bind '(w h) (. files_flow :pref_size))
	(. files_flow :set_size w h)
	(. files_flow :layout)
	(def files_scroll :min_width w :min_height (max ch 512))
	(. files_scroll :layout)
	(. tree_scroll :layout)
	(bind '(x y) (. *window* :get_pos))
	(bind '(w h) (. *window* :pref_size))
	(. *window* :change_dirty x y w h)
	(def tree_scroll :min_height 0)
	(def files_scroll :min_height 0))

(defun main ()
	;read paramaters from parent
	(bind '(reply_mbox title dir exts) (mail-read (task-netid)))
	(def *window_title* :text title)
	(def ext_filter :clear_text exts)
	(defq all_files (sort cmp (all-files dir '(""))) tree_buttons (list) file_buttons (list) current_dir (cat dir "/"))
	(populate-files all_files current_dir exts)
	(bind '(x y w h) (apply view-locate (. *window* :get_size)))
	(gui-add-front (. *window* :change x y w h))
	(while (cond
		((eql (defq msg (mail-read (task-netid))) "")
			:nil)
		((= (defq id (getf msg +ev_msg_target_id)) +event_close)
			(mail-send reply_mbox ""))
		((= id +event_ok_action)
			(mail-send reply_mbox (get :clear_text filename)))
		((= id +event_file_button)
			(defq old_filename (get :clear_text filename) new_filename (cat current_dir
				(get :text (. *window* :find_id (getf msg +ev_msg_action_source_id)))))
			(if (eql new_filename old_filename)
				(mail-send reply_mbox new_filename))
			(set filename :clear_text new_filename)
			(.-> filename :layout :dirty))
		((= id +event_tree_button)
			(setq current_dir (get :text (. *window* :find_id (getf msg +ev_msg_action_source_id))))
			(populate-files all_files current_dir exts))
		((= id +event_exts_action)
			(setq exts (get :clear_text ext_filter))
			(populate-files all_files current_dir exts))
		(:t (. *window* :event msg))))
	(gui-sub *window*))
