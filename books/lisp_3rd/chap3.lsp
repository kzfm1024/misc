;; Problem 3-1
(defun exchange (l)
  (append (last l) (list (first l))))

;; Problem 3-2
(defun construct (x y) (cons x y))

;; Problem 3-3
(defun rotate-left (l)
  (append (rest l) (list (first l))))

;; Problem 3-4
(defun rotate-right (l)
  (append (last l) (butlast l)))

;; Problem 3-5
(defun palindromize (l)
  (append l (reverse l)))

;; Problem 3-6
(defun f-to-c (f)
  (float (- (* (+ f 40) 5/9) 40)))

(defun c-to-f (f)
  (float (- (* (+ f 40) 9/5) 40)))

