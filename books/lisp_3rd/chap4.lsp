;; Problem 4-1
(defun divisible-by-three (n) (zerop (rem n 3)))

(divisible-by-three 6) ;=> T
(divisible-by-three 7) ;=> NIL
(divisible-by-three 8) ;=> NIL

;; Problem 4-2
(defun palindromep (lst) (equal lst (reverse lst)))

(palindromep '(a b c b a))
(palindromep '(a b c c b a))
(palindromep '(a b c c c a))

;; Problem 4-3
(defun rightp (a b c)
  (let ((L (* a a))
        (S (+ (* b b) (* c c))))
    (< (* (/ (abs (- S L)) L) 100.0) 2.0)))

(rightp 5 4 3)
(rightp 5 3 3)



  





  