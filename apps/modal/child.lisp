(import "sys/lisp.inc")
(import "class/lisp.inc")
(import "gui/lisp.inc")

(structure '+event 0
	(byte 'click+))

(defmacro ui-margin (m e)
	`(ui-flow _ (:flow_flags +flow_right_fill+)
		(ui-label _ (:min_width ,m))
		,e))

(ui-window mywindow (:border 1)
	(ui-flow _ (:flow_flags +flow_down_fill+)
		(ui-label _ (:min_height 8))
		(ui-margin 32 (ui-label title (:text "" :font *env_title_font*)))
		(ui-label _ (:min_height 8))
		(ui-label label (:flow_flags +flow_flag_align_hcenter+ :text "message text" :font *env_body_font*))
		(ui-label _ (:text "" :font *env_body_font*))
		(ui-flow _ (:flow_flags +flow_flag_align_hcenter+)
		(ui-flow flow ()))))
		; (ui-label _ (:text "" :min_height 20))))

(defmacro gl (ls)
	`(reduce (lambda (x y) 
		(if (>= (length x) (length y)) x y)) ,ls))

(defun build-modal ()
	(set label :text (gl b_text))
	(bind '(bw bh) (. label :pref_size))
	(def (defq grid (Grid)) :grid_width (length b_text) :grid_height 1)
	(each (lambda (x)
		(def (defq f (Flow)) :flow_flags +flow_right_fill+)
		(def (defq l1 (Label)) :min_width 8 :border 0)
		(def (defq b (Button)) :flow_flags (logior +flow_flag_align_hcenter+ +flow_flag_align_vcenter+)
			:border 1 :text x :min_width (+ bw 24) :min_height (+ bh 8))
		(def (defq l2 (Label)) :min_width 8 :border 0)
		(. f :add_child l1)
		(. f :add_child (component-connect b +event_click+))
		(. f :add_child l2)
		(. f :layout)
		(. grid :add_child f)) b_text)
	; (. flow :layout)
	(. grid :layout)
	(. flow :add_child grid)
	(set title :text title_text)
	(set label :text label_text)
	; (bind '(x y w h) (apply view-locate (. flow :pref_size)))
	; (. flow :change x y w h)
	; (set title :text title_text)
	(bind '(x y w h) (apply view-locate (. mywindow :pref_size)))
	(. mywindow :change x y (+ 48 w) (+ 12 h)))

(defun main ()
	;read paramaters from parent
	(bind '(reply_mbox title_text label_text button_text) (mail-read (task-mailbox)))
	(bind '(x y w h) (apply view-locate (. mywindow :pref_size)))
	(defq b_text (split button_text ","))
	(gui-add (build-modal))

	(while (cond
		((eql (defq msg (mail-read (task-mailbox))) "")
			nil)
		((= (length button_text) 0) (task-sleep sleep_time) (mail-send "" reply_mbox))
		((= (defq id (get-long msg ev_msg_target_id)) +event_click+)
			(defq reply (get :text (. mywindow :find_id (get-long msg ev_msg_action_source_id))))
			(mail-send reply reply_mbox))
		(t (. mywindow :event msg))))
		(. mywindow :hide))