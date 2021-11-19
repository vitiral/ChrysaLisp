;jit compile apps native functions
(import "sys/lisp.inc")
(jit "apps/pcb/" "lisp.vp" '("hit_line" "add_line" "sub_line"))

(import "class/lisp.inc")
;(import "lib/debug/frames.inc")
;(import "lib/debug/profile.inc")

(import "./app.inc")
(import "./reader.inc")
(import "./router.inc")

(enums +select 0
	(enum main timeout))

(defun route (select mbox grid_res vias_cost flood_range even_range odd_range data)
	(defq pcb_list (pcb-read data))
	(bind '(width height depth) (elem-get 0 pcb_list))
	(bind '(verb quant) '(1 1))
	(defq pcb (Pcb width height depth grid_res verb quant vias_cost flood_range even_range odd_range))
	(each! 1 -1 (lambda ((id track_radius via_radius track_gap pads wires &optional paths))
		(unless (mail-poll select)
			(task-slice)
			(setq wires '() pads (map (lambda ((radius gap pos shape))
				(Pad radius gap pos shape)) pads))
			(defq track (Track id track_radius via_radius track_gap pads wires))
			(. pcb :add_track track))) (list pcb_list))
	(. pcb :route select mbox)
	;(. pcb :print_pcb (file-stream "apps/pcb/data/test4.pcb" +file_open_write))
	(. pcb :close))

(defun main ()
	(defq select (alloc-select +select_size) running t +timeout 5000000)
	(while running
		(mail-timeout (elem-get +select_timeout select) +timeout 0)
		(defq msg (mail-read (elem-get (defq idx (mail-select select)) select)))
		(cond
			((or (= idx +select_timeout) (eql msg ""))
				;timeout or quit
				(setq running nil))
			((= idx +select_main)
				;main mailbox, reset timeout and reply with result
				(mail-timeout (elem-get +select_timeout select) 0 0)
				(mail-timeout (elem-get +select_timeout select) 1000000000 0)
				(route select (getf msg +job_reply)
					(getf msg +job_grid_res)
					(getf msg +job_vias_cost)
					(getf msg +job_flood_range)
					(getf msg +job_even_range)
					(getf msg +job_odd_range)
					(slice +job_data -1 msg)))))
	(free-select select)
	(if (get 'profile-report)
		(profile-report "Pcb")))
