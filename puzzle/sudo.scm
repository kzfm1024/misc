;;
;; sudo.scm
;;
;; http://norvig.com/sudoku.html を Scheme に移植
;;

(use srfi-1)
(use srfi-13)
(use srfi-14)
(use gauche.collection)

(define-syntax do-map
  (syntax-rules ()
    [(_ (e1 e2) e3 ...) (map (lambda (e1) e3 ...) e2)]))

(define-syntax do-every
  (syntax-rules ()
    [(_ (e1 e2) e3 ...) (every (lambda (e1) e3 ...) e2)]))

(define-syntax do-any
  (syntax-rules ()
    [(_ (e1 e2) e3 ...) (any (lambda (e1) e3 ...) e2)]))

(define-syntax block
  (syntax-rules ()
    [(_ escape body ...)
     (call/cc (lambda (escape) body ...))]))

(define (cross A B)
  (apply append
         (do-map (a (string->list A))
           (do-map (b (string->list B))
             (string a b)))))

(define *rows* "ABCDEFGHI")
(define *cols* "123456789")
(define *digits* "123456789")
(define *squares* (cross *rows* *cols*))

(define *unitlist*
  (apply append
         (do-map (c (string->list *cols*)) (cross *rows* (string c)))
         (do-map (r (string->list *rows*)) (cross (string r) *cols*))
         (do-map (rs '("ABC" "DEF" "GHI"))
           (do-map (cs '("123" "456" "789"))
             (cross rs cs)))))

(define *units*
  (let ((ht (make-hash-table 'string=?)))
    (dolist (s *squares*)
      (hash-table-put! ht s (filter (lambda (u) (member s u)) *unitlist*)))
    ht))

(define *peers*
  (let ((ht (make-hash-table 'string=?)))
    (dolist (s *squares*)
      (hash-table-put!
       ht s
       (delete s (apply lset-union (cons equal? (hash-table-get *units* s))))))
    ht))

(define (parse-grid grid)
  (let1 values (make-hash-table 'string=?)
    (dolist (s *squares*)
      (hash-table-put! values s *digits*))
    (dolist (sd (zip *squares* (string->list (string-filter grid #[0._123456789]))))
      (let ((s (car sd)) (d (string (cadr sd))))
        (when (rxmatch #/[123456789]/ d) (assign! values s d))))
    values))

(define (assign! values s d)
  (let1 ds (string-delete (hash-table-get values s) (string->char-set d))
    (if (string-null? ds)
        values ; Already assigned
        (do-every (c (string->list ds))
          (eliminate! values s (string c))))))

(define (eliminate! values s d)
  (block return
    (unless (string-scan (hash-table-get values s) d) (return values))
    (unless (eliminate-sub1! values s d) (return #f))
    (eliminate-sub2! values s d)))

(define (eliminate-sub1! values s d)
  (let* ((d2 (string-delete (hash-table-get values s) (string->char-set d)))
         (len (string-length d2)))
    (hash-table-put! values s d2)
    (cond ((= len 0) #f) ; Contradiction: removed last value
          ((= len 1) (do-every (s2 (hash-table-get *peers* s))
                       (eliminate! values s2 d2)))
          (else values))))

(define (eliminate-sub2! values s d)
  (do-every (u (hash-table-get *units* s))
    (let* ((dplaces
            (filter
             (lambda (s) (string-scan (hash-table-get values s) d)) u))
           (len (length dplaces)))
      (cond ((= len 0) #f)
            ((= len 1) (assign! values (car dplaces) d))
            (else values)))))

;; ----------------------------------------------------------------------

(define (copy-hash-table ht)
  (let ((ht2 (make-hash-table 'string=?)))
    (hash-table-for-each
     ht
     (lambda (key val) (hash-table-put! ht2 key val)))
    ht2))

(define (unfilled-square values)
  (find-min
   (filter (lambda (p) (> (string-length (cdr p)) 1))
           (hash-table-map values (lambda (k v) (cons k v))))
   :key (lambda (p) (string-length (cdr p)))))

(define (search values)
  (block return
    (unless values (return #f))
    (when (do-every (s *squares*)
            (= (string-length (hash-table-get values s)) 1))
      (return values)) ; Solved
    (let* ((sd (unfilled-square values))
           (s (car sd)) (ds (cdr sd)))
      (do-any (c (string->list ds))
        (search (assign! (copy-hash-table values) s (string c)))))))

;; ----------------------------------------------------------------------

;; very simple version
(define (printboard values) 
  (dolist (r (string->list *rows*))
    (dolist (c (string->list *cols*))
      (display (hash-table-get values (string r c)))
      (display " "))
    (newline))
  (newline))

(define easy-grid
  "003020600
   900305001
   001806400
   008102900
   700000008
   006708200
   002609500
   800203009
   005010300")

(define hard-grid1
  "4.....8.5 
   .3.......
   ...7.....
   .2.....6.
   ....8.4..
   ....1....
   ...6.3.7.
   5..2.....
   1.4......")

(use gauche.time)
(time (printboard (search (parse-grid easy-grid))))
(time (printboard (search (parse-grid hard-grid1))))
