;;;;;;;;;;;;;
; vector math
;;;;;;;;;;;;;

(import "class/num/lisp.inc")
(import "class/nums/lisp.inc")
(import "class/fixeds/lisp.inc")

(defun vec-eql (v1 v2) (every = v1 v2))

(defmacro vec-add (p1 p2 &optional _)
	(if _ `(nums-add ,p1 ,p2 ,_) `(nums-add ,p1 ,p2)))

(defmacro vec-sub (p1 p2 &optional _)
	(if _ `(nums-sub ,p1 ,p2 ,_) `(nums-sub ,p1 ,p2)))

(defmacro vec-min (p1 p2 &optional _)
	(if _ `(nums-min ,p1 ,p2 ,_) `(nums-min ,p1 ,p2)))

(defmacro vec-max (p1 p2 &optional _)
	(if _ `(nums-max ,p1 ,p2 ,_) `(nums-max ,p1 ,p2)))

(defmacro vec-mul (p1 p2 &optional _)
	(if _ `(nums-mul ,p1 ,p2 ,_) `(nums-mul ,p1 ,p2)))

(defmacro vec-div (p1 p2 &optional _)
	(if _ `(nums-div ,p1 ,p2 ,_) `(nums-div ,p1 ,p2)))

(defmacro vec-mod (p1 p2 &optional _)
	(if _ `(nums-mod ,p1 ,p2 ,_) `(nums-mod ,p1 ,p2)))

(defmacro vec-frac (p &optional _)
	(if _ `(fixeds-frac ,p ,_) `(fixeds-frac ,p)))

(defmacro vec-floor (p &optional _)
	(if _ `(fixeds-floor ,p ,_) `(fixeds-floor ,p)))

(defmacro vec-abs (p &optional _)
	(if _ `(nums-abs ,p ,_) `(nums-abs ,p)))

(defmacro vec-scale (p s &optional _)
	(if _ `(nums-scale ,p ,s ,_) `(nums-scale ,p ,s)))

(defmacro vec-dot (p1 p2) `(nums-dot ,p1 ,p2))

(defmacro vec-sum (p) `(nums-sum ,p))

(defmacro vec-clamp (p1 p2 p3 &optional _)
	(if _ `(nums-min (nums-max ,p1 ,p2 ,_) ,p3 ,_) `(nums-min (nums-max ,p1 ,p2) ,p3)))

(defun vec-reflect (p n &optional _)
	(vec-scale n (* (nums-sum (if _ (vec-mul p n _) (defq _ (vec-mul p n)))) 2.0) _)
	(vec-sub p _ _))

(defun vec-length (p)
	(sqrt (vec-dot p p)))

(defun vec-length-squared (p)
	(vec-dot p p))

(defun vec-norm (p &optional _)
	(if _ (vec-scale p (recip (vec-length p)) _) (vec-scale p (recip (vec-length p)))))

(defun vec-distance (p1 p2 &optional _)
	(if _ (vec-length (vec-sub p1 p2 _)) (vec-length (vec-sub p1 p2))))

(defun vec-distance-squared (p1 p2)
	(if _ (vec-length-squared (vec-sub p1 p2 _)) (vec-length-squared (vec-sub p1 p2))))

(defun vec-distance-to-line (p p1 p2)
	(defq lv (vec-sub p2 p1)
		pv (vec-sub p p1)
		c1 (vec-dot pv lv))
	(if (<= c1 0)
		(vec-distance p p1)
		(progn
			(defq c2 (vec-dot lv lv))
			(if (<= c2 c1)
				(vec-distance p p2)
				(vec-distance p (vec-add p1 (vec-scale lv (/ c1 c2))))))))

(defun vec-distance-squared-to-line (p p1 p2)
	(defq lv (vec-sub p2 p1)
		pv (vec-sub p p1)
		c1 (vec-dot pv lv))
	(if (<= c1 0)
		(vec-distance-squared p p1)
		(progn
			(defq c2 (vec-dot lv lv))
			(if (<= c2 c1)
				(vec-distance-squared p p2)
				(vec-distance-squared p (vec-add p1 (vec-scale lv (/ c1 c2))))))))

