;; Problem 2-2
(first '(p h w))
(first '(b k p h))
(first '((a b) (c d)))
(rest '((a b) (c d)))
(first (rest '((a b) (c d))))
(rest (first '((a b) (c d))))
(rest (first (rest '((a b) (c d)))))
(first (rest (first '((a b) (c d)))))

;; Problem 2-3
(first (rest (first (rest '((a b) (c d) (e f))))))
(first (first (rest (rest '((a b) (c d) (e f))))))
(first (first (rest '(rest ((a b) (c d) (e f))))))
(first (first '(rest (rest ((a b) (c d) (e f)))))) ;=> ERROR
(first '(first (rest (rest ((a b) (c d) (e f))))))
'(first (first (rest (rest ((a b) (c d) (e f))))))

;; Problem 2-4
(first (rest (rest '(apple orange pear grapefruit))))
(first (first (rest '((apple orange) (pear grapefruit)))))
(first (first (rest (rest (first '(((apple) (orange) (pear) (grapefruit))))))))
(first (first (first (rest (rest '(apple (orange) ((pear)) (((grapefruit)))))))))
(first (first (rest (rest '((((apple))) ((orange)) (pear) grapefruit)))))
(first (rest (first '((((apple) orange) pear) grapefruit))))

;; Problem 2-5
(append '(a b c) '()) ;=> (A B C)
(list '(a b c) '())   ;=> ((A B C) NIL)
(cons '(a b c) '())   ;=> ((A B C))

;; Problem 2-6
(setf tools (list 'hammer 'screwdriver)) ;=> (HAMMER SCREWDRIVER)
(cons 'pliers tools) ;=> (PLIERS HAMMER SCREWDRIVER)
tools ;=> (HAMMER SCREWDRIVER)
(setf tools (cons 'pliers tools)) ;=> (PLIERS HAMMER SCREWDRIVER)
tools ;=> (PLIERS HAMMER SCREWDRIVER)
(append '(saw wrench) tools) ;=> (SAW WRENCH PLIERS HAMMER SCREWDRIVER)
tools ;=> (PLIERS HAMMER SCREWDRIVER)
(setf tools (append '(saw wrench) tools)) ;=> (SAW WRENCH PLIERS HAMMER SCREWDRIVER)
tools ;=> (SAW WRENCH PLIERS HAMMER SCREWDRIVER)

;; Problem 2-7
(cons (first nil) (rest nil)) ;=> (NIL)

;; Problem 2-8
(length '(plato socrates aristotle)) ;=> 3
(length '((plato) (socrates) (aristotle))) ;=> 3
(length '((plato socrates aristotle))) ;=> 1
(reverse '(plato socrates aristotle)) ;=> (ARISTOTLE SOCRATES PLATO)
(reverse '((plato) (socrates) (aristotle))) ;=> ((ARISTOTLE) (SOCRATES) (PLATO))
(reverse '((plato socrates aristotle))) ;=> ((PLATO SOCRATES ARISTOTLE))

;; Problem 2-9
(length '((car chevrolet) (drink core) (cereal wheaties))) ;=> 3
(reverse '((car chevrolet) (drink core) (cereal wheaties)))
;=> ((CEREAL WHEATIES) (DRINK CORE) (CAR CHEVROLET))
(append '((car chevrolet) (drink core))
        (reverse '((car chevrolet) (drink coke))))
;=> ((CAR CHEVROLET) (DRINK CORE) (DRINK COKE) (CAR CHEVROLET))

;; Problem 2-10
(/ (+ 3 1) (- 3 1)) ;=> 2
(* (max 3 4 5) (min 3 4 5)) ;=> 15
(min (max 3 1 4) (max 2 7 1)) ;=> 4
