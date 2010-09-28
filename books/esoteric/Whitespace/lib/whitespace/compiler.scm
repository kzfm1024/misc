(define-module whitespace.compiler
  (export compile))
(select-module whitespace.compiler)

(define NUM "([ \t]+)\n")
(define LABEL "([ \t]+)\n")

(define (make-rx . args)
  (string->regexp (apply string-append args)))

(define rx-push (make-rx "^" "  " NUM))
(define rx-dup (make-rx "^" " \n "))
(define rx-copy (make-rx "^" " \t " NUM))
(define rx-swap (make-rx "^" " \n\t"))
(define rx-discard (make-rx "^" " \n\n"))
(define rx-slide (make-rx "^" " \t\n" NUM))

(define rx-add (make-rx "^" "\t   "))
(define rx-sub (make-rx "^" "\t  \t"))
(define rx-mul (make-rx "^" "\t  \n"))
(define rx-div (make-rx "^" "\t \t "))
(define rx-mod (make-rx "^" "\t \t\t"))

(define rx-heap_write (make-rx "^" "\t\t "))
(define rx-heap_read (make-rx "^" "\t\t\t"))

(define rx-label (make-rx "^" "\n  " LABEL))
(define rx-call (make-rx "^" "\n \t" LABEL))
(define rx-jump (make-rx "^" "\n \n" LABEL))
(define rx-jump_zero (make-rx "^" "\n\t " LABEL))
(define rx-jump_negative (make-rx "^" "\n\t\t" LABEL))

(define rx-return (make-rx "^" "\n\t\n"))
(define rx-exit (make-rx "^" "\n\n\n"))

(define rx-char_out (make-rx "^" "\t\n  "))
(define rx-num_out (make-rx "^" "\t\n \t"))
(define rx-char_in (make-rx "^" "\t\n\t "))
(define rx-num_in (make-rx "^" "\t\n\t\t"))

(define (num str)
  (define (plus s) (regexp-replace #/^ / s "+"))
  (define (minus s) (regexp-replace #/^\t/ s "-"))
  (define (zero s) (regexp-replace-all #/ / s "0"))
  (define (one s) (regexp-replace-all #/\t/ s "1"))
  (string->number (string-append "#b" (one (zero (minus (plus str)))))))

(define (label str) str)

(define (bleach src) (regexp-replace-all #/[^ \t\n]/ src ""))

(define (compile src)
  (define (push-step m) (cons (list '%push (num (m 1))) (step (m 'after))))
  (define (dup-step m) (cons '(%dup) (step (m 'after))))
  (define (copy-step m) (cons (list '%copy (num (m 1))) (step (m 'after))))
  (define (swap-step m) (cons '(%swap) (step (m 'after))))
  (define (discard-step m) (cons '(%discard) (step (m 'after))))
  (define (slide-step m) (cons (list '%slide (num (m 1))) (step (m 'after))))

  (define (add-step m) (cons '(%add) (step (m 'after))))
  (define (sub-step m) (cons '(%sub) (step (m 'after))))
  (define (mul-step m) (cons '(%mul) (step (m 'after))))
  (define (div-step m) (cons '(%div) (step (m 'after))))
  (define (mod-step m) (cons '(%mod) (step (m 'after))))

  (define (heap_write-step m) (cons '(%heap_write) (step (m 'after))))
  (define (heap_read-step m) (cons '(%heap_read) (step (m 'after))))

  (define (label-step m) (cons (list '%label (label (m 1))) (step (m 'after))))
  (define (call-step m) (cons (list '%call (label (m 1))) (step (m 'after))))
  (define (jump-step m) (cons (list '%jump (label (m 1))) (step (m 'after))))
  (define (jump_zero-step m)
    (cons (list '%jump_zero (label (m 1))) (step (m 'after))))
  (define (jump_negative-step m)
    (cons (list '%jump_negative (label (m 1))) (step (m 'after))))
                                       
  (define (return-step m) (cons '(%return) (step (m 'after))))
  (define (exit-step m) (cons '(%exit) (step (m 'after))))

  (define (char_out-step m) (cons '(%char_out) (step (m 'after))))
  (define (num_out-step m) (cons '(%num_out) (step (m 'after))))
  (define (char_in-step m) (cons '(%char_in) (step (m 'after))))
  (define (num_in-step m) (cons '(%num_in) (step (m 'after))))

  (define (step src)
    (cond [(rxmatch rx-push src) => (cut push-step <>)]
          [(rxmatch rx-dup src) => (cut dup-step <>)]
          [(rxmatch rx-copy src) => (cut copy-step <>)]
          [(rxmatch rx-swap src) => (cut swap-step <>)]
          [(rxmatch rx-discard src) => (cut discard-step <>)]
          [(rxmatch rx-slide src) => (cut slide-step <>)]

          [(rxmatch rx-add src) => (cut add-step <>)]
          [(rxmatch rx-sub src) => (cut sub-step <>)]
          [(rxmatch rx-mul src) => (cut mul-step <>)]
          [(rxmatch rx-div src) => (cut div-step <>)]
          [(rxmatch rx-mod src) => (cut mod-step <>)]
          
          [(rxmatch rx-heap_write src) => (cut heap_write-step <>)]
          [(rxmatch rx-heap_read src) => (cut heap_read-step <>)]

          [(rxmatch rx-label src) => (cut label-step <>)]
          [(rxmatch rx-call src) => (cut call-step <>)]
          [(rxmatch rx-jump src) => (cut jump-step <>)]
          [(rxmatch rx-jump_zero src) => (cut jump_zero-step <>)]
          [(rxmatch rx-jump_negative src) => (cut jump_negative-step <>)]

          [(rxmatch rx-exit src) => (cut exit-step <>)]
          [(rxmatch rx-return src) => (cut return-step <>)]

          [(rxmatch rx-char_out src) => (cut char_out-step <>)]
          [(rxmatch rx-num_out src) => (cut num_out-step <>)]
          [(rxmatch rx-char_in src) => (cut char_in-step <>)]
          [(rxmatch rx-num_in src) => (cut num_in-step <>)]
          [else '()]))
  (step (bleach src)))

;(print (compile "   \t\n\t\n \t\n\n\n"))
;(print (compile "   \tA\nB\tC\n \t\n\n\n"))

#|
(define (main args)
  (print (compile (port->string (current-input-port)))))
|#

(provide "whitespace.compiler")
