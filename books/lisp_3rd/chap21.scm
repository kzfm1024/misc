(use srfi-1)

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
  ((name :accessor hand-name :init-keyword :name)
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

(dolist (l (delete table *blocks*))
  (push! (block-support-for table) l)
  (set! (block-supported-by l) table))

(define *hand* (make <hand> :name '*hand* :position '(0 6)))

(define-method block-support-for ((object <basic-block>)) '())

(define-method put-on ((object <movable-block>) (support <basic-block>))
  (if (get-space object support)
      (and (grasp object)
           (move object support)
           (ungrasp object))
      (format #t "Sorry, there is no room for ~a on ~a.~%"
              (block-name object)
              (block-name support))))

;; スペースがない場合は #f を返す
(define-method get-space ((object <movable-block>) (support <basic-block>))
  (or (find-space object support)
      (make-space object support)))

(define-method grasp ((object <movable-block>))
  (unless (eq? (hand-grasping *hand*) object)
    (when (not (null? (block-support-for object))) (clear-top object))
    (when (not (null? (hand-grasping *hand*)))
      (get-rid-of (hand-grasping *hand*)))
    (format #t "Move hand to pick up ~a at location ~a.~%"
            (block-name object)
            (top-location object))
    (set! (hand-position *hand*) (top-location object))
    (format #t "Grasp ~a.~%" (block-name object))
    (set! (hand-grasping *hand*) object))
  #t)

(define-method ungrasp ((object <movable-block>))
  (when (not (null? (block-supported-by object)))
    (format #t "Ungrasp ~a.~%" (block-name object))
    (set! (hand-grasping *hand*) '())
    #t))

(define-method get-rid-of ((object <movable-block>))
  (put-on object table))

;; スペースがない場合は #f を返す
(define-method make-space ((object <movable-block>) (support <basic-block>))
  (call/cc
   (lambda (return)
     (dolist (obstruction (block-support-for support) #f)
       (get-rid-of obstruction)
       (let ((space (find-space object support)))
         (when space (return space)))))))

(define-method clear-top ((support <load-bearing-block>))
  (dolist (obstacle (block-support-for support) #t)
    (get-rid-of obstacle)))

(define-method move ((object <movable-block>) (support <basic-block>))
  (remove-support object)
  (let ((newplace (get-space object support)))
    (format #t "Move ~a to top of ~a at location ~a.~%"
            (block-name object)
            (block-name support)
            newplace)
    (set! (block-position object) newplace)
    (set! (hand-position *hand*) (top-location object)))
  (add-support object support)
  #t)

(define-method remove-support ((object <movable-block>))
  (let ((support (block-supported-by object)))
    (when (not (null? support))
      (set! (block-support-for support)
            (delete object (block-support-for support)))
      (set! (block-supported-by object) '())
      #t)))

(define-method add-support ((object <movable-block>)
                            (support <basic-block>))
  #t)

(define-method add-support ((object <movable-block>)
                            (support <load-bearing-block>))
  (push! (block-support-for support) object)
  (set! (block-supported-by object) support))

;; スペースがない場合は #f を返す
(define (find-space object support)
  (call/cc
   (lambda (return)
     (dotimes (offset (+ 1 (- (block-width support)
                              (block-width object)))
                      #f)
       (unless (intersections? object offset
                               (first (block-position support))
                               (block-support-for support))
         (return (list (+ offset (first (block-position support)))
                       (+ (second (block-position support))
                          (block-height support)))))))))

(define (intersections? object offset base obstacles)
  (call/cc
   (lambda (return)
     (dolist (obstacle obstacles #f)
       (let* ((ls-proposed (+ offset base))
              (rs-proposed (+ ls-proposed (block-width object)))
              (ls-obstacle (first (block-position obstacle)))
              (rs-obstacle (+ ls-obstacle (block-width obstacle))))
         (unless (or (>= ls-proposed rs-obstacle)
                     (<= rs-proposed ls-obstacle))
           (return #t)))))))

(define (top-location object)
  (list (+ (first (block-position object))
           (/ (block-width object) 2))
        (+ (second (block-position object))
           (block-height object))))

;; (begin (put-on b2 w7) (put-on b1 b2))
