(use util.stream)

(define (divisible? x y) (= (remainder x y) 0))

(define (sieve stream)
  (stream-cons
   (stream-car stream)
   (sieve (stream-filter
		   (lambda (x)
			 (not (divisible? x (stream-car stream))))
		   (stream-cdr stream)))))

(define primes (sieve (stream-iota -1 2 1)))

(stream->list (stream-take primes 100))
