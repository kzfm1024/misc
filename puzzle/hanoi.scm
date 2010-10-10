(define (hanoi n from to via)
  (cond ((= n 1)
         (format #t "disk ~D: from ~A to ~A~%" n from to))
        (else
         (hanoi (- n 1) from via to)
         (format #t "disk ~D: from ~A to ~A~%" n from to)
         (hanoi (- n 1) via to from))))

(hanoi 3 'a 'b 'c)
