(use srfi-1)
(use gauche.sequence)

(define (bingo-numbers)
  (let ((B (shuffle (iota 15 1)))
        (I (shuffle (iota 15 16)))
        (N (shuffle (iota 15 31)))
        (G (shuffle (iota 15 46)))
        (O (shuffle (iota 15 60))))
    (map list B I N G O)))

(for-each (lambda (l) (print l)) (bingo-numbers))
