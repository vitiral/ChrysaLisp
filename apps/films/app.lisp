;imports
(import 'sys/lisp.inc)
(import 'class/lisp.inc)
(import 'gui/lisp.inc)

(structure 'event 0
	(byte 'close)
	(byte 'prev 'next))

(defq images '("apps/films/captive.flm" "apps/films/cradle.flm") index 0 canvas nil id t)

(ui-window window ()
	(ui-title-bar window_title "" (0xea19) (const event_close))
	(ui-tool-bar _ ()
		(ui-buttons (0xe91d 0xe91e) (const event_prev)))
	(ui-scroll image_scroll (logior scroll_flag_vertical scroll_flag_horizontal)))

(defun win-refresh (_)
	(bind '(w h) (view-pref-size (setq canvas (canvas-load (elem (setq index _) images) load_flag_film))))
	(def image_scroll 'min_width w 'min_height h)
	(def window_title 'text (elem _ images))
	(view-add-child image_scroll canvas)
	(view-layout window_title)
 	(bind '(x y) (view-get-pos window))
	(bind '(w h) (view-pref-size window))
 	(def image_scroll 'min_width 32 'min_height 32)
	(view-change-dirty window x y w h))

(defun-bind main ()
	(gui-add (apply view-change (cat (list window 320 256) (view-get-size (win-refresh index)))))
	(while id
		(task-sleep 40000)
		(canvas-swap (canvas-next-frame canvas))
		(while (mail-poll (array (task-mailbox)))
			(cond
				((= (setq id (get-long (defq msg (mail-read (task-mailbox))) ev_msg_target_id)) event_close)
					(setq id nil))
				((<= event_prev id event_next)
					(win-refresh (% (+ index (dec (* 2 (- id event_prev))) (length images)) (length images))))
				(t (view-event window msg)))))
	(view-hide window))
