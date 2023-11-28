(import "././login/env.inc")
(import "gui/lisp.inc")
(import "lib/anaphoric/anaphoric.inc")
(import "apps/minefield/board.inc")

(enums +event 0
	(enum close beginner intermediate expert click))

(ui-window *window* ()
	(ui-flow window_flow (:flow_flags +flow_down_fill)
		(ui-title-bar *window_title* "Minefield" (0xea19) +event_close)
		(ui-flow view (:flow_flags +flow_flag_align_hcenter)
			(ui-flow across (:flow_flags +flow_right_fill)
					(ui-label left_pad (:text "" :min_width 25))
					(ui-flow center (:flow_flags +flow_down_fill)
						(ui-label top_pad (:text "" :min_height 15))
						(ui-label game_title (:text "Minefield" :min_width 100 :font (create-font "fonts/OpenSans-Regular.ctf" 20) :flow_flags
								(logior +flow_flag_align_hcenter +flow_flag_align_vcenter)))
						(ui-label _ (:text "" :font *env_window_font*))
						(. (ui-button beginner (:text "Beginner" :min_width 125 :min_height 35)) :connect +event_beginner)
						(ui-label _ (:text ""))
						(. (ui-button medium (:text "Intermediate" :min_width 125 :min_height 35)) :connect +event_intermediate)
						(ui-label _ (:text ""))
						(. (ui-button expert (:text "Expert" :min_width 125 :min_height 35)) :connect +event_expert)
						(ui-label bottom_pad (:text "" :min_height 35)))
					(ui-label right_pad (:text "" :min_width 25))))
		(ui-label status_bar (:text " "))))

(defun clicked-blank (cell)
	(defq work (list cell))
	(while (defq cell (pop work))
		(unless (eql (elem-get cell game_map) "r")
			(elem-set cell game_map "r")
			(aeach (elem-get cell game_adj)
				(cond
					((= (elem-get it game_board) 0) (push work it))
					((< 0 (elem-get it game_board) 9) (elem-set it game_map "r"))))))
	(rebuild-board))

(defun clicked-flag (cell)
	(elem-set cell game_map "b")
	(rebuild-board))

(defun right-clicked-button (cell)
	(elem-set cell game_map "f")
	(rebuild-board)
	(is-game-over?))

(defun clicked-value (cell)
	(elem-set cell game_map "r")
	(rebuild-board))

(defun clicked-mine (cell)
	(elem-set cell game_map "r")
	(rebuild-board))

