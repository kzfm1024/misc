;; Problem 2-1
(first '(p h w))
(first '(b k p h))
(first '((a b) (c d)))
(rest '((a b) (c d)))
(first (rest '((a b) (c d))))
(rest (first '((a b) (c d))))
(rest (first (rest '((a b) (c d)))))
(first (rest (first '((a b) (c d)))))

;; Problem 2-2
(first (rest (first (rest '((a b) (c d) (e f))))))
(first (first (rest (rest '((a b) (c d) (e f))))))
(first (first (rest '(rest ((a b) (c d) (e f))))))
(first (first '(rest (rest ((a b) (c d) (e f)))))) ;=> ERROR
(first '(first (rest (rest ((a b) (c d) (e f))))))
'(first (first (rest (rest ((a b) (c d) (e f))))))

;; Problem 2-3
(first (rest (rest '(apple orange pear grapefruit))))
(first (first (rest '((apple orange) (pear grapefruit)))))
(first (first (rest (rest (first '(((apple) (orange) (pear) (grapefruit))))))))
(first (first (first (rest (rest '(apple (orange) ((pear)) (((grapefruit)))))))))
(first (first (rest (rest '((((apple))) ((orange)) (pear) grapefruit)))))
(first (rest (first '((((apple) orange) pear) grapefruit))))
