;;;;;;;;;;;;;;;;;;;;;
; VP64 Emit Functions
;;;;;;;;;;;;;;;;;;;;;

(defq opcode 0)
(defmacro next-opcode (&optional i)
	(defq ci opcode)
	(setq opcode (+ opcode (opt i 1)))
	ci)

(defun reg (r)
	(find-rev r '(r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 r13 r14 rsp)))

(defcvar 'stack_align 16 'stack_state '(r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 r13 r14))

(defun stack-init ()
	(defq tk_state_size (* ptr_size (length stack_state)))
	(vp-sub-cr (+ tk_state_size (* ptr_size 2)) r1)
	(f-bind 'sys_task :stop r2)
	(vp-cpy-ri r2 r1 (+ tk_state_size ptr_size))
	(vp-cpy-ri r4 r1 tk_state_size))

(defun emit-cr (o c r)
	(cond
		((<= -0x8 c 0x7)
			(emit-short (+ o (<< (reg r) 8) (<< c 12))))
		((<= -0x80000 c 0x7ffff)
			(emit-short (+ (inc o) (<< (reg r) 8) (<< c 12)) (>>> c 4)))
		((<= -0x800000000 c 0x7ffffffff)
			(emit-short (+ (+ o 2) (<< (reg r) 8) (<< c 12)) (>>> c 4) (>>> c 20)))
		(t	(emit-short (+ (+ o 3) (<< (reg r) 8)) c (>>> c 16) (>>> c 32) (>>> c 48)))))

(defun emit-cpy-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-add-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-sub-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-cmp-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-mul-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-and-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-or-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-xor-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-shl-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-shr-cr (c r) (emit-cr (next-opcode 4) c r))
(defun emit-asr-cr (c r) (emit-cr (next-opcode 4) c r))

(defun emit-cpy-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-add-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-sub-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-cmp-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-mul-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-and-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-or-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-xor-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-shl-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-shr-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-asr-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-lnot-rr (r d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg r) 12))))
(defun emit-land-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-swp-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-ext-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-div-rrr (s d1 d2) (emit-short (+ (next-opcode) (<< (reg d1) 8) (<< (reg d2) 12)) (reg s)))
(defun emit-div-rrr-u (s d1 d2) (emit-short (+ (next-opcode) (<< (reg d1) 8) (<< (reg d2) 12)) (reg s)))

(defun emit-scr (o c d)
	(cond
		((<= -0x8 c 0x7)
			(emit-short (+ o (<< (reg d) 8) (<< c 12))))
		((<= -0x80000 c 0x7ffff)
			(emit-short (+ (inc o) (<< (reg d) 8) (<< c 12)) (>>> c 4)))
		((<= -0x800000000 c 0x7ffffffff)
			(emit-short (+ (+ o 2) (<< (reg d) 8) (<< c 12)) (>>> c 4) (>>> c 20)))
		(t	(throw "emit-scr constant out of range !" c))))

(defun emit-seq-cr (c d) (emit-scr (next-opcode 3) c d))
(defun emit-sne-cr (c d) (emit-scr (next-opcode 3) c d))
(defun emit-slt-cr (c d) (emit-scr (next-opcode 3) c d))
(defun emit-sle-cr (c d) (emit-scr (next-opcode 3) c d))
(defun emit-sgt-cr (c d) (emit-scr (next-opcode 3) c d))
(defun emit-sge-cr (c d) (emit-scr (next-opcode 3) c d))

(defun emit-seq-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-sne-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-slt-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-sle-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-sgt-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))
(defun emit-sge-rr (s d) (emit-short (+ (next-opcode) (<< (reg d) 8) (<< (reg s) 12))))

(defun emit-branch (o c d)
	(defq m (elem d *distance*))
	(and (/= *pass* 0) (> (abs c) (abs m)) (elem-set d *distance* (setq m c)))
	(cond
		((<= -0x80 m 0x7f)
			(emit-short (+ o (<< c 8))))
		((<= -0x800000 m 0x7fffff)
			(emit-short (+ (inc o) (<< c 8)) (>>> c 8)))
		(t	(throw "emit-branch constant out of range !" c))))

(defun emit-beq (l d) (emit-branch (next-opcode 2) (- l *pc*) d))
(defun emit-bne (l d) (emit-branch (next-opcode 2) (- l *pc*) d))
(defun emit-bge (l d) (emit-branch (next-opcode 2) (- l *pc*) d))
(defun emit-blt (l d) (emit-branch (next-opcode 2) (- l *pc*) d))
(defun emit-ble (l d) (emit-branch (next-opcode 2) (- l *pc*) d))
(defun emit-bgt (l d) (emit-branch (next-opcode 2) (- l *pc*) d))

