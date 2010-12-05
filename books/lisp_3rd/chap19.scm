(use srfi-1)
(define rest cdr)

;;
;; Solution 19-20
;;
(define (print-board board)
  (format #t "~%*")                     ; Upper left *.
  (print-horizontal-border board)       ; Upper boarder.
  (format #t "*")                       ; Upper right *.
  (dolist (queen-coordinates board)      
    (format #t "~%|")                   ; Left boarder.
    (dotimes (column (length board))
      (if (= column (second queen-coordinates))
          (format #t " Q")              ; Occupied.
          (format #t " .")))            ; Not occupied.
    (format #t " |"))                   ; Right border.
  (format #t "~%*")                     ; Lower left *.
  (print-horizontal-border board)       ; Lower border.
  (format #t "*"))                      ; Lower right *.

(define (print-horizontal-border board)
  (dotimes (n (+ 1 (* 2 (length board))))
    (format #t "-")))

;;(print-board '((0 1) (1 3) (2 0) (3 2)))

;;
;; Solution 19-13
;:
(define (threat i j a b)
  (or (= i a)
      (= j b)
      (= (- i j) (- a b))
      (= (+ i j) (+ a b))))

(define (conflict n m board)
  (cond ((null? board) #f)
        ((threat n
                 m
                 (first (first board))
                 (second (first board)))
         #t)
        (else (conflict n m (rest board)))))

(define (queen-aux size board n m)
  (unless (= m size)
    ;; Check for conflict in current row and column:
    (unless (conflict n m board)
      (if (= (+ 1 n) size)
          ;; If all queens placed, print solution:
          (print-board (reverse (cons (list n m) board)))
          ;; Otherwise, proceed to next row:
          (queen-aux size (cons (list n m) board) (+ 1 n) 0)))
    ;; In any case, try with next row:
    (queen-aux size board n (+ 1 m))))

(define (queen size)
  (queen-aux size '() 0 0))
