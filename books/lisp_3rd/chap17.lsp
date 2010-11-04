(defvar *x*)

(defun inside (*x* e)
  (inside-aux e))

(defun inside-aux (e)
  (cond ((atom e) (eq *x* e))
        (t (or (inside-aux (first e))
               (inside-aux (rest e))))))

;;
;; Problem 17-1
;;

(defmacro enqueue (item queue)
  `(progn (if ,queue
              (setf (rest (last ,queue)) (list ,item))
              (setf ,queue (list ,item)))
          ,queue))

(defmacro dequeue (queue)
  `(let ((result (first ,queue)))
     (setf ,queue (rest ,queue))
     result))

;;------------------------------------------------------------
(defun user-reverse (l &optional result)
  (if (endp l)
      result
      (user-reverse (rest l)
                    (cons (first l) result))))

;;
;; Problem 17-2
;;
(defun user-reverse (l)
  (if (endp l)
      nil
      (nconc (user-reverse (rest l))
             (list (first l)))))

;;
;; Problem 17-3
;;
(defun user-defined-nreverse (l &optional reversed-list)
  (if (endp l)
      reversed-list
      (let ((remaining-elements (rest l)))
        (setf (rest l) reversed-list)
        (user-defined-nreverse remaining-elements
                               l))))