(defun emit-ir (o s c d)
	(cond
		((<= -0x8000 c 0x7fff)
			(emit-short (+ o (<< (reg d) 8) (<< (reg s) 12)) c))
		((<= -0x8000000 c 0x7fffffff)
			(emit-short (+ (inc o) (<< (reg d) 8) (<< (reg s) 12)) c (>>> c 16)))
		(t	(throw "emit-ir constant out of range !" c))))

(defun emit-cpy-ir (s c d) (emit-ir (next-opcode 2) s c d))
(defun emit-cpy-ir-b (s c d) (emit-ir (next-opcode 2) s c d))
(defun emit-cpy-ir-s (s c d) (emit-ir (next-opcode 2) s c d))
(defun emit-cpy-ir-i (s c d) (emit-ir (next-opcode 2) s c d))
(defun emit-cpy-ir-ub (s c d) (emit-ir (next-opcode 2) s c d))
(defun emit-cpy-ir-us (s c d) (emit-ir (next-opcode 2) s c d))
(defun emit-cpy-ir-ui (s c d) (emit-ir (next-opcode 2) s c d))
(defun emit-lea-i (s c d) (emit-ir (next-opcode 2) s c d))

(defun emit-ri (o s d c)
	(cond
		((<= -0x8000 c 0x7fff)
			(emit-short (+ o (<< (reg d) 8) (<< (reg s) 12)) c))
		((<= -0x8000000 c 0x7fffffff)
			(emit-short (+ (inc o) (<< (reg d) 8) (<< (reg s) 12)) c (>>> c 16)))
		(t	(throw "emit-ri constant out of range !" c))))

(defun emit-cpy-ri (s d c) (emit-ri (next-opcode 2) s d c))
(defun emit-cpy-ri-b (s d c) (emit-ri (next-opcode 2) s d c))
(defun emit-cpy-ri-s (s d c) (emit-ri (next-opcode 2) s d c))
(defun emit-cpy-ri-i (s d c) (emit-ri (next-opcode 2) s d c))

(defun emit-rd (o s d1 d2)
	(emit-short (+ o (<< (reg d1) 8) (<< (reg d2) 12)) (reg s)))

(defun emit-cpy-rd (s d1 d2) (emit-rd (next-opcode) s d1 d2))
(defun emit-cpy-rd-b (s d2 d1) (emit-rd (next-opcode) s d1 d2))
(defun emit-cpy-rd-s (s d2 d1) (emit-rd (next-opcode) s d1 d2))
(defun emit-cpy-rd-i (s d2 d1) (emit-rd (next-opcode) s d1 d2))

(defun emit-dr (o s1 s2 d)
	(emit-short (+ o (<< (reg d) 8) (<< (reg s1) 12)) (reg s2)))

(defun emit-cpy-dr (s1 s2 d) (emit-dr (next-opcode) s1 s2 d))
(defun emit-cpy-dr-b (s1 s2 d) (emit-dr (next-opcode) s1 s2 d))
(defun emit-cpy-dr-s (s1 s2 d) (emit-dr (next-opcode) s1 s2 d))
(defun emit-cpy-dr-i (s1 s2 d) (emit-dr (next-opcode) s1 s2 d))
(defun emit-cpy-dr-ub (s1 s2 d) (emit-dr (next-opcode) s1 s2 d))
(defun emit-cpy-dr-us (s1 s2 d) (emit-dr (next-opcode) s1 s2 d))
(defun emit-cpy-dr-ui (s1 s2 d) (emit-dr (next-opcode) s1 s2 d))
(defun emit-lea-d (s1 s2 d) (emit-dr (next-opcode) s1 s2 d))

(defun emit-call-jmp (o r c)
	(cond
		((<= -0x80000 c 0x7ffff)
			(emit-short (+ o (<< (reg r) 8) (<< c 12)) (>>> c 4)))
		(t	(throw "emit-call-jmp constant out of range !" c))))

(defun emit-call-r (r) (emit-call-jmp (next-opcode) r 0))
(defun emit-jmp-r (r) (emit-call-jmp (next-opcode) r 0))
(defun emit-call-i (r c) (emit-call-jmp (next-opcode) r c))
(defun emit-jmp-i (r c) (emit-call-jmp (next-opcode) r c))

(defun emit-cpy-rp (r l))
(defun emit-cpy-pr (l r))
(defun emit-lea-p (l r))
(defun emit-call (l))
(defun emit-call-p (l))
(defun emit-jmp (l d))
(defun emit-jmp-p (l))

(defun emit-alloc (c) (emit-sub-cr (align c stack_align) rsp))
(defun emit-free (c) (emit-add-cr (align c stack_align) rsp))
(defun emit-push (&rest b))
(defun emit-pop (&rest b))
(defun emit-ret () (emit-short (next-opcode)))