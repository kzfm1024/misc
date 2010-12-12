(define-class <basic-block> ()
  ((name :accessor block-name :init-keyword :name)
   (width :accessor block-width :init-keyword :width)
   (height :accessor block-height :init-keyword :height)
   (position :accessor block-position :init-keyword :position)
   (supported-by :accessor block-supported-by :init-form '())))

(define-class <movable-block> (<basic-block>) ())

(define-class <load-bearing-block> (<basic-block>)
  ((support-for :accessor block-support-for :init-form '())))

(define-class <brick> (<movable-block> <load-bearing-block>) ())

(define-class <wedge> (<movable-block>) ())

(define-class <ball> (<movable-block>) ())

(define-class <table> (<load-bearing-block>) ())

(define-class <hand> ()
  ((name :accessor hand-position :init-keyword :name)
   (position :accessor hand-position :init-keyword :position)
   (grasping :accessor hand-grasping :init-form '())))

(define table (make <table> :name 'table :width 20 :height 0 :position '(0 0)))
(define b1 (make <brick> :name 'b1 :width 2 :height 2 :position '(0 0)))
(define b2 (make <brick> :name 'b2 :width 2 :height 2 :position '(2 0)))
(define b3 (make <brick> :name 'b3 :width 4 :height 4 :position '(4 0)))
(define b4 (make <brick> :name 'b4 :width 2 :height 2 :position '(8 0)))
(define w5 (make <wedge> :name 'w5 :width 2 :height 4 :position '(10 0)))
(define b6 (make <brick> :name 'b6 :width 4 :height 2 :position '(12 0)))
(define w7 (make <wedge> :name 'w7 :width 2 :height 2 :position '(16 0)))
(define l8 (make <ball>  :name 'l8 :width 2 :height 2 :position '(18 0)))
(define *blocks* (list table b1 b2 b3 b4 w5 b6 w7 l8))

(use srfi-1)

(dolist (l (delete table *blocks*))
  (push! (block-support-for table) l)
  (set! (block-supported-by l) table))

(define *hand* (make <hand> :name '*hand* :position '(0 6)))

#|
(defmethod block-support-for ((object basic-block))
  nil)

(defmethod put-on ((object movable-block) (support basic-block))
  (if (get-space object support)
      (and (grasp object)
           (move object support)
           (ungrasp object))
      (format t "~&Sorry, there is no room for ~a on ~a."
              (block-name object)
              (block-name support))))

(defmethod get-space ((object movable-block) (support basic-block))
  (or (find-space object support)
      (make-space object support)))

(defmethod grasp ((object movable-block))
  (unless (eq (hand-grasping *hand*) object)
    (when (block-support-for object) (clear-top object))
    (when (hand-grasping *hand*)
      (get-rid-of (hand-grasping *hand*)))
    (format t "~&Move hand to pick up ~a at location ~a."
            (block-name object)
            (top-location object))
    (setf (hand-position *hand*) (top-location object))
    (format t "~&Grasp ~a." (block-name object))
    (setf (hand-grasping *hand*) object))
  t)

(defmethod ungrasp ((object movable-block))
  (when (block-supported-by object)
    (format t "~&Ungrasp ~a." (block-name object))
    (setf (hand-grasping *hand*) nil)
    t))

(defmethod get-rid-of ((object movable-block))
  (put-on object table))

(defmethod make-space ((object movable-block) (support basic-block))
  (dolist (obstruction (block-support-for support))
    (get-rid-of obstruction)
    (let ((space (find-space object support)))
      (when space (return space)))))

(defmethod clear-top ((support load-bearing-block))
  (dolist (obstacle (block-support-for support) t)
    (get-rid-of obstacle)))

(defmethod move ((object movable-block) (support basic-block))
  (remove-support object)
  (let ((newplace (get-space object support)))
    (format t "~&Move ~a to top of ~a at location ~a."
            (block-name object)
            (block-name support)
            newplace)
    (setf (block-position object) newplace)
    (setf (hand-position *hand*) (top-location object)))
  (add-support object support)
  t)

(defmethod remove-support ((object movable-block))
  (let ((support (block-supported-by object)))
    (when support
      (setf (block-support-for support)
            (remove object (block-support-for support)))
      (setf (block-supported-by object) nil)
      t)))

(defmethod add-support ((object movable-block)
                        (support basic-block))
  t)

(defmethod add-support ((object movable-block)
                        (support load-bearing-block))
  (push object (block-support-for support))
  (setf (block-supported-by object) support))

(defun find-space (object support)
  (dotimes (offset (+ 1 (- (block-width support)
                           (block-width object))))
    (unless (intersections-p object offset
                             (first (block-position support))
                             (block-support-for support))
      (return (list (+ offset (first (block-position support)))
                    (+ (second (block-position support))
                       (block-height support)))))))

(defun intersections-p (object offset base obstacles)
  (dolist (obstacle obstacles)
    (let* ((ls-proposed (+ offset base))
           (rs-proposed (+ ls-proposed (block-width object)))
           (ls-obstacle (first (block-position obstacle)))
           (rs-obstacle (+ ls-obstacle (block-width obstacle))))
      (unless (or (>= ls-proposed rs-obstacle)
                  (<= rs-proposed ls-obstacle))
        (return t)))))

(defun top-location (object)
  (list (+ (first (block-position object))
           (/ (block-width object) 2))
        (+ (second (block-position object))
           (block-height object))))

(defmethod print-object ((x basic-block) stream)
  (format stream "#<block ~a>" (block-name x)))
|#