(defun is-game-over? (&optional lost)
	(defq message "")
	(cond
		(game_over (setq message "You Lost!"))
		((= (length (filter (# (or (eql %0 "f") (eql %0 "b"))) game_map)) (last difficulty))
			(setq message "You Won!" game_over :t))
		(:t :nil))
	(set status_bar :text message)
	(.-> status_bar :layout :dirty))

(defun colorize (value)
	(elem-get value '(+argb_black 0x000000ff 0x00006600 0x00ff0000 +argb_magenta
		+argb_black 0x00700000 +argb_grey1 0x0002bbdd +argb_black)))

(defun board-layout ((gw gh nm))
	(. across :sub)
	(setq game_grid (Grid))
	(defq gwh (* gw gh))
		; (ui-grid game_grid (:grid_width 1 :grid_height 5)
	(each (lambda (_)
		(. (defq mc (Button)) :connect (+ _ +event_click))
		(def mc :text "" :border 1 :flow_flags
			(logior +flow_flag_align_hcenter +flow_flag_align_vcenter) :min_width 32 :min_height 32)
		(. game_grid :add_child mc)) (range 0 gwh))
	(def game_grid :grid_width gw :grid_height gh :color (const *env_toolbar_col*) :font *env_window_font*)
	(bind '(w h) (. game_grid :pref_size))
	(. game_grid :change 0 0 w h)
	(def view :min_width w :min_height h)
	(. view :add_child game_grid)
	(bind '(x y w h) (apply view-fit (cat (. *window* :get_pos) (. *window* :pref_size))))
	(. *window* :change_dirty x y w h))

(defun rebuild-board ()
	(bind '(gw gh nm) difficulty)
	(. game_grid :sub)
	(setq game_grid (Grid))
	(defq gwh (* gw gh))
		; (ui-grid game_grid (:grid_width 1 :grid_height 5)
	(each (lambda (_)
		(defq value :nil)
		(cond
			((eql (elem-get _ game_map) "f")
				(. (defq mc (Button)) :connect (+ +event_click _))
				(def mc :text "F" :border 1 :flow_flags
					(logior +flow_flag_align_hcenter +flow_flag_align_vcenter) :min_width 32 :min_height 32)
				(. game_grid :add_child mc))
			((eql (elem-get _ game_map) "b")
				(. (defq mc (Button)) :connect (+ +event_click _))
				(def mc :text "" :border 1 :flow_flags
					(logior +flow_flag_align_hcenter +flow_flag_align_vcenter) :min_width 32 :min_height 32)
				(. game_grid :add_child mc))
			((eql (elem-get _ game_map) "r")
				(if (< 0 (elem-get _ game_board) 9)
					(. (defq mc (Label)) :connect (+ +event_click _))
					(defq mc (Label)))
				(def mc :text
					(cond
						((= (defq value (elem-get _ game_board)) 0) "")
						((< 0 value 9) (str value))
						((= value 9) "X"))
					:flow_flags (logior +flow_flag_align_hcenter +flow_flag_align_vcenter)
					:border 0 :ink_color (colorize value) :color (if (= value 9) +argb_red *env_toolbar_col*) :min_width 32 :min_height 32)
				(. game_grid :add_child mc))
			(:t :nil))) (range 0 gwh))
	(def game_grid :grid_width gw :grid_height gh :color (const *env_toolbar_col*) :font *env_window_font*)
	(bind '(w h) (. game_grid :pref_size))
	(. game_grid :change 0 0 w h)
	(def view :min_width w :min_height h)
	(. view :add_child game_grid)
	(bind '(x y w h) (apply view-fit (cat (. *window* :get_pos) (. *window* :pref_size))))
	(. *window* :change_dirty x y w h))

(defun main ()
	(defq started :nil game (list) game_board (list) game_adj (list) game_map (list)
		first_click :t difficulty (list) game_grid :nil click_offset 4 game_over :nil mouse_down 0)
	(bind '(x y w h) (apply view-locate (. *window* :pref_size)))
	(gui-add-front (. *window* :change x y w h))
	(while (cond
		((= (defq id (getf (defq msg (mail-read (task-netid))) +ev_msg_target_id)) +event_close)
			:nil)
		((= id +event_beginner) (setq started :t) (board-layout (setq difficulty beginner_settings)))
		((= id +event_intermediate) (setq started :t) (board-layout (setq difficulty intermediate_settings)))
		((= id +event_expert) (setq started :t) (board-layout (setq difficulty expert_settings)))
		((and started (not game_over)
			(<= +event_click id (+ +event_click (dec (* (first difficulty) (second difficulty))))))
			(defq cid (- id click_offset))
			(bind '(gw gh gn) difficulty)
			(when first_click (setq first_click :nil game (create-game gw gh gn cid) game_board (first game)
				game_map (second game) game_adj (third game)))
			(cond
				((= mouse_down 1)
					(cond
						((eql (elem-get cid game_map) "f") (clicked-flag cid))
						((= (elem-get cid game_board) 9) (clicked-mine cid) (setq game_over :t))
						((= (elem-get cid game_board) 0) (clicked-blank cid))
						((< 0 (elem-get cid game_board) 9) (clicked-value cid))
						(:t :nil)))
				((/= mouse_down 1)
					(cond
						((eql (elem-get cid game_map) "b") (right-clicked-button cid))
						((eql (elem-get cid game_map) "f") (clicked-flag cid))
						(:t :nil)))
				(:t :nil))
			(is-game-over?))
		(:t
			(and (= (getf msg +ev_msg_type) +ev_type_mouse)
				(/= 0 (getf msg +ev_msg_mouse_buttons))
				(setq mouse_down (getf msg +ev_msg_mouse_buttons)))
			(. *window* :event msg))))
	(gui-sub *window*))
