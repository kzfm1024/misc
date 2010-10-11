;; Problem 5-1
(defun skip-first-n (n lst)
  (if (zerop n)
      lst
      (skip-first-n (- n 1) (rest lst))))

(skip-first-n 3 '(a b c d e f g h i))

;; Problem 5-2
(defun keep-first-n (n lst)
  (if (zerop n)
      nil
      (cons (first lst) (keep-first-n (- n 1) (rest lst)))))

(keep-first-n 3 '(a b c d e f g h i))

;; Problem 5-3
(defun keep-first-n-cleverly (n lst)
  (keep-first-n-cleverly-aux n lst '()))

(defun keep-first-n-cleverly-aux (n lst result)
  (if (zerop n)
      (reverse result)
      (keep-first-n-cleverly-aux (- n 1) (rest lst) (cons (first lst) result))))

(keep-first-n-cleverly 3 '(a b c d e f g h i))

;; Problem 5-4
(defun add (x y)
  (if (zerop x)
      y
      (add (1- x) (1+ y))))
