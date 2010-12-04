(setf (get 's 'neighbors) '(a d)
      (get 's 'neighbors) '(a d)
      (get 'a 'neighbors) '(s b d)
      (get 'b 'neighbors) '(a c e)
      (get 'c 'neighbors) '(b)
      (get 'd 'neighbors) '(s a e)
      (get 'e 'neighbors) '(b d f)
      (get 'f 'neighbors) '(e))

(defun extend (path)
  (print (reverse path))
  (mapcar #'(lambda (new-node) (cons new-node path))
          (remove-if #'(lambda (neighbor) (member neighbor path))
                     (get (first path) 'neighbors))))

(defun depth-first (start finish &optional (queue (list (list start))))
  ;;(print queue)
  (cond ((endp queue) nil)
        ((eq finish (first (first queue)))
         (reverse (first queue)))
        (t (depth-first
            start
            finish
            (append (extend (first queue))
                    (rest queue))))))

(defun breadth-first (start finish &optional (queue (list (list start))))
  (print queue)
  (cond ((endp queue) nil)
        ((eq finish (first (first queue)))
         (reverse (first queue)))
        (t (breadth-first
            start
            finish
            (append (rest queue)
                    (extend (first queue)))))))

;;
;; Solution 19-1
;;
(defun depth-first (start finish)
  (do ((queue (list (list start))
              (append (extend (first queue))
                      (rest queue))))
      ((endp queue) nil)
    (when (eq finish (first (first queue)))
      (return (reverse (first queue))))))

;;
;; Solution 19-2
;;
(defun depth-first (start finish &optional (path (list start)))
  (if (eq finish (first path))
      (reverse path)
      (let ((neighbors (get (first path) 'neighbors)))
        (dolist (neighbor neighbors)
          (unless (member neighbor path)
            (let ((result (depth-first start
                                       finish
                                       (cons neighbor path))))
              (when result (return result))))))))

;;

(sort '(3 1 4 1 5 9) #'<)

(setf pi-front '(3 1 4 1 5 9))

(sort pi-front #'<)

(sort (copy-list pi-front) #'<)

;;

(defun best-first (start finish &optional (queue (list (list start))))
  (cond ((endp queue) nil)
        ((eq finish (first (first queue)))
         (reverse (first queue)))
        (t (best-first
            start
            finish
            (sort (append (extend (first queue))
                          (rest queue))
                  #'(lambda (p1 p2) (closerp p1 p2 finish)))))))

(setf (get 's 'coordinates) '(0 3)
      (get 'a 'coordinates) '(4 6)
      (get 'b 'coordinates) '(7 6)
      (get 'c 'coordinates) '(11 6)
      (get 'd 'coordinates) '(3 0)
      (get 'e 'coordinates) '(6 0)
      (get 'f 'coordinates) '(11 3))

(defun straight-line-distance (node-1 node-2)
  (let ((coordinates-1 (get node-1 'coordinates))
        (coordinates-2 (get node-2 'coordinates)))
    (sqrt (+ (expt (- (first coordinates-1)
                      (first coordinates-2))
                   2)
             (expt (- (second coordinates-1)
                      (second coordinates-2))
                   2)))))

(defun closerp (path-1 path-2 target-node)
  (< (straight-line-distance (first path-1) target-node)
     (straight-line-distance (first path-2) target-node)))

;;
;; Solution 19-3
;;

(merge 'list
       '(1 3 5 7 9)
       '(0 2 4 6 8)
       #'<)

(merge 'list "13579" '(#\0 #\2 #\4 #\6 #\8) #'char<)

(merge 'string "13579" '(#\0 #\2 #\4 #\6 #\8) #'char<)

(defun best-first (start finish &optional (queue (list (list start))))
  (cond ((endp queue) nil)
        ((eq finish (first (first queue)))
         (reverse (first queue)))
        (t (best-first
            start
            finish
            (merge 'list
                   (sort (extend (first queue))
                         #'(lambda (p1 p2) (closerp p1 p2 finish)))
                   (rest queue)
                   #'(lambda (p1 p2) (closerp p1 p2 finish)))))))

;;
;; Solution 19-4
;;
(defun hill-climb (start finish &optional (queue (list (list start))))
  (cond ((endp queue) nil)
        ((eq finish (first (first queue)))
         (reverse (first queue)))
        (t (hill-climb
            start
            finish
            (append (sort (extend (first queue))
                          #'(lambda (p1 p2) (closerp p1 p2 finish)))
                    (rest queue))))))

;;
;; Solution 19-5
;;
(defun path-length (path)
  (if (endp (rest path))
      0
      (+ (straight-line-distance (first path) (second path))
         (path-length (rest path)))))

(defun shorterp (path-1 path-2)
  (< (path-length path-1) (path-length path-2)))

(defun branch-and-bound (start finish &optional (queue (list (list start))))
  (cond ((endp queue) nil)
        ((eq finish (first (first queue)))
         (reverse (first queue)))
        (t (branch-and-bound
            start
            finish
            (sort (append (extend (first queue))
                          (rest queue))
                  #'shorterp)))))

;;
;; Solution 19-6
;;
(defun beam (start finish width &optional (queue (list (list start))))
  (setf queue (butlast queue (max (- (length queue) width) 0)))
  (cond ((endp queue) nil)
        ((eq finish (first (first queue)))
         (reverse (first queue)))
        (t (beam start
                 finish
                 width
                 (sort
                  (apply #'append (mapcar #'extend queue))
                  #'(lambda (p1 p2) (closerp p1 p2 finish)))))))

;;
;; Solution 19-7
;;
(setf (get 's 'successors) '(a d)
      (get 'a 'successors) '(b d)
      (get 'b 'successors) '(c e)
      (get 'c 'successors) '()
      (get 'd 'successors) '(e)
      (get 'e 'successors) '(f)
      (get 'f 'successors) '())

(setf (get 's 'time-consumed) 3
      (get 'a 'time-consumed) 2
      (get 'b 'time-consumed) 4
      (get 'c 'time-consumed) 3
      (get 'd 'time-consumed) 3
      (get 'e 'time-consumed) 2
      (get 'f 'time-consumed) 1)

(defun first-path-incomplete-p (p1 p2)
  (not (endp (extend p1)))) ; Second argument ignored.

(defun all-paths (start &optional (queue (list (list start))))
  (cond ((endp (extend (first queue)))
         (mapcar #'reverse queue))
        (t (all-paths
            start
            (sort (append (extend (first queue)) (rest queue))
                  #'first-path-incomplete-p)))))

(defun extend (path)
  (mapcar #'(lambda (new-node) (cons new-node path))
          (remove-if #'(lambda (successor) (member successor path))
                     (get (first path) 'successors))))
;;
;; Solution 19-8
;;
(defun time-consumed (path)
  (if (endp path)
      0
      (+ (get (first path) 'time-consumed)
         (time-consumed (rest path)))))

(defun longerp (path-1 path-2)
  (> (time-consumed path-1)
     (time-consumed path-2)))

(defun critical-path (start &optional (queue (list (list start))))
  (cond ((endp (extend (first queue)))
         (reverse (first (sort queue #'longerp))))
        (t (critical-path
            start
            (sort (append (extend (first queue)) (rest queue))
                  #'first-path-incomplete-p)))))

;;
;; Solution 19-9
;;
(defun move-best-to-front (queue predicate)
  (if (endp queue)
      nil
      (let ((result (first queue)))
        (dolist (next (rest queue)
                 (cons result
                       (remove result queue :test #'equal)))
          (when (funcall predicate next result)
            (setf result next))))))

;;
;; Solution 19-10
;;
(defun transfer (x y a b c)
  (cond ((= x c)
         (format t "~%I can produce ~a units in A." c)
         nil)
        ((= y c)
         (format t "~%I can produce ~a units in B." c)
         nil)
        ;; If crock A is full, empty it:
        ((= x a) (cons '(empty a) (transfer 0 y a b c)))
        ;; If crokc B is empty, fill it:
        ((= y 0) (cons '(fill b) (transfer x b a b c)))
        ;; Will what is in B fit into A?
        ;; If yes, empty B into A:
        ((> (- a x) y) (cons '(empty b into a)
                             (transfer (+ x y) 0 a b c)))
        ;; Otherwise, fill A from B:
        (t (cons '(fill a from b)
                 (transfer a (- y (- a x)) a b c)))))

;;
;; Solution 19-11
;;
(defun water-crock (a b c)
  (cond ((and (> c a) (> c b))
         (format t "~%~a is too large." c))
        ((not (zerop (rem c (gcd a b))))
         (format t "~%Sorry, I cannot produce ~a units." c))
        (t (transfer 0 0 a b c))))

;;
;; Solution 19-20
;;
(defun print-board (board)
  (format t "~%*")                      ; Upper left *.
  (print-horizontal-border board)       ; Upper boarder.
  (format t "*")                        ; Upper right *.
  (dolist (queen-coordinates board)
    (format t "~%|")                    ; Left boarder.
    (dotimes (column (length board))
      (if (= column (second queen-coordinates))
          (format t " Q")               ; Occupied.
          (format t " .")))             ; Not occupied.
    (format t " |"))                    ; Right border.
  (format t "~%*")                      ; Lower left *.
  (print-horizontal-border board)       ; Lower border.
  (format t "*"))                       ; Lower right *.

(defun print-horizontal-border (board)
  (dotimes (n (+ 1 (* 2 (length board))))
    (format t "-")))

;;
;; Solution 19-13
;:
(defun threat (i j a b)
  (or (= i a)
      (= j b)
      (= (- i j) (- a b))
      (= (+ i j) (+ a b))))

(defun conflict (n m board)
  (cond ((endp board) nil)
        ((threat n
                 m
                 (first (first board))
                 (second (first board)))
         t)
        (t (conflict n m (rest board)))))

(defun queen (size &optional (board nil) (n 0) (m 0))
  (unless (= m size)
    ;; Check for conflict in current row and column:
    (unless (conflict n m board)
      (if (= (+ 1 n) size)
          ;; If all queens placed, print solution:
          (print-board (reverse (cons (list n m) board)))
          ;; Otherwise, proceed to next row:
          (queen size (cons (list n m) board) (+ 1 n) 0)))
    ;; In any case, try with next row:
    (queen size board n (+ 1 m))))