(defun vec-manhattan-distance (p1 p2)
	(nums-sum (nums-abs (defq _ (nums-sub p1 p2)) _)))

(defun vec-euclidean-distance (p1 p2)
	(sqrt (nums-sum (nums-mul (defq _ (nums-sub p1 p2)) _ _))))

(defun vec-squared-euclidean-distance (p1 p2)
	(nums-sum (nums-mul (defq _ (nums-sub p1 p2)) _ _)))

(defun vec-chebyshev-distance (p1 p2)
	(reduce max (nums-abs (defq _ (nums-sub p1 p2)) _)))

;specific vector stuff

(defun vec-perp-2d ((x y))
	(list y (neg x)))

(defun vec-det-2d ((x1 y1) (x2 y2))
	(- (* x1 y2) (* y1 x2)))

(defun vec-cross-3d ((x1 y1 z1) (x2 y2 z2))
	(list (- (* y1 z2) (* z1 y2))
		(- (* z1 x2) (* x1 z2))
		(- (* x1 y2) (* y1 x2))))

(defun vec-intersect-2d (l1_p1 av l2_p1 bv)
	(defq axb (vec-det-2d av bv)
		da (vec-det-2d (vec-add-2d l1_p1 av) l1_p1)
		db (vec-det-2d (vec-add-2d l2_p1 bv) l2_p1))
	(if (/= axb 0)
		(list
			(/ (vec-det-2d
				(list da (elem 0 av))
				(list db (elem 0 bv))) axb)
			(/ (vec-det-2d
				(list da (elem 1 av))
				(list db (elem 1 bv))) axb))))

(defun vec-intersect-lines-2d (l1_p1 l1_p2 l2_p1 l2_p2)
	(defq av (vec-sub-2d l1_p2 l1_p1)
		bv (vec-sub-2d l2_p2 l2_p1)
		axb (vec-det-2d av bv)
		da (vec-det-2d l1_p2 l1_p1)
		db (vec-det-2d l2_p2 l2_p1))
	(if (/= axb 0)
		(list
			(/ (vec-det-2d
				(list da (elem 0 av))
				(list db (elem 0 bv))) axb)
			(/ (vec-det-2d
				(list da (elem 1 av))
				(list db (elem 1 bv))) axb))))

(defun vec-collide-lines-2d (l1_p1 l1_p2 l2_p1 l2_p2)
	(defq av (vec-sub-2d l1_p2 l1_p1)
		bv (vec-sub-2d l2_p2 l2_p1)
		cv (vec-sub-2d l2_p2 l1_p1)
		axb (vec-det-2d av bv)
		axc (vec-det-2d av cv)
		cxb (vec-det-2d cv bv))
	(cond
		((= axb 0) nil)
		((> axb 0)
			(cond
				((or (< axc 0) (> axc axb)) nil)
				((or (< cxb 0) (> cxb axb)) nil)
				(t t)))
		(t
			(cond
				((or (> axc 0) (< axc axb)) nil)
				((or (> cxb 0) (< cxb axb)) nil)
				(t t)))))

(defun vec-collide-thick-lines-2d (l1_p1 l1_p2 l2_p1 tl2_p2 r)
	(cond
		((vec-collide-lines-2d l1_p1 l1_p2 l2_p1 l2_p2))
		((<= (distance-squared-to-line-2d l2_p1 l1_p1 l1_p2) (setq r (* r r))))
		((<= (distance-squared-to-line-2d l2_p2 l1_p1 l1_p2) r))
		((<= (distance-squared-to-line-2d l2_p1 l1_p1 l1_p2) r))
		((<= (distance-squared-to-line-2d l2_p2 l1_p1 l1_p2) r))))