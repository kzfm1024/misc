(use srfi-1)
(use srfi-27)

(define rest cdr)

(define (elt lst n) (first (drop lst n)))

(define (random-elt choices)
  (elt choices (random-integer (length choices))))

(random-source-randomize! default-random-source)

;;; ==============================

(define *simple-grammar*
  '((sentence -> (noun-phrase verb-phrase))
    (noun-phrase -> (Article Noun))
    (verb-phrase -> (Verb noun-phrase))
    (Article -> the a)
    (Noun -> man ball woman table)
    (Verb -> hit took saw liked)))

(define *grammar* *simple-grammar*)

;;; ==============================

(define (rule-lhs rule) (rest rule))

(define (rule-rhs rule) (rest (rest rule)))
  
(define (rewrites category)
  (rule-rhs (assoc category *grammar*)))

;;; ==============================

(define (generate phrase)
  (cond ((pair? phrase)
         (append-map generate phrase))
        ((rewrites phrase)
         (generate (random-elt (rewrites phrase))))
        (else (list phrase))))
