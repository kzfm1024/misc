(defclass basic-block ()
  ((name :accessor block-name :initarg :name)
   (width :accessor block-width :initarg :width)
   (height :accessor block-height :initarg :height)
   (position :accessor block-position :initarg :position)
   (supported-by :accessor block-supported-by :initform nil)))

(defclass movable-block (basic-block) ())

(defclass load-bearing-block (basic-block)
  ((support-for :accessor block-support-for :initform nil)))

(defclass brick (movable-block load-bearing-block) ())

(defclass wedge (movable-block) ())

(defclass ball (movable-block) ())

(defclass table (load-bearing-block) ())

(defclass hand ()
  ((name :accessor hand-name :initarg :name)
   (position :accessor hand-position :initarg :position)
   (grasping :accessor hand-grasping :initform nil)))

(defvar *blocks*
  (list
   (make-instance 'table :name 'table :width 20 :height 0 :position '(0 0))
   (make-instance 'brick :name 'b1 :width 2 :height 2 :position '(0 0))
   (make-instance 'brick :name 'b2 :width 2 :height 2 :position '(2 0))
   (make-instance 'brick :name 'b3 :width 4 :height 4 :position '(4 0))
   (make-instance 'brick :name 'b4 :width 2 :height 2 :position '(8 0))
   (make-instance 'wedge :name 'w5 :width 2 :height 4 :position '(10 0))
   (make-instance 'brick :name 'b6 :width 4 :height 2 :position '(12 0))
   (make-instance 'wedge :name 'w7 :width 2 :height 2 :position '(16 0))
   (make-instance 'ball  :name 'l8 :width 2 :height 2 :position '(18 0))))

(dolist (l *blocks*) (set (block-name l) l))

(defvar table)
(defvar b1)
(defvar b2)
(defvar b3)
(defvar b4)
(defvar w5)
(defvar b6)
(defvar w7)
(defvar l8)

(dolist (l (remove table *blocks*))
  (push l (block-support-for table))
  (setf (block-supported-by l) table))

(defvar *hand* (make-instance 'hand :name '*hand* :position '(0 6)))

;;
;; Solution 21-1
;;
(defun make-block (class name width height x y)
  (make-instance class
                 :name name
                 :width width
                 :height height
                 :position (list x y)))

;;
;; Solution 21-2
;;
(defclass basic-block ()
  ((name :accessor block-name :initarg :name)
   (width :accessor block-width :initarg :width)
   (height :accessor block-height :initarg :height)
   (position :accessor block-position :initform '(0 10))
   (supported-by :accessor block-supported-by :initform nil)))

(defvar *blocks*
  (list
   (make-instance 'table :name 'table :width 20 :height 0 :position '(0 0))
   (make-instance 'brick :name 'b1 :width 2 :height 2)
   (make-instance 'brick :name 'b2 :width 2 :height 2)
   (make-instance 'brick :name 'b3 :width 4 :height 4)
   (make-instance 'brick :name 'b4 :width 2 :height 2)
   (make-instance 'wedge :name 'w5 :width 2 :height 4)
   (make-instance 'brick :name 'b6 :width 4 :height 2)
   (make-instance 'wedge :name 'w7 :width 2 :height 2)
   (make-instance 'ball  :name 'l8 :width 2 :height 2)))

(dolist (l *blocks*) (set (block-name l) l))

(defvar table)

(dolist (l (remove table *blocks*))
  (put-on l table))

;;------------------------------------------------------------

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
