(defvar *x*)

(defun inside (*x* e)
  (inside-aux e))

(defun inside-aux (e)
  (cond ((atom e) (eq *x* e))
        (t (or (inside-aux (first e))
               (inside-aux (rest e))))))
