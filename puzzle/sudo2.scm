;;
;; sudo2.scm
;;
;; http://norvig.com/sudoku.html を Scheme に移植
;; 試しにオブジェクトシステムを使用
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

(define (copy-hash-table ht)
  (let ((ht2 (make-hash-table (hash-table-type ht))))
    (hash-table-for-each
     ht
     (lambda (key val) (hash-table-put! ht2 key val)))
    ht2))

(define (cross A B)
  (apply append
         (do-map (a (string->list A))
           (do-map (b (string->list B))
             (string a b)))))

(define-class <grid-values> ()
  ((rows :init-value "ABCDEFGHI" :allocation :class)
   (cols :init-value "123456789" :allocation :class)
   (digits :init-value "123456789" :allocation :class)
   (squares :allocation :class)
   (unitlist :allocation :class)
   (units :allocation :class)
   (peers :allocation :class)
   (values :init-keyword :values :allocation :instance)))

(define-method initialize ((self <grid-values>) initargs)
  (next-method)
  (unless (slot-bound? self 'squares)
    (set! (ref self 'squares)
          (cross (ref self 'rows) (ref self 'cols))))
  (unless (slot-bound? self 'unitlist)
    (set! (ref self 'unitlist)
          (let ((rows (ref self 'rows)) (cols (ref self 'cols)))
            (apply append
                   (do-map (c (string->list cols)) (cross rows (string c)))
                   (do-map (r (string->list rows)) (cross (string r) cols))
                   (do-map (rs '("ABC" "DEF" "GHI"))
                     (do-map (cs '("123" "456" "789"))
                       (cross rs cs)))))))
  (unless (slot-bound? self 'units) 
    (set! (ref self 'units) 
          (let ((ht (make-hash-table 'string=?))
                (squares (ref self 'squares))
                (unitlist (ref self 'unitlist)))
            (dolist (s squares)
              (hash-table-put!
               ht s
               (filter (lambda (u) (member s u)) unitlist)))
            ht)))
  (unless (slot-bound? self 'peers)
    (set! (ref self 'peers)
          (let ((ht (make-hash-table 'string=?))
                (squares (ref self 'squares))
                (units (ref self 'units)))
            (dolist (s squares)
              (hash-table-put!
               ht s
               (delete s (apply lset-union
                                (cons equal? (hash-table-get units s))))))
            ht)))
  (unless (slot-bound? self 'values)
    (set! (ref self 'values)
          (let1 ht (make-hash-table 'string=?)
            (dolist (s (ref self 'squares))
              (hash-table-put! ht s (ref self 'digits)))
            ht)))
  )  
          
(define-method copy ((gv <grid-values>))
  (make <grid-values> :values (copy-hash-table (ref gv 'values))))

(define-method get-value ((gv <grid-values>) (s <string>))
  (ref (ref gv 'values) s))

(define-method set-value! ((gv <grid-values>) (s <string>) (d <string>))
  (hash-table-put! (ref gv 'values) s d))

(define-method peers ((gv <grid-values>) (s <string>))
  (hash-table-get (ref gv 'peers) s))

(define-method units ((gv <grid-values>) (s <string>))
  (hash-table-get (ref gv 'units) s))

(define-method unfilled-square/fewest-possibilities ((gv <grid-values>))
  (find-min
   (filter (lambda (p) (> (string-length (cdr p)) 1))
           (hash-table-map (ref gv 'values) (lambda (k v) (cons k v))))
   :key (lambda (p) (string-length (cdr p)))))

(define-method assign! ((gv <grid-values>) (s <string>) (d <string>))
  (let1 ds (string-delete (get-value gv s) (string->char-set d))
    (if (string-null? ds)
        gv ; Already assigned
        (do-every (c (string->list ds))
          (eliminate! gv s (string c))))))

(define-method eliminate! ((gv <grid-values>) (s <string>) (d <string>))
  (block return
    (unless (string-scan (get-value gv s) d) (return gv)) ; Already eliminated
    (unless (let* ((d2 (string-delete (get-value gv s) (string->char-set d)))
                   (len (string-length d2)))
              (set-value! gv s d2)
              (set-value! gv s d2)
              (cond ((= len 0) #f) ; Contradiction: removed last value
                    ((= len 1) (do-every (s2 (peers gv s))
                                         (eliminate! gv s2 d2)))
                    (else gv)))
      (return #f))
    (check-the-units! gv s d)))

(define-method check-the-units! ((gv <grid-values>) (s <string>) (d <string>))
  ;; Now check the places where d appears in the units of s
  (do-every (u (units gv s))
    (let* ((dplaces
            (filter
             (lambda (s) (string-scan (get-value gv s) d)) u))
           (len (length dplaces)))
      (cond ((= len 0) #f)
            ((= len 1) (assign! gv (car dplaces) d))
            (else gv)))))

;; very simple version
(define-method printboard ((gv <grid-values>))
  (dolist (r (string->list (ref gv 'rows)))
    (dolist (c (string->list (ref gv 'cols)))
      (display (hash-table-get (ref gv 'values) (string r c)))
      (display " "))
    (newline))
  (newline))

;; ----------------------------------------------------------------------

(define (parse-grid grid)
  (let1 gv (make <grid-values>)
    (dolist (sd (zip (ref gv 'squares)
                     (string->list (string-filter grid #[0._123456789]))))
      (let ((s (car sd)) (d (string (cadr sd))))
        (when (rxmatch #/[123456789]/ d) (assign! gv s d))))
    gv))

(define (search gv)
  (block return
    (unless gv (return #f))
    (when (do-every (s (ref gv 'squares))
            (= (string-length (get-value gv s)) 1))
      (return gv)) ; Solved
    (let* ((sd (unfilled-square/fewest-possibilities gv))
           (s (car sd)) (ds (cdr sd)))
      (do-any (c (string->list ds))
        (search (assign! (copy gv) s (string c)))))))

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
(time (printboard (parse-grid easy-grid)))
(time (printboard (search (parse-grid hard-grid1))))
