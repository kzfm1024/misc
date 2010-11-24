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
    (sqrt (+ (expt (- (first coordiantes-1)
                      (first coordiantes-2))
                   2)
             (expt (- (second coordiantes-1)
                      (second coordiantes-2))
                   2)))))

(defun closerp (path-1 path-2 target-node)
  (< (straight-line-distance (first path-1) target-node)
     (straight-line-distance (first path-2) target-node)))
