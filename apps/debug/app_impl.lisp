(import "././login/env.inc")
(import "gui/lisp.inc")

;our UI widgets and events
(import "./widgets.inc")

(structure +debug 0
	(netid reply origin)
	(int type)
	(offset data))

(enums +debug_rec 0
	(enum buf state reply_id))

(enums +select 0
	(enum main service tip exit))

(defq +rate_exit 1000000)

(defun vdu-print (vdu buf s)
	(defq ch (const (dec +height)))
	(. buf :paste s)
	(bind '(w h) (. buf :get_size))
	(when (> h ch)
		(.-> buf (:set_cursor 0 0) (:cut 0 (- h ch)))
		(. buf :set_cursor 0 ch))
	(. buf :clear_undo)
	(if vdu (. buf :vdu_load vdu 0 0 :text)))

(defun set-slider-values ()
	(defq val (get :value *hslider*) mho (max 0 (dec (length buf_list))))
	(def *hslider* :maximum mho :portion 1 :value (min val mho))
	(. *hslider* :dirty))

(defun step (_)
	(when (elem-get +debug_rec_reply_id _)
		(mail-send (elem-get +debug_rec_reply_id _)
			(str (elem-get +debug_rec_state _)))))

(defun play (_)
	(defq state (elem-get +debug_rec_state _))
	(elem-set +debug_rec_state _ :play)
	(if (eql state :paused) (step _)))

(defun forward (_)
	(defq state (elem-get +debug_rec_state _))
	(elem-set +debug_rec_state _ :forward)
	(if (eql state :paused) (step _)))

(defun pause (_)
	(elem-set +debug_rec_state _ :paused))

(defun reset (&optional _)
	(setd _ -1)
	(if (<= 0 _ (dec (length buf_list)))
		(progn
			(def *hslider* :value _)
			(setq selected_index _)
			(vdu-print *vdu* (elem-get +debug_rec_buf (elem-get selected_index buf_list)) ""))
		(progn
			(clear buf_list)
			(clear buf_keys)
			(setq selected_index :nil)
			(. *vdu* :load '(
				{ChrysaLisp Debug 0.5}
				{Toolbar1 buttons act on a single task.}
				{Toolbar2 buttons act on all tasks.}
				{Slider to switch between tasks.}
				{}
				{In Lisp files:}
				{}
				{add (import "lib/debug/debug.inc")}
				{to debug all functions/methods.}
				{}
				{Use:}
				{}
				{(debug-brk name condtion)}
				{}
				{as a conditional breakpoint.}) 0 0 0 1000)))
	(set-slider-values))

;import actions, bindings and app ui classes
(import "./actions.inc")

(defun dispatch-action (&rest action)
	(catch (eval action) (progn (prin _) (print) :t)))

(defun main ()
	(defq select (alloc-select +select_size) syntax (Syntax)
		buf_keys (list) buf_list (list) selected_index :nil *running* :t
		entry (mail-declare (elem-get +select_service select) "*Debug" "Debug Service 0.4"))
	(def *window* :tip_mbox (elem-get +select_tip select))
	(bind '(x y w h) (apply view-locate (. *window* :pref_size)))
	(gui-add-front (. *window* :change x y w h))
	(reset)
	(while *running*
		(defq *msg* (mail-read (elem-get (defq idx (mail-select select)) select)))
		(cond
			((= idx +select_tip)
				;tip time mail
				(if (defq view (. *window* :find_id (getf *msg* +mail_timeout_id)))
					(. view :show_tip)))
			;new debug msg
			((= idx +select_service)
				(defq reply_id (getf *msg* +debug_reply)
					key (sym (getf *msg* +debug_origin))
					type (getf *msg* +debug_type)
					data (slice +debug_data -1 *msg*)
					index (find-rev key buf_keys))
				(unless index
					(push buf_keys key)
					(push buf_list (list (Buffer :t syntax) :paused :nil))
					(reset (setq index (dec (length buf_list)))))
				(defq buf_rec (elem-get index buf_list)
					buf (elem-get +debug_rec_buf buf_rec)
					state (elem-get +debug_rec_state buf_rec))
				(cond
					((> type 0)
						(vdu-print (if (= index selected_index) *vdu*) buf data)
						(pause (elem-get index buf_list)))
					((not (eql state :forward))
						(vdu-print (if (= index selected_index) *vdu*) buf data)))
				(if (or (eql (defq state (elem-get +debug_rec_state buf_rec)) :play)
						(eql state :forward))
					(mail-send reply_id (str state))
					(elem-set +debug_rec_reply_id buf_rec reply_id)))
			((= idx +select_exit)
				;exit mail
				(setq *running* :nil))
			;must be GUI event
			((defq id (getf *msg* +ev_msg_target_id) action (. *event_map* :find id))
				;call bound event action
				(dispatch-action action))
			((and (not (Textfield? (. *window* :find_id id)))
					(= (getf *msg* +ev_msg_type) +ev_type_key_down)
					(> (getf *msg* +ev_msg_key_scode) 0))
				;key event
				(defq key (getf *msg* +ev_msg_key_key)
					mod (getf *msg* +ev_msg_key_mod))
				(cond
					((/= 0 (logand mod (const
							(+ +ev_key_mod_control +ev_key_mod_alt +ev_key_mod_meta))))
						;call bound control/command key action
						(when (defq action (. *key_map_control* :find key))
							(dispatch-action action)))
					((/= 0 (logand mod +ev_key_mod_shift))
						;call bound shift key action, else insert
						(cond
							((defq action (. *key_map_shift* :find key))
								(dispatch-action action))
							((<= +char_space key +char_tilde)
								;insert char etc ...
								(char key))))
					((defq action (. *key_map* :find key))
						;call bound key action
						(dispatch-action action))
					((<= +char_space key +char_tilde)
						;insert char etc ...
						(char key))))
			;otherwise
			(:t (. *window* :event *msg*))))
	(free-select select)
	(gui-sub *window*)
	;restart any paused debug ipc
	(each play buf_list))
